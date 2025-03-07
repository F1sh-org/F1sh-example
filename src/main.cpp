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

// Remove comment from this line to use ESP Smart Config
// #define USE_SMART_CONFIG

#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "AsyncJson.h"
#include <ArduinoJson.h>
#include <motor.h>

const char *ssid = "F1sh";
const char *password = "stemistclub";

const char *hostname = "f1sh.local";

const int channel = 0; // 1-13 - You should change this if there are multiple APs in the area.

static AsyncWebServer server(80);
static AsyncWebSocket ws("/ws");

void initWiFiAP() {
#ifdef USE_SMART_CONFIG
  WiFi.mode(WIFI_AP_STA);
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
#endif
#ifndef USE_SMART_CONFIG
  Serial.print("\n\nCreating hotspot");
  WiFi.setHostname(hostname);
  WiFi.encryptionType(WIFI_AUTH_WPA2_PSK);
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid,password,(channel >= 1) && (channel <= 13) ? channel : int(random(1, 13)));
#endif
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
      Serial.printf("index: %" PRIu64 ", len: %" PRIu64 ", final: %" PRIu8 ", opcode: %" PRIu8 "\n", info->index, info->len, info->final, info->opcode);
      String msg = "";
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
              const char* c1 = doc["c1"];
              const char* c2 = doc["c2"];
              const char* c3 = doc["c3"];
              const char* c4 = doc["c4"];
              Serial.printf("c1: %s  c2: %s  c3: %s  c4: %s\n",c1,c2,c3,c4);
              setPWMMotors(int(c1),int(c2),int(c3),int(c4));
            }
            if (doc["action"] == "servo")
            {
              const char* s1 = doc["s1"];
              const char* s2 = doc["s2"];
              const char* s3 = doc["s3"];
              const char* s4 = doc["s4"];
              const char* s5 = doc["s5"];
              const char* s6 = doc["s6"];
              Serial.printf("s1: %s  s2: %s  s3: %s  s4: %s  s5: %s  s6: %s\n",s1,s2,s3,s4,s5,s6);
            }
            if (doc["action"] == "reboot") {
              ESP.restart();
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

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  initMotors();
#ifdef ESP32
  LittleFS.begin(true);
#else
  LittleFS.begin();
#endif
  initWiFiAP();
  initWebServer();
}

void loop() {
  ws.cleanupClients();
}