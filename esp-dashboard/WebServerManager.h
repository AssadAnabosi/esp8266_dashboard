#pragma once
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"
#include <ArduinoJson.h>

extern WiFiManager wifiManager;
class WebServerManager
{
public:
    WebServerManager();
    void begin();
    void handleClient();
    void handleHealth();
    void handleStatus();
    void handleScan();
    void handleConnect();
    void handleDisconnect();
    void handleGetSettings();
    void handlePostSettings();
    void handleReboot();
    void handleReset();
    void sendJson(const DynamicJsonDocument &doc);

private:
    ESP8266WebServer server;
};