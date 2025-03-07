#include <Arduino.h>
#ifdef ESP32
#include <AsyncTCP.h>
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(TARGET_RP2040) || defined(TARGET_RP2350) || defined(PICO_RP2040) || defined(PICO_RP2350)
#include <RPAsyncTCP.h>
#include <WiFi.h>
#endif
#include <F1sh.h>
#include <motor.h>

// Remove comment from this line to use ESP Smart Config
#define USE_SMART_CONFIG

const char *ssid = "F1sh";
const char *password = "stemistclub";

const char *hostname = "f1sh.local";

const int channel = 0; // 1-13 - You should change this if there are multiple APs in the area.

void handleMotors(int c1, int c2, int c3, int c4) {
  // Your motor control code here
  Serial.printf("Motor callback: %d, %d, %d, %d\n", c1, c2, c3, c4);
}

void handleServos(int s1, int s2, int s3, int s4, int s5, int s6) {
  // Your servo control code here
  Serial.printf("Servo callback: %d, %d, %d, %d, %d, %d\n", s1, s2, s3, s4, s5, s6);
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