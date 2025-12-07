# Demo: ESP8266 pub_sub example

This repository is a small demo that shows a minimal ESP8266/NodeMCU sketch using an FS-based web server and WebSocket example. It's not a full smart-home project — just a demonstration sketch.
The exact used controller `Wemos NodeMCU V3` which has many different names out there, here is some links to the board on [GeeksCorner](https://geekscorner.shantaweb.com/) if you want to buy it: <https://geekscorner.shantaweb.com/1604196541-nodemcu-v3-lua-wifi-module-integration-of>

wemos front                |  wemos back
:-------------------------:|:-------------------------:
![wemos front](https://cdn1.botland.store/67475-pdt_540/wifi-module-esp8266-wemos-nodemcu-v3-32mb-11-gpio-adc-pwm.jpg)  |  ![wemos back](https://cdn2.botland.store/67477-pdt_540/wifi-module-esp8266-wemos-nodemcu-v3-32mb-11-gpio-adc-pwm.jpg)

## Check video

[![Watch YouTube Video](https://img.youtube.com/vi/tuk2a8agZ80/0.jpg)](https://www.youtube.com/watch?v=tuk2a8agZ80)

## Contents

- src/pub_sub.ino — demo sketch (Web server + WebSocket)  
  Open: [src/pub_sub.ino](src/pub_sub.ino)
- platformio.ini — PlatformIO build configuration  
  Open: [platformio.ini](platformio.ini)
- config/gen_compile_commands.py — optional helper script to generate compile_commands.json  
  Open: [config/gen_compile_commands.py](config/gen_compile_commands.py)

Build and run (PlatformIO)

1. Install PlatformIO extension in VS Code.
2. Connect your NodeMCU device.
3. Select the environment `nodemcuv2` and run Build / Upload from PlatformIO.

Notes

- This repo is intended as a demo only. Inspect `src/pub_sub.ino` to see the example logic.
- Adjust WiFi and other settings in the sketch before flashing your device.

## Contact GeeksCorner

| [Facebook](https://www.facebook.com/profile.php?id=61580976832543)             |  [Whatsapp](https://wa.me/201100045221) |
:-------------------------:|:-------------------------:
![facebook_qr](https://github.com/AnasKhedr/waveshare_pico_rp2040_zero/blob/master/imgs/fb.png?raw=true) | ![whatsapp_qt](https://github.com/AnasKhedr/waveshare_pico_rp2040_zero/blob/master/imgs/whatsapp.png?raw=true)
