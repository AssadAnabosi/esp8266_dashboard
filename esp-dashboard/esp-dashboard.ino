#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include "WebServerManager.h"
#include "ConfigStorage.h"

#include "WiFiManager.h"
WiFiManager wifiManager;

WebServerManager webServer;

void setup()
{
  Serial.begin(115200);
  // Flush serial buffer
  Serial.println("");
  LittleFS.begin();
  ConfigStorage::begin();
  // Initialize WiFiManager and attempt to connect to WiFi
  if (wifiManager.autoConnect())
  {
    Serial.println("STA IP address: ");
    Serial.println(WiFi.localIP());
  }
  webServer.begin();
}

void loop()
{
  webServer.handleClient();
}
