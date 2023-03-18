#ifdef ESP32
#include <ESPmDNS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESP8266mDNS.h>
#endif

#include <LittleFS.h>
#include <ESPAsyncWebServer.h>

#include <secrets.h>

// SKETCH BEGIN
AsyncWebServer server(80);
const char * hostName = "pio-webserver";

void setup(){
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("STA: Failed!\n");
    WiFi.disconnect(false);
    delay(1000);
    WiFi.begin(ssid, password);
  }

  MDNS.begin(hostName);
  MDNS.addService("http","tcp",80);

  LittleFS.begin();

  server.serveStatic("/", LittleFS, "/www/").setDefaultFile("index.html");

  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(LittleFS, "/www/index.html");
  });

  server.begin();
}

void loop(){
#ifdef ESP8266
  // Required for ESP8266 but absent from ESP32 mDNS library
  MDNS.update();
#endif
}
