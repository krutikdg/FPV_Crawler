#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include "esp_wifi.h"

const char *WIFI_NAME = "Crawler";
const char *WIFI_PASSWORD = "change-this-password";
const int WIFI_CHANNEL = 6;

const char *CAMERA_ADDRESS = "192.168.4.50";
const int CAMERA_PORT = 81;
const char *CAMERA_PATH = "/stream";

const unsigned long FAILSAFE_MS = 300;
const unsigned long TELEMETRY_INTERVAL_MS = 200;
const unsigned long BATTERY_INTERVAL_MS = 250;
const unsigned long SERVO_INTERVAL_MS = 15;
const unsigned long GYRO_INTERVAL_US = 5000;

const int SPEED_LIMIT_PERCENT = 80;
const int MOTOR_DEADZONE = 8;
const int MOTOR_MIN_DUTY = 70;
const int MOTOR_MAX_DUTY = 255;
const int LEFT_MOTOR_DIRECTION = 1;
const int RIGHT_MOTOR_DIRECTION = 1;

const int SERVO_MIN_PULSE_US = 500;
const int SERVO_MAX_PULSE_US = 2400;
const int PAN_CENTER = 90;
const int PAN_RANGE = 60;
const int PAN_DIRECTION = 1;
const int TILT_CENTER = 90;
const int TILT_RANGE = 40;
const int TILT_DIRECTION = 1;
const int SERVO_STEP_DEGREES = 3;

const int HEADLIGHT_BRIGHTNESS = 255;

const float GYRO_DIRECTION = -1.0;
const float GYRO_NOISE_DPS = 0.3;
const int GYRO_CALIBRATION_SAMPLES = 200;
const float HEADING_KP = 2.0;
const int HEADING_MAX_CORRECTION = 35;
const int STRAIGHT_TURN_LIMIT = 10;
const int STRAIGHT_THROTTLE_MIN = 15;

const int STOP_DISTANCE_MM = 120;
const int SLOW_DISTANCE_MM = 350;
const int SLOW_MIN_PERCENT = 25;
const int MAX_DISTANCE_MM = 2000;

const float DIVIDER_TOP_OHMS = 100000.0;
const float DIVIDER_BOTTOM_OHMS = 47000.0;
const float BATTERY_CALIBRATION = 1.0;
const float BATTERY_PRESENT_MIN_VOLTS = 3.0;
const float LOW_BATTERY_VOLTS = 6.6;
const float EMPTY_BATTERY_VOLTS = 6.0;

const int PIN_STANDBY = 4;
const int PIN_LEFT_PWM = 25;
const int PIN_LEFT_IN1 = 26;
const int PIN_LEFT_IN2 = 27;
const int PIN_RIGHT_PWM = 33;
const int PIN_RIGHT_IN1 = 32;
const int PIN_RIGHT_IN2 = 13;
const int PIN_PAN_SERVO = 18;
const int PIN_TILT_SERVO = 19;
const int PIN_HEADLIGHTS = 23;
const int PIN_BATTERY = 34;
const int PIN_SDA = 21;
const int PIN_SCL = 22;

const int MOTOR_PWM_FREQUENCY = 20000;
const int MOTOR_PWM_BITS = 8;
const int SERVO_PWM_FREQUENCY = 50;
const int SERVO_PWM_BITS = 14;
const int LIGHT_PWM_FREQUENCY = 5000;
const int LIGHT_PWM_BITS = 8;

const int CHANNEL_LEFT_MOTOR = 0;
const int CHANNEL_RIGHT_MOTOR = 1;
const int CHANNEL_PAN_SERVO = 2;
const int CHANNEL_TILT_SERVO = 3;
const int CHANNEL_HEADLIGHTS = 4;

const uint8_t MPU_ADDRESS = 0x68;
const uint8_t MPU_REGISTER_POWER = 0x6B;
const uint8_t MPU_REGISTER_GYRO_CONFIG = 0x1B;
const uint8_t MPU_REGISTER_GYRO_Z = 0x47;
const float GYRO_COUNTS_PER_DPS = 131.0;

