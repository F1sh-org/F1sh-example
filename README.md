# F1sh - Control a robot using a web interface.

[![CC BY-NC 4.0][cc-by-nc-shield]][cc-by-nc]

## Introduction
This is the quickstart example project for F1sh, a library for controlling a robot using a web interface, packed in a single header file.

The library is designed to work with F1sh's motor driver board which containes 4 motors and 6 servos powered by an Adafruit's PCA9685 and processed using ESP32 microcontrollers. ESP8266 are not tested and supported. However, we expects it should work properly with ESP8266 with minimal tweaks

## Credit

This project were made possible using multiple open-source and community-driven library:

[ESPAsyncWebServer](https://github.com/ESP32Async/ESPAsyncWebServer.git): LGPL-3.0 license

[AsyncTCP](https://github.com/ESP32Async/AsyncTCP.git): LGPL-3.0 license

[Adafruit PWM Servo Driver Library](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library.git): BSD License

[ArduinoJson](https://github.com/bblanchon/ArduinoJson.git): MIT license

Also a small part of this quickstart project contains code from [MakerBot](https://github.com/makerviet/maker-bot.git) under MIT license, for more information checkout `motor.h` in `include` directory.

## License
This work is licensed under a
[Creative Commons Attribution-NonCommercial 4.0 International License][cc-by-nc].

[![CC BY-NC 4.0][cc-by-nc-image]][cc-by-nc]

[cc-by-nc]: https://creativecommons.org/licenses/by-nc/4.0/
[cc-by-nc-image]: https://licensebuttons.net/l/by-nc/4.0/88x31.png
[cc-by-nc-shield]: https://img.shields.io/badge/License-CC%20BY--NC%204.0-lightgrey.svg
