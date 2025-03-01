#include <Arduino.h>
#ifdef ESP32
#include <AsyncTCP.h>
#include <WiFi.h>
#include <DNSServer.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(TARGET_RP2040) || defined(TARGET_RP2350) || defined(PICO_RP2040) || defined(PICO_RP2350)
#include <RPAsyncTCP.h>
#include <WiFi.h>
#endif

#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

// configure softAP and DNS.
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;

const char *ssid = "F1sh";
const char *password = "stemistclub";

const char *hostname = "f1sh.local";

const int channel = 0; // 1-13 - You should change this if there are multiple APs in the area.

static AsyncWebServer server(80);
static AsyncWebSocket ws("/ws");

static uint32_t lastWS = 0;
static uint32_t deltaWS = 100;

static uint32_t lastHeap = 0;

void initWiFiAP() {
  WiFi.setHostname(hostname);
  WiFi.encryptionType(WIFI_AUTH_WPA2_PSK);
  WiFi.begin(ssid, password);
  Serial.print("\n\nCreating hotspot");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid,password,(channel >= 1) && (channel <= 13) ? channel : int(random(1, 13)));
  dnsServer.start(DNS_PORT, "*", apIP);
  Serial.println("\n\nWiFi parameters:");
  Serial.print("Mode: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? "Station" : "Client");
  Serial.print("IP address: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP());
}

void initStatic() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->redirect("/index.html");
  });
  server.rewrite("*", "/index.html");
  server.serveStatic("/index.html", LittleFS, "/index.html");
  server.addHandler(&ws);
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
        }
      }
    }
  });
  server.begin();
}

void wsCleanup() {
  uint32_t now = millis();

  if (now - lastWS >= deltaWS) {
    ws.printfAll("kp%.4f", (10.0 / 3.0));
    lastWS = millis();
  }

  if (now - lastHeap >= 2000) {
    // cleanup disconnected clients or too many clients
    ws.cleanupClients();

#ifdef ESP32
    Serial.printf("Free heap: %" PRIu32 "\n", ESP.getFreeHeap());
#endif
    lastHeap = now;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
#ifdef ESP32
  LittleFS.begin(true);
#else
  LittleFS.begin();
#endif
  initWiFiAP();
}

void loop() {
  dnsServer.processNextRequest();
  wsCleanup();
}