WebServer server(80);
WebSocketsServer webSocket(81);
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

int commandThrottle = 0;
int commandTurn = 0;
int commandPan = 0;
int commandTilt = 0;
bool commandLights = false;
bool commandAssist = true;
unsigned long lastCommandTime = 0;

int currentPan = PAN_CENTER;
int currentTilt = TILT_CENTER;
unsigned long lastServoUpdate = 0;
unsigned long lastBatteryRead = 0;
unsigned long lastTelemetry = 0;

float batteryVolts = 0;
int distanceMm = MAX_DISTANCE_MM;
float headingDegrees = 0;
float targetHeading = 0;
bool holdingHeading = false;
float gyroBias = 0;
bool gyroReady = false;
bool distanceReady = false;
unsigned long lastGyroMicros = 0;

void startPwm(int pin, int channel, int frequency, int bits) {
#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcAttach(pin, frequency, bits);
#else
  ledcSetup(channel, frequency, bits);
  ledcAttachPin(pin, channel);
#endif
}

void writePwm(int pin, int channel, int value) {
#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcWrite(pin, value);
#else
  ledcWrite(channel, value);
#endif
}

void setMotor(int pwmPin, int pwmChannel, int in1Pin, int in2Pin, int speedPercent) {
  if (abs(speedPercent) < MOTOR_DEADZONE) {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);
    writePwm(pwmPin, pwmChannel, 0);
    return;
  }

  if (speedPercent > 0) {
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
  } else {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
  }

  int duty = map(abs(speedPercent), MOTOR_DEADZONE, 100, MOTOR_MIN_DUTY, MOTOR_MAX_DUTY);
  writePwm(pwmPin, pwmChannel, duty);
}

void driveMotors(int leftPercent, int rightPercent) {
  leftPercent = constrain(leftPercent, -100, 100) * LEFT_MOTOR_DIRECTION;
  rightPercent = constrain(rightPercent, -100, 100) * RIGHT_MOTOR_DIRECTION;
  setMotor(PIN_LEFT_PWM, CHANNEL_LEFT_MOTOR, PIN_LEFT_IN1, PIN_LEFT_IN2, leftPercent);
  setMotor(PIN_RIGHT_PWM, CHANNEL_RIGHT_MOTOR, PIN_RIGHT_IN1, PIN_RIGHT_IN2, rightPercent);
}

void stopMotors() {
  driveMotors(0, 0);
}

int angleToDuty(int angle) {
  angle = constrain(angle, 0, 180);
  long pulse = map(angle, 0, 180, SERVO_MIN_PULSE_US, SERVO_MAX_PULSE_US);
  long maxDuty = (1L << SERVO_PWM_BITS) - 1;
  return (int)(pulse * maxDuty / 20000L);
}

void writeServo(int pin, int channel, int angle) {
  writePwm(pin, channel, angleToDuty(angle));
}

int moveToward(int current, int target, int step) {
  if (current < target) {
    return min(current + step, target);
  }
  if (current > target) {
    return max(current - step, target);
  }
  return current;
}

void centerCameraHead() {
  currentPan = PAN_CENTER;
  currentTilt = TILT_CENTER;
  writeServo(PIN_PAN_SERVO, CHANNEL_PAN_SERVO, currentPan);
  writeServo(PIN_TILT_SERVO, CHANNEL_TILT_SERVO, currentTilt);
}

