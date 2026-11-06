# FPV_Crawler
An FPV remote controlled car that can be controlled from the browser and also shows you live feed from the camera to your phone. ESP32 creates its own wifi network and serves a joystick page. A separate camera board streams the video, which means the controls keep working even if the video lags. The camera tilts according to the gyro of your phone, if not allowed, it stays fixed centre straight by default. It also features LED headlights and object detection and stop by itself to prevent accidents. The car also stops by itself if the connection drops. I built this primarily for fun and to learn how video, control, and sensors fit on a tiny board. 

# Built With

- ESP32 DevKit (38-pin ESP32-WROOM-32 board)
- XIAO ESP32-S3 Sense (with camera
- JGA25-370 12V metal gearmotor
- TB6612FNG motor driver module
- MPU6050 module
- VL53L0X distance sensor module
- Micro servo SG90
- etc

# Bill Of Materials

| Component | Qty | Price/Unit (USD) | Purpose | Purchase Link |
|---|---|---|---|---|
| ESP32 DevKit (38-pin ESP32-WROOM-32 board) | 1 | 4.50 | Main controller that hosts the WiFi hotspot and joystick page, runs failsafe, drives motors and servos, reads sensors | https://www.google.com/aclk?sa=L&ai=DChsSEwih6OXPyO6XAxX6mWYCHf9CAdgYACICCAEQDBoCc20&co=1&ase=2&gclid=CjwKCAiAzKvXBhBAEiwABLl9AqgLKM4862Ra9j8Mfk9vBmhgFz_DY9BM76b8euHzZuM8oG0pbzVDORoCnlMQAvD_BwE&cid=CAAS9gHkaHqIoCnP3ZoCWaz18t8OVUfGfJV7OnsTkAfojIyqaz17ndYDJWU5Ys5b3-UqWbJMqOI075r46qo6Hmyk8pPKoyDKQtDc1WOKp9vHSvB4hM-IHGdzKD1RhWjaks1rX7RO1vIO9Jyg20jFtlMTftVuexyVJ4TOGZsVEh8GRdGM7k6211nJif9Is6n4eYkQGvx-WiImKJ5266bo46wjcRMNxwxdiHnWWOjBvxMvLC5p1xYzsSPjwk866eksrWAC4Nt4-NkXFjztsgaC7zHv7btFT0CbgrJOiSM8MB9esr84U9gHAPgZTlnAN7ulRaykNpNARFlkojM&cce=2&category=acrcp_v1_32&sig=AOD64_3G2b1EEXrv8kYTRpAVRFWR_6jwLA&ctype=5&q=&nis=4&ved=2ahUKEwja4d_PyO6XAxUZi-EIHWWwGVwQ5bgDKAB6BAgNEAs&adurl= |
| XIAO ESP32-S3 Sense (with camera) | 1 | 18 | Camera board, streams live video to the phone | https://www.google.com/aclk?sa=L&ai=DChsSEwiZ067uyO6XAxUxq0sFHU2QHxcYACICCAEQCxoCc2Y&co=1&ase=2&gclid=CjwKCAiAzKvXBhBAEiwABLl9Ahf07yNIYSGsTCiI7k3lyv4TmNMX6kfHwGAuNDgccsO574vu7jHuwxoCgXgQAvD_BwE&cid=CAAS9gHkaPe1jEgGpA7459gN_rvULGLmvn7OAvsoITljTXWe1mLX6pYuNCqD53MBpR4j9M_FzgsOiysXXNfAwyE_NhhZB8mqM-rN3z2nPSm3BHXhyWNtpqY1Gh5uCW5zn3YhMFC8ZgQV_yGLLlK2AJ9BzA-YNkEfwuaNUL42-H73Gb36QMvgYHle1dHPGtCrAAhojSl3-fcMOUYTaq63BwKkgapYXba2BJdq6GaHoTXsVg55bAOlcX06H5uh4mlXI5-OS0TSbYCoZvLo-hbpE0lxcOu-IqqySjUK7G45HD9gmBUv3Y2ptLHEOhJRvEZlb2W0uehIsBRLGuY&cce=2&category=acrcp_v1_32&sig=AOD64_2jlgizCvVPvKwYkhFFmeE5NIBWQg&ctype=5&q=&nis=4&ved=2ahUKEwi8sKLuyO6XAxXeleEIHYa7IiEQ5bgDKAB6BAgbEBY&adurl= |
| JGA25-370 12V metal gearmotor (D-shaft 4 mm, no encoder, about 120 RPM) | 4 | 4.43 | Drive for 4WD; two motors share each TB6612 channel; check shaft length (21 mm) and body length (52.6 mm) against the listing | https://www.google.com/aclk?sa=L&ai=DChsSEwiA14OXye6XAxXBjEsFHSt8FwcYACICCAEQBBoCc2Y&co=1&ase=2&gclid=CjwKCAiAzKvXBhBAEiwABLl9Ak_s9Kbhox6kzyW37nsYAueqohmxpguHX7z5je8j-nZ45IJRAehDxRoCeOQQAvD_BwE&cid=CAAS9gHkaG-wwpXm_rMGjVHqQu6JXlrfeFGmyTL_wo8H9b_bJ1KMRSBlfOuD9kbKj8TNa8Qec6ROhLBV26GiOUqbfjHWxsUKGC_CPjmMnZdb90rSSgU1lw4KQhSG4Fs7z4uBv1pDcWgaT_vmi0c1B3Bm8rfqUI_xCyIv3iWwf2CtxJtRJpSDT8gIKv1GYAdFLhEuOL9A1dxiIw9Pm6MdcX27v0HCgUauO9-QLGMfqint_J_K8MAB15LE6B2BAr7HG3KCLzH7ZYUXfyOYuR0JDdKRwfIgtGvLXX-evj_KJLOiXn_dqDzm-DtvW29Q1HDmViZCM1nvLqgpQP8&cce=2&category=acrcp_v1_32&sig=AOD64_3CZYhk1eCv4D7Eid6ashm62-QyOg&ctype=5&q=&nis=4&ved=2ahUKEwjc2PyWye6XAxW-m-EIHfNPKTIQ5bgDKAB6BAgNEAs&adurl= |
| Rubber wheel 65 mm with 4 mm D-shaft hub | 4 | 1.8 | Wheels for the JGA25 shaft| https://www.google.com/aclk?sa=L&ai=DChsSEwidmLSrye6XAxUWxjwCHczfJxcYACICCAEQBRoCc2Y&co=1&ase=2&gclid=CjwKCAiAzKvXBhBAEiwABLl9Aq1tw459Bo8V1xuak05--aRRFraZB7XwlAMLI0yxC9WcBp4Rd4sxzBoCjEoQAvD_BwE&cid=CAAS9gHkaOlmhgUOC_AIlOrK4oS1nY1C2D5ESz0s5HsPlH-3Rze08Ln5vWqRo33t7-yl1sL230EpYkGwBQ4z_8drfTzkfiHHpAV2kVnXP0WKt7TgHNk1B_utBYS3sVjWt9vBVLkb5uGLGrFbGKsZpPFRWAOnYHLJogU-AH9JI_geanb2ObV7jNpr9FkzAWx-Lf-XCEyskJXEPDUt8L5-ckuR7qAQzqLdIFEvsIaSosFMbjvJAkp7BhcMTfaa0L-GrqykglL6tnavpIRSmVx56A3moy_tVkOGwJLG79tZC0QMDcFt_t_Tb4a6_8tIq6FXWGIxhCOyWn8ONmI&cce=2&category=acrcp_v1_32&sig=AOD64_2m5c38UVBbIsHxAR4_KaSNnNU7gA&ctype=46&q=&nis=4&ved=2ahUKEwjpza2rye6XAxXli-EIHX4lARAQ5bgDKAB6BAgOEAs&adurl= |
| TB6612FNG motor driver module | 1 | 1.6 | Drives the left and right motor pairs | https://www.google.com/aclk?sa=L&ai=DChsSEwiKz96-ye6XAxWamWYCHd2DCIEYACICCAEQChoCc20&co=1&ase=2&gclid=CjwKCAiAzKvXBhBAEiwABLl9Ahugvdq5vtvJuWrT1QH4oHdlBSXwlThFCfc8W2HKKbvyb8sd2ZGnKRoCx08QAvD_BwE&cid=CAAS9gHkaLvmqqEBjE6u7jbMX390SGf_TPe0jY-UFZ5ZfjvnwgznhBlpiWFWaZoNax-4R6r4CQSzEGVf4p0E5ExxtywFX5t9wvEY0s40-4Iwi--2YZnhGAz-8o07IwMzzuQNPmOcgSXdj1dP2wpte6pgonW0Z2oF8p2bPX-Z4FFqdVmGvV0IpwYTCTAKgCvSJlY9xvhgU7RqTITs-XfFfNkOoXN9EXn7ScF40ZLennMLBJeoR48ojGolCO4PPUDM5oybIXelQk366SEc3uItoq4yX8sCApkKBBaYHPoUuIS-aesohYk-JOhDCcep9WmZcd2BrE5twfT9q8Y&cce=2&category=acrcp_v1_32&sig=AOD64_1DBom574c-dp0Nrcjb7FWcMgK8Tw&ctype=5&q=&nis=4&ved=2ahUKEwjRiti-ye6XAxWdluEIHZSfFaIQ5bgDKAB6BAgTEAs&adurl= |
| 18650 Li-ion cell (protected) | 2 | 0.8 | Battery pack in series | https://www.google.com/aclk?sa=L&ai=DChsSEwi38sXeye6XAxUDhGYCHTHOLu4YACICCAEQARoCc20&co=1&ase=2&gclid=CjwKCAiAzKvXBhBAEiwABLl9AnBLORiAkUd0tmaP8Ry8Plc88QaMxEeYedkhAz-pyx2-NpUZ_YnGRRoCzs4QAvD_BwE&cid=CAAS9gHkaIoR4Z8aJumPbM95Qy1GPuogfO-YnsE5biSzdMzqRTIS2mDBZQnYU9Q-CCb5yAQT2O4xDF9IlSVBZ4eV0MRynOQeUxdTkgRfur4qi2NLvx1VUNSXrQZLoVnzT7Pdtinve-RPYXeSSXFzqBvLWYrIzV27-vdQVlgjY2JUtmOl9_xrTtEnSsgGABm-CX7ABqlUGCUEAPMXSM-QbllOKbkYYmlSL5nC8Oh1_Cse1OTtbCjcl56QLWPyj9rU1Pg4Ff-pikyznM0WPMQppJseuUAagP7MZzlpQ2cINt0Ijpk5Jhl7lAnRu2o61vS3bHfRoHzHMbvre-0&cce=2&category=acrcp_v1_32&sig=AOD64_0cxeIquiBEj1h6Ea4xq4VQLBKEaQ&ctype=46&q=&nis=4&ved=2ahUKEwiKhb_eye6XAxUDkeEIHTVcDiQQ5bgDKAB6BAgQEAs&adurl= |
| 2-slot 18650 holder (series wiring) | 1 | 0.32 | Holds the two cells in the printed battery bay | https://stemvolt.in/product/2-slot-18650-lithium-battery-holder/ |
| 2-bay 18650 charger | 1 | 4.8 | Charges the cells outside the car | https://www.amazon.in/Rechargeable-Lithium-Batteries-Capacity-Charging/dp/B0HPKNXB1Y |
| Adjustable buck converter module (3A rated) | 2 | 0.40 | Two 5V rails, one for ESP32 plus camera board and one for servos and headlights | https://www.rytronics.in/product/mini-mp1584-dc-dc-3a-adjustable-buck-module/?srsltid=AYCX9vJEhVAjZ58iGwrHbki_UnDtnGxTfhcneGCYtUVdeGeDcl4am618-Z4 |
| Latching power switch (rated 3A or more, about 12 mm panel hole) | 1 | 1.4 | Onboard power on/off through the deck | https://robu.in/product/12mm-12v-ring-light-self-lock-non-momentary-metal-push-button-switch-blue-light/?gad_source=1&gad_campaignid=17416544847&gbraid=0AAAAADvLFWcBh1I6vZzgsLbtonbjD3TX3&gclid=CjwKCAiAzKvXBhBAEiwABLl9AhA5B3C4UCNYKTSqtbDJOvNn3BWp6wBBKRxu_Hh32sA7mjY4Lt5V-RoC_WIQAvD_BwE |
| MPU6050 module (GY-521) | 1 | 2.80 | Gyro for straight line hold | https://zbotic.in/product/gy-521-mpu-6050-module-without-soldering/?gad_source=1&gad_campaignid=20903026778&gbraid=0AAAAACTNRdVdvwsIV39lj4y57I0SBYzCo&gclid=CjwKCAiAzKvXBhBAEiwABLl9AqMPCg_ccgpq5dWEMzxp2NKgd1fQgWrqEJtQT3Uhpen6nAFZAjCUuxoCffgQAvD_BwE |
| VL53L0X distance sensor module | 1 | 3 | Front obstacle auto-stop | https://www.amazon.in/Accurate-Distance-Measurement-Proximity-Applications/dp/B0FBLG1TZ1/ref=asc_df_B0FBLG1TZ1?mcid=cced8a685d5b398fb86f891fd0b626de&tag=googleshopdes-21&linkCode=df0&hvadid=709856082360&hvpos=&hvnetw=g&hvrand=5950710866178753422&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9062076&hvtargid=pla-2630629779032&psc=1&hvocijid=5950710866178753422-B0FBLG1TZ1-&hvexpln=0&gad_source=1 |
| Micro servo SG90 | 2 | 2.31 | Pan and tilt for the camera head | https://www.amazon.in/SG90-Micro-Servo-Motor-Robotics/dp/B0G257K9YW/ref=asc_df_B0G257K9YW?mcid=5c9f28f56ca73aad933176d02d447384&tag=googleshopdes-21&linkCode=df0&hvadid=709965974765&hvpos=&hvnetw=g&hvrand=13839117369846092622&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9062076&hvtargid=pla-2468124395932&psc=1&hvocijid=13839117369846092622-B0G257K9YW-&hvexpln=0&gad_source=1 |
| Pan tilt bracket for micro servos | 1 | 1.80 | Mount for the camera head | https://www.google.com/aclk?sa=L&ai=DChsSEwjo6KO53u6XAxXkmWYCHeUsBgAYACICCAEQBhoCc20&co=1&ase=2&gclid=CjwKCAiAzKvXBhBAEiwABLl9AgFWPxcE2qAQ25bDNk-GvQ4z09Lz2lk7apsB6Z3cvYeeB99170kNGBoCvfMQAvD_BwE&cid=CAAS3gHkaLbjd6D6ggluluXpT3DMkqb2Nqtxp1q48I6JiNXzJyUH7-ur3n-1X37_1qmr-mVIBWU0rIUrG7h2r5y3kdbHJWUDFMVGdn1bF9iLKq1flbI1iT7leCBnZEZnuT9RSZsRCpGLuOTX6GJfv8NGSoOiNkoqY5A3hvA-PmAZ6kvZDJEAfKQ9e_QePC7SRG10C4V4sQRC1YRuVWZnrVcH4mtCZ2WWRrNiTQ69VfVM6J4oNij350pENlw_aTCHcKpWDJrcguQ_jToX1cd2ueK9tJXpVljusco48R18_Bgf5vs&cce=2&category=acrcp_v1_33&sig=AOD64_3O3sL2A3PxUwGeaxBvwmtOm1t6Jg&ctype=5&q=&nis=4&ved=2ahUKEwiupZ253u6XAxVrxjgGHVTAIr8Q5bgDKAB6BAgQECY&adurl= |
| White 5 mm LEDs (pack) | 1 | 1.1 | Headlights for dark rooms | https://www.amazon.in/Assorted-White-Yellow-Electronics-Projects/dp/B0HLXKH6R3/ref=sr_1_103?dib=eyJ2IjoiMSJ9.tzYXptWWLXFsIzuvLiWHPXiVHOvRwWaJhuPYsYsOgZj8JpXmqi1Z57zQZ6675TOQA_bXb2Dw2o9JR8xI7Ifv3UZ5GpECfq70ahqdR_Ny7tvwQ3nFFvFJb6ef69F9OF3aLQwyHpx4VWYmVWax0suDCRcjyS8Lx3mwBv55RUmQBka1khjqy1FkC2R2pz2ZnaUDpzcUF1L5aiIvFe6BAeaNrFUHK5elGrLRsOXCD_N618X4w3vRNz9P7yel2y5uNqbAGsmw5O0ia0fVUF7zqv_43BrkjRV4oH0AOfC2kcydoR4.ByfUdS8BA4QKyFURsinj0yPp4PvVZAEywFjyh19yroE&dib_tag=se&keywords=5mm+white+led+pack&qid=1793809931&sr=8-103&xpid=aSFIQEWVRP5QJ |
| NPN transistor pack (2N2222 or similar) | 1 | 1.26 | Switches the headlights from an ESP32 pin | https://www.google.com/aclk?sa=L&ai=DChsSEwiy1ZPw3u6XAxXHwzwCHSoVPI8YACICCAEQBRoCc2Y&co=1&ase=2&gclid=CjwKCAiAzKvXBhBAEiwABLl9AoN8Rz4i8xqP26xyzK9FXuqglff-MdcBiM167t77WlkyAl-cnz9KPRoCzaYQAvD_BwE&cid=CAAS9gHkaIw2LK9GOUngRhxa0CfP1XRzFBNh-siBRt85tSkYZAyvXfGGJ7Tp_VOGPBSt5tAQ7L2ton1CpSoOGfgJSqlAcTauSTVW5cJRnn0QZSQJ4prK_zf_GUyGDJhrTBLGcnNTWCa9u-cdFs3cxCS553DQelK6PTwhTvR1IB8IM8Yz5rw942KDbwkXtb8fWZyvkFGXI9GQayNvEgy9rKKwLCQlUAHX9pAk6Bw-_Ebg5KcTRAKPD5TEPISg-5x6KSzmyDgbNHks9aT0A2q38vg_jZRw91A6HPGQ1MZkVnd2TQZJgDfZSLoO2-AF1uoOyFaVL5MXwthywY8&cce=2&category=acrcp_v1_33&sig=AOD64_17hJWSxWD9BqnZwPsKbkPvPGCVHw&ctype=5&q=&nis=4&ved=2ahUKEwjx6o3w3u6XAxXHm-EIHVI1EC8Q5bgDKAB6BAgREAs&adurl= |
| Capacitor assortment (1000uF electrolytic and 0.1uF ceramic) | 1 | 10.7 | Smooths the camera and servo power rails and the motor leads | https://www.theengineerstore.in/products/orange-500pcs-0-1uf-1000uf-plug-in-electrolytic-capacitor-kit?variant=43847259390115&country=IN&currency=INR&utm_medium=product_sync&utm_source=google&utm_content=sag_organic&utm_campaign=sag_organic&srsltid=AYCX9vKL0X4mumu8UVfT3fhOJu26tOcMCUp7ZIxZAjOq3GbMNHNfcg59jrE |
| Resistor assortment (needs 100k, 47k, 10k, 1k and 100 ohm) | 1 | 3 | Battery voltage divider, LED current limiting, transistor base resistor| https://www.google.com/aclk?sa=L&ai=DChsSEwik3_mh3-6XAxWuxDwCHQkANiYYACICCAEQBxoCc2Y&co=1&ase=2&gclid=CjwKCAiAzKvXBhBAEiwABLl9AgDesRAuKGzuu0auXbHS9ZD-8CFlzk-hIH49PyRqObrIaBCdWTHdFhoCGX8QAvD_BwE&cid=CAAS9gHkaCMCghMJrRRR91JNYrHt9mIeNOrrK1U3Lj0ybZe8DSk0r5z5RfELYz1u8i-204EooevJdWMTX1fo8R7a0Ib_PYrg28NKxvt1b6C_AZwGgb7QE7ZXRJT-y-puonYyk0EWXjZ-G9wug_L22jTFPyw6ilb_UzuoZZ95qskmMjtGcLk4y-hIJ8oQIgnSMcRJXsN_57SqrpvRyb-pXfti131jm8RdKYBsMHofR3gwms7yT1OXT2wy3PLTYcYOSTtpr4pp8LdPfoYiZ6EamRwFvLixl7qoxEOjzadyn1Yy2HejdDNRXpm3eWJPoonkHxyw46Kft8rAKfs&cce=2&category=acrcp_v1_33&sig=AOD64_3B4fQsyiAK0pCSim-z4TuwyKzX1w&ctype=5&q=&nis=4&ved=2ahUKEwjps-2h3-6XAxULkeEIHVILIl0Q5bgDKAB6BAgNEAs&adurl= |
| M3 x 16 mm screws (pack) | 1 | 2.66 | Clamp caps  | https://www.amazon.in/Rpi-shop-tensile-Machine-Quantity/dp/B07XY5YXKD |
| M3 x 10 mm screws (pack) | 1 | 1.05 | Deck to corner posts | https://www.amazon.in/Rpi-shop-Phillip-Phillips-Quantity/dp/B08FG7DL3T/ref=asc_df_B08FG7DL3T?mcid=0893e9a12f383fd7a993acb5de791f40&tag=googleshopdes-21&linkCode=df0&hvadid=709963085735&hvpos=&hvnetw=g&hvrand=6953664151164244133&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9062076&hvtargid=pla-1445440184293&psc=1&hvocijid=6953664151164244133-B08FG7DL3T-&hvexpln=0&gad_source=1 |
| Double-sided foam tape | 1 | 1.5 | Mount the buck modules and the IMU in their pockets | https://www.google.com/aclk?sa=L&ai=DChsSEwjOw8ns3-6XAxVUMIMDHQ8hMN4YACICCAEQCRoCc2Y&co=1&ase=2&gclid=CjwKCAiAzKvXBhBAEiwABLl9AhRmFbLkdgFxiCUZ68GLnkYWhVxSMvU1QAKGzPXx9WDZpZc-cFxbSRoCTG4QAvD_BwE&cid=CAAS9gHkaIz7TAi8Wmo-dfJusLc08uGIS2_6Fs29dGAT6oa4rnC6iZ9oEI5YvMUvJwkujMQtyZqqeCuDrdfTZa6L9iO9Tb4T9eZ7jY32EJftwmZAl8W8u-ysY5cpbUdR5xSSnh2jDIP5qjZNiHyIQelbnmSoK83z9t5D6GgT3ySfSCPXs6RR3-tL29pRFWVvr48yEEz-nRR6UyQp3HIX12eNz8Ew3JBoUp1W1eBFtvRQJ9S0J2Q7dnm2hi45NVLsoRm_uLov62v6aj9atjzOw7RGVSPZAkUPfgT62O1XE-ungMpRDH2kz5mCQNoDZQ7XM3o4KPXcProsbh0&cce=2&category=acrcp_v1_33&sig=AOD64_2MT5CGEQ7mj6885yBC-oIMiuFjMQ&ctype=5&q=&nis=4&ved=2ahUKEwjT-cPs3-6XAxVbjeEIHRHeBsMQ5bgDKAB6BAgIEAs&adurl= |
| PETG filament (1 kg spool) | 1 | 12.46 | Frame (1), deck (1), clamp caps (8), camera cradle (1) | https://www.google.com/aclk?sa=L&ai=DChsSEwi_mIH23-6XAxVRNIMDHTRoJwsYACICCAEQBRoCc2Y&co=1&ase=2&gclid=CjwKCAiAzKvXBhBAEiwABLl9Amj1NblHCk3wugJ85gpuvbnTmOhbhVklxxS0XOCL7GCWhbxhg_ge-BoC8AEQAvD_BwE&cid=CAAS0wHkaHHoSKWDcwqxYcBB_qhD2Vv9-rDkG4ys7epxwiYQ5-tJkTIPXv2xjh1zMSLOWGdVgrqYMs463TUc0qDc3TiLnH79Pi9WT-eRYiy-gRBkg2CeSi709SuikHX4GjWSKzxZgpbKA0VDWA-2LxObPu7ki6fAVGodcPC4Do7au8LaqsN7hsXyn_-9Q5jfDTgRy7_uYiRcUuzrI37IB08x1v9hjNOs6Ju51JWHTljN2zgoocF7RsRdIkNBRONSDaxFkC1mMlWHKv91GqB84WmIlBzuMJmJ&cce=2&category=acrcp_v1_32&sig=AOD64_1z6nnQwFvf4x65u-JkjDDZ6B2f5A&ctype=5&q=&nis=4&ved=2ahUKEwjr9Pr13-6XAxXlj-EIHZLAFE8Q5bgDKAB6BAgPEBs&adurl= |
| Silicone wire 22AWG | 1 | 1.4 | Power and motor wiring | https://www.google.com/aclk?sa=L&ai=DChsSEwiwqLiK4O6XAxWJkWYCHe_pMpgYACICCAEQBRoCc20&co=1&ase=2&gclid=CjwKCAiAzKvXBhBAEiwABLl9At8Tw7PvmsHIGCGgqZQkf5r388iuFeZq2VFM93ZYX1zU0J-cAzcKeRoCEhsQAvD_BwE&cid=CAAS9gHkaAw9plgfIv-CRFOykt3vyunUJWCcKfRQ7lmRYfYP-i72B7-emEXAkUgt-NFznZmBfgkumy14WiLzsXq_fs3IBx1jpu08SOy-BzPI5khXGVuJ3tqQdzNT25Mo8Z1OoDdU2nzvfaSxGlog8Ga9EkWcL3PVBU81jGCdzP0Y39HUdFXxvrBW9qdKllUWRu1ntNYPJiKSU5F9wHCbZC8JKyNL_IM9ueAyfX1hnWm9-DWhvmX5YUeHiiCc6fzb3qR1EAyV-I07kBnsZu3rxqnnDcFivL6ObDcbte7gu-bNk7rym8GkYom5ShTG_QXXwV4p4cgTGBoW9So&cce=2&category=acrcp_v1_33&sig=AOD64_1ik2rm30qjkvqWqQ4zn6_VMKLCEw&ctype=5&q=&nis=4&ved=2ahUKEwiW3LGK4O6XAxWKluEIHV8CJEYQ5bgDKAB6BAgLEBU&adurl= |
| Jumper wire set (male-female and male-male) | 1 | 1.6 | Prototype wiring | https://www.google.com/aclk?sa=L&ai=DChsSEwiAh_KW4O6XAxXWyTwCHTu5FAsYACICCAEQFRoCc2Y&co=1&ase=2&gclid=CjwKCAiAzKvXBhBAEiwABLl9Ai20pCxD4ob7YAgvOgZ6nt-L9G6iynd7TJHzANOkJvi49WV_EpHnrxoCho4QAvD_BwE&cid=CAAS9gHkaFuRBIUdtXOJbum96gGFlhWtHw4DEfRLlRqB1QwCn_v-V_d6yokoINCAslMuvYsPFDSKbu3tp_MiVjcbcAphJb4GFnUtFNwcRDjJlUWKiifOTqbHV4g9m8d21fkOE21T1XL4H83V0Q7HfYow_6NYgR8IemRaDjKa_J8ukhf_PM39F_3riifZfkjwh7Nn6JCRFyjr-UlqTofE94TDl00b_u4eE3My21gZvY6n_6wqYdviUUFaLFR2FCuYDfMCMZ2S23r3LK3Mh_dbAPKEJA_Dsr7Yj5EQ1RuAlRmUt9SVbfH45fpoPXEp1wU5fqraKJGAGPoaHGE&cce=2&category=acrcp_v1_33&sig=AOD64_2sqLMVTnaF5I0NLhxPOx67clnKgw&adurl=&ctype=5&q= |
| Soldering iron and solder | 1 | 4 | Motor leads, power wiring | https://www.google.com/aclk?sa=L&ai=DChsSEwjQ6Mqm4O6XAxVNp2YCHaEVL5kYACICCAEQCBoCc20&co=1&ase=2&gclid=CjwKCAiAzKvXBhBAEiwABLl9AgDzltcP79Z8imCTn2-beDkB1r3YUVVHmZWwqQObys8DsnpM14lOlxoCQisQAvD_BwE&cid=CAAS9gHkaJpcBxiYrkQpgJNR1QWCKKb3Mnc9mnrzU56HBpvOgQWtCav9XUpg51I6N1ci33qhdYVUH5e1JPaufibMkGpos3U9QPJ7W242qme0ZSkSzVgCMTXVurpG-I9Q1dMM_l2QNMxEs_3vpGTu8Eo6wHl-oMN7SJRI5Wm3BIKA7i1K_LQ7xHcuDnlyKApLHSOmGH3j31pQL0r1p6x4EYeKd7bynnbASFlCgJYBghl_ud0ALbF4R-mEluP1dZ1aQsR7kZt64i0lMa6zCizVxxrzlmCgxFKZ7RejbNwOcn4sApS21_7fGjBAq-yOxukbfY9yqJyv1_G-cIk&cce=2&category=acrcp_v1_33&sig=AOD64_0LlBOZxl4ZEda9wHxM2QnGt_9rhw&ctype=5&q=&nis=4&ved=2ahUKEwiOrsWm4O6XAxV0jeEIHahyNNwQ5bgDKAB6BAgREBs&adurl= |
| Digital calipers | 1 | 3.16 | Measure every part before finalising the CAD parameters | https://www.google.com/aclk?sa=L&ai=DChsSEwj9lua34O6XAxWHhGYCHfSmAXwYACICCAEQBhoCc20&co=1&ase=2&gclid=CjwKCAiAzKvXBhBAEiwABLl9AgtDGtVyIIE_l4757AcOnEoSDVEAT_r8xe1yZ741E-gZTm3weIyOyxoCCkoQAvD_BwE&cid=CAAS9gHkaOVo9T7ESNgVbTyfCaR4bLe3ssn73l7aqnyFEgV5Yycb72jvMzQTwoG5HSSYGw0Ws7Z6Lu4M-xRIOSH6CrmyU_et6xvFt95rbOppBew7kAKPe8U7jGH4Yppi8XZrLEQ-BTaXi2sDmGKhmCzoM1_sxpqO0Pkcw9eQ5KOzE3Tu-cTuBYNieudhE_ebWkBASso0rOGsYEpq_r-JW5QwV8RfagciAXqMmuibAxvPl7pIpqdxGflEUENUI6JlUH1T7v-rzQGFqa_TFpl_e_qXaqbgz2AbWz3V1xjsB-tENENBuzoqWnh2FsJTpk09lZH9o4IJA_ripX4&cce=2&category=acrcp_v1_33&sig=AOD64_3eVlJer-Nc-9GGRu7v0np5aUpUKQ&ctype=5&q=&nis=4&ved=2ahUKEwiKp-C34O6XAxXnjOEIHel_N9cQ5bgDKAB6BAgMEBs&adurl= |

<b>Estimated total: $114.05</b>

# Circuit Diagram

<img width="1258" height="696" alt="pt2" src="https://github.com/user-attachments/assets/87692c61-c700-4812-a9bf-0d4f2630bbf5" />

# Pin Map
Board: ESP32 DevKit (ESP32-WROOM-32).

## Signal pins

| Component | Component Pin | MCU Pin |
|---|---|---|
| TB6612FNG | STBY | GPIO4 |
| TB6612FNG | PWMA | GPIO25 |
| TB6612FNG | AIN1 | GPIO26 |
| TB6612FNG | AIN2 | GPIO27 |
| TB6612FNG | PWMB | GPIO33 |
| TB6612FNG | BIN1 | GPIO32 |
| TB6612FNG | BIN2 | GPIO13 |
| TB6612FNG | VCC | 3V3 |
| TB6612FNG | GND | GND |
| Battery voltage divider | Midpoint | GPIO34 |
| MPU6050 | VCC | 3V3 |
| MPU6050 | GND | GND |
| MPU6050 | SDA | GPIO21 |
| MPU6050 | SCL | GPIO22 |
| MPU6050 | AD0 | GND |
| VL53L0X | VCC | 3V3 |
| VL53L0X | GND | GND |
| VL53L0X | SDA | GPIO21 |
| VL53L0X | SCL | GPIO22 |
| Pan servo | Signal | GPIO18 |
| Tilt servo | Signal | GPIO19 |
| NPN transistor (headlights) | Base (through 1k resistor) | GPIO23 |
| XIAO ESP32-S3 Sense | none | none |

## I2C addresses

| Device | Address | Note |
|---|---|---|
| MPU6050 | 0x68 | 0x69 if AD0 is tied high |
| VL53L0X | 0x29 | Default address |

## Power and load connections 

| From | To |
|---|---|
| Cell 1 negative | Cell 2 positive (series junction) |
| Pack positive (Cell 1 positive) | Power switch L1 |
| Power switch COM | Net VBAT |
| Pack negative (Cell 2 negative) | GND |
| VBAT | TB6612 VM, Buck A IN+, Buck B IN+ |
| Buck A OUT+ (set to 5.0V) | ESP32 5V (VIN) pin, XIAO 5V pin, 1000uF capacitor (C1) to GND |
| Buck B OUT+ (set to 5.0V) | Both servo V+ pins, both headlight resistors, 1000uF capacitor (C2) to GND |
| All module grounds | One shared GND |
| TB6612 A01 and A02 | Left motor M1 |
| TB6612 B01 and B02 | Right motor M2 |
| 0.1uF ceramic (C3, C4) | Across each motor's terminals |
| STBY | 10k resistor to GND |
| VBAT | 100k resistor, then midpoint (to GPIO34), then 47k resistor to GND |
| Midpoint | 0.1uF capacitor (C5) to GND |
| Both servo GND pins | GND |
| Buck B 5V | 100 ohm resistor, then white LED anode (two separate branches) |
| Both LED cathodes | NPN collector |
| NPN emitter | GND |
| NPN base | 1k resistor to GPIO23, plus 10k resistor from base to GND |

## Before you wire

- Set both bucks to 5.0V with a multimeter before connecting the ESP32, XIAO or servos.
- With the switch off, the buck inputs and TB6612 VM must read 0V.
- Do not plug USB into the ESP32 or XIAO while a buck feeds their 5V pins.

## Assembly guide

Read all of it once before you start. The order matters, because most of the expensive mistakes happen when a step is done early.

### What you need on the bench

Everything in the Bill of Materials, plus a multimeter, you need it to set the buck converters and to check the battery reading. A small Phillips screwdriver, side cutters, and wire strippers help as well.

### Before you touch anything

1. Measure every part with calipers (motor body, shaft length, wheel hub, boards) and compare the numbers with the Fusion parameters. The printed parts only fit if those numbers are right.
2. Print one clamp station (a block and a cap) in PLA and try it on a real motor before printing the frame in PETG.
3. Flash both boards on the bench, before they go in the car. See Step 1.

### Step 1: flash the boards on the bench

1. Open `crawler_controller.ino` and `crawler_camera.ino`. Change the WiFi password in the settings block at the top of both files so the two match. WPA2 needs at least 8 characters.
2. Install the libraries: WebSockets by Markus Sattler, and Adafruit VL53L0X (the Adafruit BusIO library comes along with it).
3. Flash the ESP32 DevKit with the board set to an ESP32 Dev Module.
4. Flash the XIAO ESP32-S3 Sense with the board set to the XIAO ESP32S3 and PSRAM enabled. Check the exact PSRAM setting name in your Arduino core version, since it changes.
5. Power each board from USB only, one at a time. Open the Serial Monitor at 115200 and confirm that the ESP32 starts its network and the XIAO joins it and prints its address (192.168.4.50).

Do not connect USB while the board is also powered from the buck converter. Flash on the bench, then install.

### Step 2: print the parts

| Part | Quantity | Orientation |
|---|---|---|
| Frame | 1 | Floor on the bed |
| Deck | 1 | Flat |
| Clamp cap | 8 | Upside down, screw side on the bed |
| Camera cradle | 1 | Open top up |

### Step 3: prepare the motors

1. Solder about 15 cm of silicone wire to each motor's two terminals.
2. Pair the motors by side. The two left motors go in parallel on one TB6612 channel (A01 and A02), and the two right motors go on the other (B01 and B02). Parallel means the red lead of each motor joins the same wire, and the black leads join the other.
3. Solder one 0.1 uF ceramic capacitor across each channel's two output wires, close to the motors.
4. Test each pair for a second with 5 V or the bench supply before they go in the frame. If a pair turns the wrong way, you can swap its wires now.

### Step 4: fit the motors into the clamps

1. Slide each motor into its two grooves with the shaft going through the hole in the side wall.
2. Place a cap over each clamp and start both M3x16 screws by hand.
3. Tighten the two screws a little at a time, alternating, until the motor does not turn when you push the shaft. Stop there. Printed plastic cracks if you keep going.
4. Check that the cap has not touched the block. There should still be a visible gap of about 0.4 mm.

### Step 5: fit the wheels

1. Line up the flat of the shaft with the flat in the wheel hub and push the wheel on until the hub is fully seated.
2. Spin each wheel by hand. If it scrapes the frame, there is not enough gap. Pull it back a millimeter only if the hub still grips the shaft by more than half its length.

### Step 6: battery bay and power switch

1. Press the two-slot 18650 holder into the battery bay. The bay is a loose fit, so a small piece of foam tape keeps it still.
2. Fit the latching switch into the hole in the deck near the rear.
3. Put the cells in the holder with the right polarity. Check it twice against the markings. Do not close the switch yet.

### Step 7: set the buck converters before anything else touches them

1. Wire the battery holder to the switch and the switch to the two buck converters only. Leave everything else disconnected.
2. Switch on and adjust each buck output to 5.0 V with the multimeter.
3. Switch off and write "A" and "B" on the two modules so you do not swap them later.

This is the most important electrical step. A buck converter that comes out of the box set to 12 V will destroy an ESP32.

### Step 8: mount the boards on the deck

1. Press the ESP32 and the TB6612 into their pockets and hold them with a small zip tie through the slots.
2. Stick the two buck converters and the IMU into their pockets with double-sided foam tape.
3. Check that the IMU lies flat, with its chip facing up. The heading hold in the code assumes that.

### Step 9: wire it up

Use the Pin Mapping section for the full table. The order below keeps each stage easy to test.

1. Power. VBAT to TB6612 VM and to both buck inputs. Buck A output to the ESP32 5V (VIN) pin and the XIAO 5V pin. Buck B output to both servo power wires and the two headlight resistors. Every ground goes to the shared ground.
2. Logic power. ESP32 3V3 to TB6612 VCC.
3. Motor driver. GPIO4 to STBY (with the 10k resistor from STBY to ground), GPIO25 to PWMA, GPIO26 to AIN1, GPIO27 to AIN2, GPIO33 to PWMB, GPIO32 to BIN1, GPIO13 to BIN2.
4. Battery reading. VBAT through 100k to a node, from the node through 47k to ground. The node goes to GPIO34 with a 0.1 uF capacitor to ground.
5. Sensors. GPIO21 to the SDA pin of both the IMU and the distance sensor, GPIO22 to SCL on both. Connect the IMU's AD0 pin to ground. The IMU answers at 0x68 and the distance sensor at 0x29.
6. Servos. GPIO18 to the pan signal and GPIO19 to the tilt signal.
7. Headlights. Each LED has a 100 ohm resistor from Buck B to its anode. Both cathodes go to the 2N2222 collector. The emitter goes to ground. GPIO23 goes through a 1k resistor to the base, and a 10k resistor runs from the base to ground.

Route the motor wires away from the I2C wires where you can. Motors add noise to a bus.

### Step 10: front wall and camera head

1. Press the distance sensor board into its holder behind the window in the front wall, chip facing the opening.
2. Press the two LEDs into their holes.
3. Fit the pan and tilt bracket. Center both servos before you press the horns on: power them once so they move to the middle position, then attach the horns so the camera looks straight ahead.
4. Seat the XIAO in the camera cradle with the lens in the window, and hold it with a zip tie. Fix the cradle to the bracket with double-sided tape or small screws.
5. Leave a loop of slack in the camera's power wires so the pan motion does not pull on them.

The holes for the bracket in the deck should be cut to match the bracket you actually received. Measure the bracket before you print the final deck.

### Step 11: first power-up

1. Lift the car so the wheels are in the air. Close the switch.
2. Join the phone to the WiFi network called Crawler and open 192.168.4.1 in the browser.
3. Check in this order: the page loads, the battery voltage shows roughly the right number, the distance reading changes when you put a hand in front of the sensor, the camera video appears, and the headlights switch on.
4. Push the joystick forward. Both sides should turn forward. If one side spins backward, switch off and swap the two wires of that pair.
5. Let the connection drop (walk away, or switch the phone's WiFi off). The wheels should stop within a fraction of a second. If they do not, do not drive the car until you find out why.
6. Close the deck on the posts with four M3x10 screws.

### Step 12: first drive

Drive slowly on a clear floor first. Try turning in place, then a straight line, then drive toward a wall and let the auto brake slow the car down. Tune the settings block in the code (speeds, brake distances) to taste.

## Calibration

Battery reading. Measure the pack voltage with the multimeter and compare it with the number on the page. If they differ by more than a few hundredths of a volt, adjust the calibration value in the settings block.

Gyro direction. The heading hold assumes the IMU is flat with its Z axis up. If the car veers more when assist is on than when it is off, change the gyro direction setting from -1 to 1.

Camera orientation. If the picture is upside down or mirrored, change the flip and mirror settings at the top of `crawler_camera.ino`.

## Troubleshooting

### The Crawler network does not appear
The ESP32 is probably not powered. Check the 5 V at the ESP32's pin with the multimeter. Then check the Serial Monitor for the network start message. If you changed the password, make sure it is at least 8 characters.

### The phone joins the network but the page will not load
Android sometimes drops a network that has no internet and falls back to mobile data. Turn mobile data off, or accept the prompt that asks to stay connected. Make sure the address is typed as http://192.168.4.1, since the browser may try https.

### The page loads but there is no video
The XIAO is the usual cause. Check that it is powered, that it prints its address in the Serial Monitor, and that it joined the network (power the ESP32 first, since the XIAO needs the network to exist). Try the stream address directly in the phone browser: http://192.168.4.50:81/stream. If the camera fails to start, confirm PSRAM is enabled and check the camera pin numbers in the sketch against Seeed's current pinout, because I wrote them from memory.

### The video is slow or stutters
The stream is small on purpose (QVGA). Try moving closer to the car, turning the JPEG quality number up a little (a higher number means lower quality in this library), and keeping other phones off the network.

### Tilting the phone does nothing
This is expected on many Android phones when the page is served over plain HTTP. The page waits about 2 seconds and falls back to a centered camera, and the screen should say tilt is off. It is a limit of the browser, not a fault in the car.

### The motors do not move
Work down this list. The battery voltage must be above 6.0 V (the code stops driving below that). STBY must be high after setup (GPIO4). The TB6612 needs power on VM and on VCC (3.3 V). The failsafe stops the motors if commands stop arriving for 300 ms, so a phone that went to sleep also looks like dead motors.

### The motors are weak, or the ESP32 restarts when driving
This usually means the battery voltage dips under load. Check the cells are charged and the battery holder contacts are firm. Keep the motor and battery wires short and thick. If a restart persists, a larger capacitor across the battery supply near the motor driver is a likely fix, but I have not tested that on this build.

### The motor driver gets hot
Each TB6612 channel is rated for about 1.2 A continuously, and two motors on one channel can ask for more than that if they stall. Do not push the car against a wall at full throttle. If the chip gets too hot to touch, switch off and let it cool.

### The car spins instead of driving straight
One side is reversed. Swap the two wires of the pair on that side.

### The car veers to one side
The motors differ a little, which is normal. Turn the heading hold on. If the veer gets worse with it on, flip the gyro direction setting. Also check that the wheels are seated fully on their shafts.

### A wheel rubs the frame
Check the gap between the wall and the wheel. A thin washer between the hub and the wall can fix a small error. If the shaft is too short for the hub to seat, the gap in the CAD needs to shrink, and the frame has to be reprinted.

### A motor slips in its clamp
Tighten the two cap screws a little more, alternating between them. If the motor still turns, the groove is too big for your motor, and you need to reduce the clamp gap or the groove diameter in the CAD.

### A cap or block cracked
You tightened too far. Print a new cap, and stop tightening as soon as the motor will not turn.

### The distance sensor never brakes, or brakes too early
Check the Serial Monitor for the sensor's address (0x29). If it is missing, look at the SDA and SCL wires. If it reads but the numbers look odd, check that the chip lines up with the window and that the board is not tilted. A sensor looking through a deep opening can see the edge of the hole.

### An I2C device is not found
The IMU must answer at 0x68 with AD0 on ground, and the distance sensor at 0x29. Swapped SDA and SCL wires, a missing ground, or a missing 3.3 V are the common causes. Run an I2C scanner sketch to see what answers.

### The servos jitter
Servos pull power in short bursts. Make sure they run from Buck B and not from the ESP32 pins, share a ground with the ESP32, and that the 1000 uF capacitor sits across Buck B's output. If the camera head still twitches at rest, check for a loose signal wire.

### The headlights do not light
Check the LED polarity. Then check the transistor pin order against its datasheet, since 2N2222 variants can be wired differently. A base resistor that is open, or a missing connection from GPIO23, also keeps them off.

### The battery reading is wrong
Check the two resistor values (100k and 47k) and the connection to GPIO34. Then follow the battery calibration step above.

### I cannot upload to the ESP32
Unplug the battery pack first, since USB and the buck converter should not power the board at the same time. If the upload still times out, hold the BOOT button while the upload starts. Avoid wiring anything to GPIO0, 2, 12 or 15, which affect how the chip boots.

### Something smells hot or the supply browns out
Switch off at once and remove the cells. Look for a short between a power wire and ground, a reversed wire on a module, or a buck converter that was not set to 5.0 V.

### Safety
Use protected 18650 cells and charge them outside the car. Take them out if the car will sit unused for a long time.


# CAD Models
1] Made the dummies <br>
<img width="1512" height="982" alt="Screenshot 2026-11-03 at 8 20 17 PM" src="https://github.com/user-attachments/assets/57a50330-8a00-44ef-aba2-77896e0c15f8" /> <br>
2] Made the tub base with battery holder provision <br>
<img width="1512" height="982" alt="Screenshot 2026-11-03 at 9 42 24 PM" src="https://github.com/user-attachments/assets/8dcf6fe1-de5a-4077-b69a-516bf771b001" /> <br>
3] Added lower clamps <br>
<img width="1206" height="739" alt="Screenshot 2026-11-04 at 6 58 14 AM" src="https://github.com/user-attachments/assets/22739006-50b4-4359-9924-8ba2b3a07623" /> <br>
4] Added upper clamps <br>
<img width="1512" height="982" alt="Screenshot 2026-11-04 at 7 37 53 AM" src="https://github.com/user-attachments/assets/e57c985b-1bef-443b-824f-1582b234d9ac" />
<img width="1512" height="982" alt="Screenshot 2026-11-04 at 7 37 56 AM" src="https://github.com/user-attachments/assets/ed35da55-530f-4268-bed4-15c43fb80b0f" /> <br>
5] Added Deck with slots for components <br>
<img width="1512" height="982" alt="Screenshot 2026-11-04 at 8 01 16 AM" src="https://github.com/user-attachments/assets/5a61b23b-3d79-46fd-bf60-8e8afca14fb5" /> <br>
6] Made Camera Cradle <br>
<img width="1512" height="982" alt="Screenshot 2026-11-04 at 10 39 46 AM" src="https://github.com/user-attachments/assets/02390ddc-ec1d-440d-be24-2d3b0570bcc1" /> <br>
7] Complete with all the dummies in place <br>
<img width="1512" height="982" alt="Screenshot 2026-11-04 at 4 05 58 PM" src="https://github.com/user-attachments/assets/cb3de45d-981b-47c4-a9b1-67463f9273e2" />









