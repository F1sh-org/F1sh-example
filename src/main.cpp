#include <Arduino.h>
#include <F1sh.h>
#include <motor.h>

/*
  This is a quickstart example for F1sh, this is a good way to explore F1sh and its capabilities.
  This example is assumes that you are using Stemist Club's motor driver board which containes 4 motors and 6 servos powered by an Adafruit's PCA9685 controller.
  Released under the Creative Commons Attribution-NonCommercial 4.0 International License.
  For more infomation on F1sh, please visit: https://github.com/stemistclub/F1sh.git
*/


// Remove comment from this line to use ESP Smart Config
#define USE_SMART_CONFIG


// If you use Smart Config, you don't need to provide those parameters
const char *ssid = "F1sh";
const char *password = "stemistclub";

const char *hostname = "f1sh.local";

const int channel = 0; // 1-13 - You should change this if there are multiple APs in the area.

void handleMotors(int c1, int c2, int c3, int c4) {
  // Your motor control code here
}

void handleServos(int s1, int s2, int s3, int s4, int s5, int s6) {
  // Your servo control code here
}

void setup() {
  Serial.begin(115200);
  initMotors();
  #ifdef USE_SMART_CONFIG
  F1shInitSmartAP();
  #else
  F1shInitAP(ssid, password, hostname, channel);
  #endif
  // Register callbacks
  setMotorCallback(handleMotors);
  setServoCallback(handleServos);
}

void loop() {
  F1shLoop();
}