void updateCameraHead() {
  if (millis() - lastServoUpdate < SERVO_INTERVAL_MS) {
    return;
  }
  lastServoUpdate = millis();

  int targetPan = PAN_CENTER + PAN_DIRECTION * commandPan * PAN_RANGE / 100;
  int targetTilt = TILT_CENTER + TILT_DIRECTION * commandTilt * TILT_RANGE / 100;

  currentPan = moveToward(currentPan, targetPan, SERVO_STEP_DEGREES);
  currentTilt = moveToward(currentTilt, targetTilt, SERVO_STEP_DEGREES);

  writeServo(PIN_PAN_SERVO, CHANNEL_PAN_SERVO, currentPan);
  writeServo(PIN_TILT_SERVO, CHANNEL_TILT_SERVO, currentTilt);
}

bool batteryPresent() {
  return batteryVolts > BATTERY_PRESENT_MIN_VOLTS;
}

bool batteryLow() {
  return batteryPresent() && batteryVolts < LOW_BATTERY_VOLTS;
}

bool batteryEmpty() {
  return batteryPresent() && batteryVolts < EMPTY_BATTERY_VOLTS;
}

void readBattery() {
  int millivolts = analogReadMilliVolts(PIN_BATTERY);
  float pinVolts = millivolts / 1000.0;
  float volts = pinVolts * (DIVIDER_TOP_OHMS + DIVIDER_BOTTOM_OHMS) / DIVIDER_BOTTOM_OHMS;
  volts = volts * BATTERY_CALIBRATION;

  if (batteryVolts == 0) {
    batteryVolts = volts;
  } else {
    batteryVolts = batteryVolts * 0.9 + volts * 0.1;
  }
}

void updateBattery() {
  if (millis() - lastBatteryRead < BATTERY_INTERVAL_MS) {
    return;
  }
  lastBatteryRead = millis();
  readBattery();
}

void updateHeadlights() {
  int duty = 0;
  if (commandLights && !batteryEmpty()) {
    duty = HEADLIGHT_BRIGHTNESS;
  }
  writePwm(PIN_HEADLIGHTS, CHANNEL_HEADLIGHTS, duty);
}

bool startGyro() {
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(MPU_REGISTER_POWER);
  Wire.write(0);
  if (Wire.endTransmission() != 0) {
    return false;
  }

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(MPU_REGISTER_GYRO_CONFIG);
  Wire.write(0);
  Wire.endTransmission();
  return true;
}

float readGyroRate() {
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(MPU_REGISTER_GYRO_Z);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS, (uint8_t)2);

  int high = Wire.read();
  int low = Wire.read();
  int16_t rawValue = (int16_t)((high << 8) | low);
  return rawValue / GYRO_COUNTS_PER_DPS;
}

void calibrateGyro() {
  float total = 0;
  for (int i = 0; i < GYRO_CALIBRATION_SAMPLES; i++) {
    total = total + readGyroRate();
    delay(5);
  }
  gyroBias = total / GYRO_CALIBRATION_SAMPLES;
}

void updateHeading() {
  if (!gyroReady) {
    return;
  }

  unsigned long now = micros();
  if (now - lastGyroMicros < GYRO_INTERVAL_US) {
    return;
  }
  float seconds = (now - lastGyroMicros) / 1000000.0;
  lastGyroMicros = now;

  float rate = (readGyroRate() - gyroBias) * GYRO_DIRECTION;
  if (fabs(rate) < GYRO_NOISE_DPS) {
    rate = 0;
  }
  headingDegrees = headingDegrees + rate * seconds;
}

void updateDistance() {
  if (!distanceReady) {
    return;
  }
  if (!lox.isRangeComplete()) {
    return;
  }

  int reading = lox.readRange();
  if (reading > MAX_DISTANCE_MM) {
    reading = MAX_DISTANCE_MM;
  }
  distanceMm = reading;
}

int applyAutoBrake(int throttle) {
  if (!commandAssist || !distanceReady) {
    return throttle;
  }
  if (throttle <= 0) {
    return throttle;
  }
  if (distanceMm <= STOP_DISTANCE_MM) {
    return 0;
  }
  if (distanceMm < SLOW_DISTANCE_MM) {
    int allowedPercent = map(distanceMm, STOP_DISTANCE_MM, SLOW_DISTANCE_MM, SLOW_MIN_PERCENT, 100);
    return throttle * allowedPercent / 100;
  }
  return throttle;
}

