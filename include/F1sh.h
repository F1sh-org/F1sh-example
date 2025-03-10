#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <WiFi.h>
#include <LittleFS.h>
#include "AsyncJson.h"
#include <ArduinoJson.h>

/*
  This is the header file for F1sh, a library for controlling a robot using a web interface.
  The library is designed to work with the ESP32 and ESP8266 microcontrollers, and its all packed in a single header file.
  Released under the Creative Commons Attribution-NonCommercial 4.0 International License.
  Created by B4iter (@b4iterdev).
  Stemist Club logo and all associated assets are property of Stemist Club. All rights reserved.
  For more infomation on F1sh, please visit: https://github.com/stemistclub/F1sh.git
*/


typedef void (*MotorCallback)(int, int, int, int);
typedef void (*ServoCallback)(int, int, int, int, int, int);


static AsyncWebServer server(80);
static AsyncWebSocket ws("/ws");

static MotorCallback motorCallback = nullptr;
static ServoCallback servoCallback = nullptr;

void initWiFiAP(const char *ssid,const char *password,const char *hostname, int channel) {
    WiFi.setHostname(hostname);
    WiFi.encryptionType(WIFI_AUTH_WPA2_PSK);
    WiFi.begin(ssid, password);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid,password,(channel >= 1) && (channel <= 13) ? channel : int(random(1, 13)));
    Serial.print("IP address: ");
    Serial.println(WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP());
}

void initWiFiSmart() {
    WiFi.mode(WIFI_STA);
    WiFi.beginSmartConfig();
    //Wait for SmartConfig packet from mobile
    Serial.println("Waiting for SmartConfig.");
    while (!WiFi.smartConfigDone()) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("SmartConfig received.");
    //Wait for WiFi to connect to AP
    Serial.println("Waiting for WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi Connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP());
}

void initWebServer() {
    ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
      (void)len;
  
      if (type == WS_EVT_CONNECT) {
        ws.textAll("new client connected");
        Serial.println("ws connect");
        client->setCloseClientOnQueueFull(false);
        client->ping();
  
      } else if (type == WS_EVT_DISCONNECT) {
        ws.textAll("client disconnected");
        Serial.println("ws disconnect");
  
      } else if (type == WS_EVT_ERROR) {
        Serial.println("ws error");
  
      } else if (type == WS_EVT_PONG) {
        Serial.println("ws pong");
  
      } else if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        // Serial.printf("index: %" PRIu64 ", len: %" PRIu64 ", final: %" PRIu8 ", opcode: %" PRIu8 "\n", info->index, info->len, info->final, info->opcode);
        // String msg = "";
        if (info->final && info->index == 0 && info->len == len) {
          if (info->opcode == WS_TEXT) {
            data[len] = 0;
            Serial.printf("ws text: %s\n", (char *)data);
  
            // Parse the JSON message
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, data);
            if (error) {
              Serial.print(F("deserializeJson() failed: "));
              Serial.println(error.f_str());
              return;
            }
  
            // Extract data
            if (!doc["action"].isNull()) {
              const char* action = doc["action"];
              Serial.println(action);
              if (doc["action"] == "motor")
              {
                const int c1 = doc["c1"].as<int>();
                const int c2 = doc["c2"].as<int>();
                const int c3 = doc["c3"].as<int>();
                const int c4 = doc["c4"].as<int>();
                Serial.printf("c1: %d  c2: %d  c3: %d  c4: %d\n", c1, c2, c3, c4);
                if (motorCallback) {
                  motorCallback(c1, c2, c3, c4);
                }
              }
              if (doc["action"] == "servo")
              {
                const int s1 = doc["s1"].as<int>();;
                const int s2 = doc["s2"].as<int>();;
                const int s3 = doc["s3"].as<int>();;
                const int s4 = doc["s4"].as<int>();;
                const int s5 = doc["s5"].as<int>();;
                const int s6 = doc["s6"].as<int>();;
                Serial.printf("s1: %d  s2: %d  s3: %d  s4: %d  s5: %d  s6: %d\n",s1,s2,s3,s4,s5,s6);
                if (servoCallback) {
                  servoCallback(s1, s2, s3, s4, s5, s6);
                }
              }
              if (doc["action"] == "reboot") {
                ESP.restart();
              }
              if (doc["action"] == "get") {
                // send available data
                ws.text(client->id(), "{\"action\":\"get\",\"data\":\"ok\"}");
              }
            }
          }
        }
      }
    });
    server.rewrite("/config", "/index.html");
    server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
    server.addHandler(&ws);
    server.begin();
}

void setMotorCallback(MotorCallback callback) {
    motorCallback = callback;
}

void setServoCallback(ServoCallback callback) {
    servoCallback = callback;
}

void F1shInitAP(const char *ssid,const char *password,const char *hostname, int channel) {
  Serial.println("Starting F1sh as an Access Point");
#ifdef ESP32
  LittleFS.begin(true);
#else
  LittleFS.begin();
#endif
  initWiFiAP(ssid,password,hostname,channel);
  initWebServer();
}

void F1shInitSmartAP(){
  Serial.println("Starting F1sh in SmartConfig mode");
#ifdef ESP32
    LittleFS.begin(true);
#else
    LittleFS.begin();
#endif
    initWiFiSmart();
    initWebServer();
}

void F1shLoop() {
  ws.cleanupClients();
}
