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


// Remove comment from this line to use ESP Smart Config
// #define USE_SMART_CONFIG


// If you use Smart Config, you don't need to provide those parameters
const char *ssid = "F1sh";
const char *password = "stemistclub";

const char *hostname = "f1sh.local";

const int channel = 0; // 1-13 - You should change this if there are multiple APs in the area.

F1sh f1sh;

void handleGamepad(const float axes[][4], const float buttons[][17]){
  if (!axes || !buttons) {
    Serial.println("Invalid gamepad data received");
    return;
  }
  float localButtons[17];
  memcpy(localButtons, buttons[0], sizeof(float) * 17);
  float mappedX = map(axes[0][0], (float)-1, (float)1, 0, 255);
  float mappedY = map(axes[0][1], (float)-1, (float)1, 0, 255);
  gamepadControl(mappedX, mappedY, &localButtons);
}

void setup() {
  Serial.begin(115200);
  initMotors();
  #ifdef USE_SMART_CONFIG
  f1sh.F1shInitSmartAP();
  #else
  f1sh.F1shInitAP(ssid, password, hostname, channel);
  #endif
  #ifdef CORE_DEBUG_LEVEL
    Serial.setDebugOutput(true);
  #endif
  // Register callbacks
  f1sh.setGamepadCallback(handleGamepad);
}

void loop() {
  f1sh.F1shLoop();
}