int applyHeadingHold(int throttle, int turn) {
  bool drivingStraight = commandAssist && gyroReady;
  drivingStraight = drivingStraight && abs(turn) < STRAIGHT_TURN_LIMIT;
  drivingStraight = drivingStraight && abs(throttle) > STRAIGHT_THROTTLE_MIN;

  if (!drivingStraight) {
    holdingHeading = false;
    return turn;
  }

  if (!holdingHeading) {
    targetHeading = headingDegrees;
    holdingHeading = true;
  }

  float error = targetHeading - headingDegrees;
  int correction = (int)(error * HEADING_KP);
  correction = constrain(correction, -HEADING_MAX_CORRECTION, HEADING_MAX_CORRECTION);
  return turn + correction;
}

void updateDriving() {
  if (millis() - lastCommandTime > FAILSAFE_MS) {
    holdingHeading = false;
    stopMotors();
    return;
  }

  if (batteryEmpty()) {
    holdingHeading = false;
    stopMotors();
    return;
  }

  int throttle = commandThrottle * SPEED_LIMIT_PERCENT / 100;
  int turn = commandTurn * SPEED_LIMIT_PERCENT / 100;

  throttle = applyAutoBrake(throttle);
  turn = applyHeadingHold(throttle, turn);

  int leftPercent = throttle + turn;
  int rightPercent = throttle - turn;
  driveMotors(leftPercent, rightPercent);
}

int readWeakestSignal() {
  wifi_sta_list_t stations;
  if (esp_wifi_ap_get_sta_list(&stations) != ESP_OK) {
    return 0;
  }
  if (stations.num == 0) {
    return 0;
  }

  int weakest = 0;
  for (int i = 0; i < stations.num; i++) {
    int strength = stations.sta[i].rssi;
    if (i == 0 || strength < weakest) {
      weakest = strength;
    }
  }
  return weakest;
}

void handleCommandText(uint8_t clientNumber, const char *text) {
  int throttle = 0;
  int turn = 0;
  int pan = 0;
  int tilt = 0;
  int lights = 0;
  int assist = 0;

  int fieldsRead = sscanf(text, "M,%d,%d,%d,%d,%d,%d", &throttle, &turn, &pan, &tilt, &lights, &assist);
  if (fieldsRead == 6) {
    commandThrottle = constrain(throttle, -100, 100);
    commandTurn = constrain(turn, -100, 100);
    commandPan = constrain(pan, -100, 100);
    commandTilt = constrain(tilt, -100, 100);
    commandLights = (lights == 1);
    commandAssist = (assist == 1);
    lastCommandTime = millis();
    return;
  }

  if (text[0] == 'P' && text[1] == ',') {
    webSocket.sendTXT(clientNumber, text);
  }
}

void handleSocketEvent(uint8_t clientNumber, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_DISCONNECTED) {
    commandThrottle = 0;
    commandTurn = 0;
    commandPan = 0;
    commandTilt = 0;
    stopMotors();
    return;
  }

  if (type != WStype_TEXT) {
    return;
  }
  if (length == 0 || length >= 64) {
    return;
  }

  char text[64];
  memcpy(text, payload, length);
  text[length] = '\0';
  handleCommandText(clientNumber, text);
}

void sendTelemetry() {
  if (millis() - lastTelemetry < TELEMETRY_INTERVAL_MS) {
    return;
  }
  lastTelemetry = millis();

  if (webSocket.connectedClients() == 0) {
    return;
  }

  int holding = 0;
  if (holdingHeading) {
    holding = 1;
  }
  int low = 0;
  if (batteryLow()) {
    low = 1;
  }

  char message[80];
  snprintf(message, sizeof(message), "T,%.2f,%d,%d,%.1f,%d,%d",
           batteryVolts, distanceMm, readWeakestSignal(), headingDegrees, holding, low);
  webSocket.broadcastTXT(message);
}

