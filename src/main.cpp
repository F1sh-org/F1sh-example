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

static AsyncWebServer server(80);

void initWiFi() {
  WiFi.setHostname(hostname);
  WiFi.encryptionType(WIFI_AUTH_WPA2_PSK);
  WiFi.begin(ssid, password);
  Serial.print("\n\nCreating hotspot");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid,password);
  dnsServer.start(DNS_PORT, "*", apIP);
  Serial.println("\n\nWiFi parameters:");
  Serial.print("Mode: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? "Station" : "Client");
  Serial.print("IP address: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP());
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starting...");
#ifdef ESP32
  LittleFS.begin(true);
#else
  LittleFS.begin();
#endif
  initWiFi();
}

void loop() {
  // put your main code here, to run repeatedly:
  dnsServer.processNextRequest();
}