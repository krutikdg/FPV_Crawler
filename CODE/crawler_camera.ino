#include "esp_camera.h"
#include "esp_http_server.h"
#include <WiFi.h>

const char *WIFI_NAME = "Crawler";
const char *WIFI_PASSWORD = "change-this-password";

IPAddress cameraAddress(192, 168, 4, 50);
IPAddress gatewayAddress(192, 168, 4, 1);
IPAddress subnetMask(255, 255, 255, 0);

const int STREAM_PORT = 81;
const framesize_t FRAME_SIZE = FRAMESIZE_QVGA;
const int JPEG_QUALITY = 12;
const bool FLIP_VERTICAL = false;
const bool MIRROR_HORIZONTAL = false;
const unsigned long WIFI_TIMEOUT_MS = 15000;
const unsigned long WIFI_CHECK_MS = 2000;
const int MAX_FAILED_GRABS = 20;

const int CAMERA_PIN_XCLK = 10;
const int CAMERA_PIN_SDA = 40;
const int CAMERA_PIN_SCL = 39;
const int CAMERA_PIN_D0 = 15;
const int CAMERA_PIN_D1 = 17;
const int CAMERA_PIN_D2 = 18;
const int CAMERA_PIN_D3 = 16;
const int CAMERA_PIN_D4 = 14;
const int CAMERA_PIN_D5 = 12;
const int CAMERA_PIN_D6 = 11;
const int CAMERA_PIN_D7 = 48;
const int CAMERA_PIN_VSYNC = 38;
const int CAMERA_PIN_HREF = 47;
const int CAMERA_PIN_PCLK = 13;

const char *STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=frame";
const char *STREAM_BOUNDARY = "\r\n--frame\r\n";
const char *STREAM_PART_HEADER = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

httpd_handle_t streamServer = NULL;

bool startCamera() {
  camera_config_t config = {};

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = CAMERA_PIN_D0;
  config.pin_d1 = CAMERA_PIN_D1;
  config.pin_d2 = CAMERA_PIN_D2;
  config.pin_d3 = CAMERA_PIN_D3;
  config.pin_d4 = CAMERA_PIN_D4;
  config.pin_d5 = CAMERA_PIN_D5;
  config.pin_d6 = CAMERA_PIN_D6;
  config.pin_d7 = CAMERA_PIN_D7;
  config.pin_xclk = CAMERA_PIN_XCLK;
  config.pin_pclk = CAMERA_PIN_PCLK;
  config.pin_vsync = CAMERA_PIN_VSYNC;
  config.pin_href = CAMERA_PIN_HREF;
  config.pin_sccb_sda = CAMERA_PIN_SDA;
  config.pin_sccb_scl = CAMERA_PIN_SCL;
  config.pin_pwdn = -1;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAME_SIZE;
  config.jpeg_quality = JPEG_QUALITY;
  config.fb_count = 2;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.grab_mode = CAMERA_GRAB_LATEST;

  esp_err_t result = esp_camera_init(&config);
  if (result != ESP_OK) {
    Serial.printf("Camera start failed, error 0x%x\n", result);
    return false;
  }

  sensor_t *sensor = esp_camera_sensor_get();
  if (sensor != NULL) {
    sensor->set_vflip(sensor, FLIP_VERTICAL ? 1 : 0);
    sensor->set_hmirror(sensor, MIRROR_HORIZONTAL ? 1 : 0);
  }
  return true;
}

esp_err_t streamHandler(httpd_req_t *request) {
  esp_err_t result = httpd_resp_set_type(request, STREAM_CONTENT_TYPE);
  if (result != ESP_OK) {
    return result;
  }
  httpd_resp_set_hdr(request, "Access-Control-Allow-Origin", "*");

  char partHeader[80];
  int failedGrabs = 0;

  while (true) {
    camera_fb_t *frame = esp_camera_fb_get();
    if (frame == NULL) {
      failedGrabs++;
      if (failedGrabs > MAX_FAILED_GRABS) {
        return ESP_FAIL;
      }
      delay(10);
      continue;
    }
    failedGrabs = 0;

    int headerLength = snprintf(partHeader, sizeof(partHeader), STREAM_PART_HEADER, (unsigned int)frame->len);

    result = httpd_resp_send_chunk(request, STREAM_BOUNDARY, strlen(STREAM_BOUNDARY));
    if (result == ESP_OK) {
      result = httpd_resp_send_chunk(request, partHeader, headerLength);
    }
    if (result == ESP_OK) {
      result = httpd_resp_send_chunk(request, (const char *)frame->buf, frame->len);
    }

    esp_camera_fb_return(frame);

    if (result != ESP_OK) {
      break;
    }
  }
  return result;
}

void startStreamServer() {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = STREAM_PORT;
  config.ctrl_port = STREAM_PORT + 1;

  httpd_uri_t streamUri;
  memset(&streamUri, 0, sizeof(streamUri));
  streamUri.uri = "/stream";
  streamUri.method = HTTP_GET;
  streamUri.handler = streamHandler;
  streamUri.user_ctx = NULL;

  if (httpd_start(&streamServer, &config) == ESP_OK) {
    httpd_register_uri_handler(streamServer, &streamUri);
    Serial.println("Stream server started");
  } else {
    Serial.println("Stream server failed to start");
  }
}

void connectWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.config(cameraAddress, gatewayAddress, subnetMask);
  WiFi.setSleep(false);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_TIMEOUT_MS) {
    delay(250);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected, address ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Could not join the Crawler network yet");
  }
}

void setup() {
  Serial.begin(115200);

  if (!startCamera()) {
    delay(5000);
    ESP.restart();
  }

  connectWifi();
  startStreamServer();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost, reconnecting");
    WiFi.disconnect();
    WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  }
  delay(WIFI_CHECK_MS);
}