const char PAGE_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
<title>Crawler</title>
<style>
html, body {
  margin: 0;
  height: 100%;
  background: #000;
  color: #fff;
  font-family: sans-serif;
  overflow: hidden;
  touch-action: none;
  user-select: none;
  -webkit-user-select: none;
}
#video {
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  object-fit: contain;
}
#status {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  display: flex;
  flex-wrap: wrap;
  gap: 12px;
  padding: 6px 12px;
  font-size: 14px;
  background: rgba(0, 0, 0, 0.5);
}
#stick {
  position: fixed;
  left: 32px;
  bottom: 32px;
  width: 160px;
  height: 160px;
  border-radius: 50%;
  background: rgba(255, 255, 255, 0.15);
  border: 2px solid rgba(255, 255, 255, 0.4);
}
#knob {
  position: absolute;
  left: 50px;
  top: 50px;
  width: 60px;
  height: 60px;
  border-radius: 50%;
  background: rgba(255, 255, 255, 0.7);
}
#buttons {
  position: fixed;
  right: 24px;
  bottom: 32px;
  display: flex;
  flex-direction: column;
  gap: 10px;
}
button {
  width: 110px;
  height: 44px;
  font-size: 14px;
  color: #fff;
  background: rgba(0, 0, 0, 0.5);
  border: 2px solid #fff;
  border-radius: 8px;
}
button.on {
  background: #1a8f3c;
}
.warning {
  color: #ff5a5a;
}
</style>
</head>
<body>
<img id="video" src="CAMERA_URL_HERE">
<div id="status">
  <span id="connection">OFFLINE</span>
  <span id="battery">BAT --</span>
  <span id="distance">DIST --</span>
  <span id="signal">WIFI --</span>
  <span id="ping">PING --</span>
  <span id="heading">HDG --</span>
  <span id="tilt">TILT --</span>
</div>
<div id="stick"><div id="knob"></div></div>
<div id="buttons">
  <button id="lightsButton">LIGHTS</button>
  <button id="assistButton" class="on">ASSIST</button>
  <button id="centerButton">CENTER</button>
  <button id="fullscreenButton">FULL</button>
</div>
<script>
var TILT_RANGE_DEGREES = 30;
var TILT_PAN_SIGN = 1;
var TILT_UP_SIGN = 1;
var SEND_INTERVAL_MS = 50;
var PING_INTERVAL_MS = 1000;
var TILT_WAIT_MS = 2000;
var STICK_TRAVEL = 50;

var video = document.getElementById("video");
var stick = document.getElementById("stick");
var knob = document.getElementById("knob");
var connectionLabel = document.getElementById("connection");
var batteryLabel = document.getElementById("battery");
var distanceLabel = document.getElementById("distance");
var signalLabel = document.getElementById("signal");
var pingLabel = document.getElementById("ping");
var headingLabel = document.getElementById("heading");
var tiltLabel = document.getElementById("tilt");
var lightsButton = document.getElementById("lightsButton");
var assistButton = document.getElementById("assistButton");
var centerButton = document.getElementById("centerButton");
var fullscreenButton = document.getElementById("fullscreenButton");

var CAMERA_URL = video.getAttribute("src");
var socket = null;
var throttle = 0;
var turn = 0;
var panValue = 0;
var tiltValue = 0;
var lightsOn = false;
var assistOn = true;
var tiltWorking = false;
var neutralBeta = null;
var neutralGamma = null;
var lastBeta = 0;
var lastGamma = 0;
var activePointer = null;

function clamp(value, low, high) {
  return Math.min(high, Math.max(low, value));
}

function connect() {
  socket = new WebSocket("ws://" + location.hostname + ":81/");
  socket.onopen = function () {
    connectionLabel.textContent = "ONLINE";
  };
  socket.onclose = function () {
    connectionLabel.textContent = "OFFLINE";
    setTimeout(connect, 1000);
  };
  socket.onmessage = handleMessage;
}

