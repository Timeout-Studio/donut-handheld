# Do-NUT Handheld Device
This is a sub-project for 28th Graduation Project of Yuan Ze University Department of Information Communication by Timeout Studio

## Installation
### Required Plugins
- Platformio

### Required Libraries
- bblanchon/ArduinoJson@^7.3.1
- bodmer/TFT_eSPI@^2.5.4
- links2004/WebSockets@^2.6.1
- lvgl/lvgl@8.3.11

### Steps
1. Clone repo
2. Delete `.pio/libdeps/YOUR_BOARD/TFT_eSPI/User_Setup.h` (We put it in include folder and use build_flags)
3. Move `./include/lv_conf.h` to `.pio/libdeps/YOUR_BOARD/lvgl`

## Tested Hardware
- Adafruit ESP32-S3 Feather with STEMMA QT / Qwiic - 8MB Flash No PSRAM
- GC9A01 1.28" 240x240 Rounded Screen
- EC11 Rotary Encoder
- 1mW 780nm Infrared Laser Pointer
- GY-271 HMC5883L Magnetometer Module

## License
- Do-NUT Handheld Device as a whole is licensed under the GNU General Public License, Version 2. Individual files may have a different, but compatible license.