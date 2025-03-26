#include <Arduino.h>
#include <F1sh.h>
#include <motor.h>
#include <gamepadController.h>

/*
  This is a quickstart example for F1sh, this is a good way to explore F1sh and its capabilities.
  This example is assumes that you are using F1sh's motor driver board which containes 4 motors and 6 servos powered by an Adafruit's PCA9685 controller.
  Released under the Creative Commons Attribution-NonCommercial 4.0 International License.
  For more infomation on F1sh, please visit: https://github.com/F1sh-org/F1sh.git
*/



// F1sh expects a certificate and a key file to be present in the root of the LittleFS filesystem.
// Create a cert by yourself by running the following command:
// openssl req -x509 -newkey rsa:4096 -nodes -keyout server.key -out server.crt -sha256 -days 36500 -subj "/C=VN/ST=HN/O=F1sh-org/OU=F1sh/CN=F1sh.local" -addext "subjectAltName = IP:192.168.4.1"
// After that put both server.crt and server.key in the root of the LittleFS filesystem by putting them under /data folder from this project
// Then under PlatformIO, go to the Platform and select Upload Filesystem Image to upload the files to the ESP32.


// If you use Smart Config, you don't need to provide those parameters
const char *ssid = "F1sh";
const char *password = "stemistclub";

const char *hostname = "f1sh.local";

const int channel = 0; // 1-13 - You should change this if there are multiple APs in the area.

F1sh f1sh;

void handleGamepad() {
  int X = (int)f1sh.mapFloat(f1sh.gamepad[0].axis[0], -1.0f, 1.0f, 0.0f, 255.0f);
  int Y = (int)f1sh.mapFloat(f1sh.gamepad[0].axis[1], -1.0f, 1.0f, 0.0f, 255.0f);
  gamepadControl(X, Y, &f1sh.gamepad[0].button);
}

void setup() {
  Serial.begin(115200);
  initMotors();
  f1sh.F1shInitAP(ssid, password, hostname, channel);
  #ifdef CORE_DEBUG_LEVEL
    Serial.setDebugOutput(true);
  #endif
  // Register callbacks
  f1sh.setGamepadCallback(handleGamepad);
}

void loop() {

}