function handleMessage(event) {
  var parts = event.data.split(",");

  if (parts[0] === "T") {
    batteryLabel.textContent = "BAT " + parts[1] + "V";
    distanceLabel.textContent = "DIST " + parts[2] + "mm";
    signalLabel.textContent = "WIFI " + parts[3] + "dBm";
    headingLabel.textContent = "HDG " + parts[4];
    if (parts[6] === "1") {
      batteryLabel.className = "warning";
    } else {
      batteryLabel.className = "";
    }
  }

  if (parts[0] === "P") {
    var delay = Math.round(performance.now() - parseFloat(parts[1]));
    pingLabel.textContent = "PING " + delay + "ms";
  }
}

function sendCommand() {
  if (!socket || socket.readyState !== 1) {
    return;
  }
  var lights = lightsOn ? 1 : 0;
  var assist = assistOn ? 1 : 0;
  var message = "M," + Math.round(throttle) + "," + Math.round(turn) + "," +
    Math.round(panValue) + "," + Math.round(tiltValue) + "," + lights + "," + assist;
  socket.send(message);
}

function sendPing() {
  if (!socket || socket.readyState !== 1) {
    return;
  }
  socket.send("P," + Math.round(performance.now()));
}

function moveStick(event) {
  var box = stick.getBoundingClientRect();
  var dx = event.clientX - (box.left + box.width / 2);
  var dy = event.clientY - (box.top + box.height / 2);
  var distance = Math.sqrt(dx * dx + dy * dy);

  if (distance > STICK_TRAVEL) {
    dx = dx * STICK_TRAVEL / distance;
    dy = dy * STICK_TRAVEL / distance;
  }

  knob.style.transform = "translate(" + dx + "px, " + dy + "px)";
  turn = dx / STICK_TRAVEL * 100;
  throttle = -dy / STICK_TRAVEL * 100;
}

function releaseStick(event) {
  if (event.pointerId !== activePointer) {
    return;
  }
  activePointer = null;
  knob.style.transform = "translate(0px, 0px)";
  throttle = 0;
  turn = 0;
}

stick.addEventListener("pointerdown", function (event) {
  activePointer = event.pointerId;
  stick.setPointerCapture(event.pointerId);
  moveStick(event);
});

stick.addEventListener("pointermove", function (event) {
  if (event.pointerId === activePointer) {
    moveStick(event);
  }
});

stick.addEventListener("pointerup", releaseStick);
stick.addEventListener("pointercancel", releaseStick);

lightsButton.addEventListener("click", function () {
  lightsOn = !lightsOn;
  lightsButton.className = lightsOn ? "on" : "";
});

assistButton.addEventListener("click", function () {
  assistOn = !assistOn;
  assistButton.className = assistOn ? "on" : "";
});

centerButton.addEventListener("click", function () {
  neutralBeta = lastBeta;
  neutralGamma = lastGamma;
});

fullscreenButton.addEventListener("click", function () {
  document.documentElement.requestFullscreen()
    .then(function () {
      return screen.orientation.lock("landscape");
    })
    .catch(function () {
      console.log("Fullscreen or rotation lock was refused");
    });
});

function handleOrientation(event) {
  if (event.beta === null || event.gamma === null) {
    return;
  }

  tiltWorking = true;
  lastBeta = event.beta;
  lastGamma = event.gamma;

  if (neutralBeta === null) {
    neutralBeta = event.beta;
    neutralGamma = event.gamma;
  }

  var landscape = window.innerWidth > window.innerHeight;
  if (!landscape) {
    panValue = 0;
    tiltValue = 0;
    return;
  }

  var flip = 1;
  if (screen.orientation && screen.orientation.angle === 270) {
    flip = -1;
  }

  var panDegrees = (event.beta - neutralBeta) * flip * TILT_PAN_SIGN;
  var tiltDegrees = (event.gamma - neutralGamma) * flip * TILT_UP_SIGN;
  panValue = clamp(panDegrees / TILT_RANGE_DEGREES * 100, -100, 100);
  tiltValue = clamp(tiltDegrees / TILT_RANGE_DEGREES * 100, -100, 100);
}

function checkTilt() {
  if (tiltWorking) {
    tiltLabel.textContent = "TILT ON";
  } else {
    panValue = 0;
    tiltValue = 0;
    tiltLabel.textContent = "TILT OFF";
  }
}

function reloadVideo() {
  video.src = CAMERA_URL + "?t=" + Date.now();
}

video.addEventListener("error", function () {
  setTimeout(reloadVideo, 1000);
});

window.addEventListener("deviceorientation", handleOrientation);
setTimeout(checkTilt, TILT_WAIT_MS);
setInterval(sendCommand, SEND_INTERVAL_MS);
setInterval(sendPing, PING_INTERVAL_MS);
connect();
</script>
</body>
</html>
)rawliteral";

void sendPage() {
  String cameraUrl = String("http://") + CAMERA_ADDRESS + ":" + CAMERA_PORT + CAMERA_PATH;
  String page = String(PAGE_HTML);
  page.replace("CAMERA_URL_HERE", cameraUrl);
  server.send(200, "text/html", page);
}

void startNetwork() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_NAME, WIFI_PASSWORD, WIFI_CHANNEL);
  WiFi.setSleep(false);

  server.on("/", sendPage);
  server.begin();

  webSocket.begin();
  webSocket.onEvent(handleSocketEvent);

  Serial.print("Open http://");
  Serial.println(WiFi.softAPIP());
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_STANDBY, OUTPUT);
  digitalWrite(PIN_STANDBY, LOW);

  pinMode(PIN_LEFT_IN1, OUTPUT);
  pinMode(PIN_LEFT_IN2, OUTPUT);
  pinMode(PIN_RIGHT_IN1, OUTPUT);
  pinMode(PIN_RIGHT_IN2, OUTPUT);

  startPwm(PIN_LEFT_PWM, CHANNEL_LEFT_MOTOR, MOTOR_PWM_FREQUENCY, MOTOR_PWM_BITS);
  startPwm(PIN_RIGHT_PWM, CHANNEL_RIGHT_MOTOR, MOTOR_PWM_FREQUENCY, MOTOR_PWM_BITS);
  startPwm(PIN_PAN_SERVO, CHANNEL_PAN_SERVO, SERVO_PWM_FREQUENCY, SERVO_PWM_BITS);
  startPwm(PIN_TILT_SERVO, CHANNEL_TILT_SERVO, SERVO_PWM_FREQUENCY, SERVO_PWM_BITS);
  startPwm(PIN_HEADLIGHTS, CHANNEL_HEADLIGHTS, LIGHT_PWM_FREQUENCY, LIGHT_PWM_BITS);

  stopMotors();
  centerCameraHead();
  writePwm(PIN_HEADLIGHTS, CHANNEL_HEADLIGHTS, 0);

  analogSetPinAttenuation(PIN_BATTERY, ADC_11db);
  readBattery();

  Wire.begin(PIN_SDA, PIN_SCL);
  Wire.setClock(400000);

  gyroReady = startGyro();
  if (gyroReady) {
    calibrateGyro();
    lastGyroMicros = micros();
  } else {
    Serial.println("Gyro not found");
  }

  distanceReady = lox.begin();
  if (distanceReady) {
    lox.startRangeContinuous();
  } else {
    Serial.println("Distance sensor not found");
  }

  startNetwork();
  digitalWrite(PIN_STANDBY, HIGH);
}

void loop() {
  server.handleClient();
  webSocket.loop();

  updateHeading();
  updateDistance();
  updateBattery();

  updateDriving();
  updateCameraHead();
  updateHeadlights();

  sendTelemetry();
}
