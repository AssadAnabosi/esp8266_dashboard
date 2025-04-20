#include "WebServerManager.h"
#include "WiFiManager.h"
#include "ConfigStorage.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

WebServerManager::WebServerManager()
  : server(80) {}

void WebServerManager::handleClient() {
  server.handleClient();
}

void WebServerManager::begin() {
  // Static Files
  server.serveStatic("/", LittleFS, "/web/index.html");
  server.serveStatic("/home", LittleFS, "/web/home.html");
  server.serveStatic("/scan", LittleFS, "/web/scan.html");
  server.serveStatic("/settings", LittleFS, "/web/settings.html");
  server.serveStatic("/style.css", LittleFS, "/web/style.css");
  server.serveStatic("/js/site.js", LittleFS, "/web/js/site.js");
  server.serveStatic("/js/home.js", LittleFS, "/web/js/home.js");
  server.serveStatic("/js/scan.js", LittleFS, "/web/js/scan.js");
  server.serveStatic("/js/settings.js", LittleFS, "/web/js/settings.js");

  // API Endpoints
  server.on("/api/health", HTTP_GET, std::bind(&WebServerManager::handleHealth, this));
  server.on("/api/status", HTTP_GET, std::bind(&WebServerManager::handleStatus, this));
  server.on("/api/scan", HTTP_GET, std::bind(&WebServerManager::handleScan, this));
  server.on("/api/connect", HTTP_POST, std::bind(&WebServerManager::handleConnect, this));
  server.on("/api/disconnect", HTTP_POST, std::bind(&WebServerManager::handleDisconnect, this));
  server.on("/api/settings", HTTP_GET, std::bind(&WebServerManager::handleGetSettings, this));
  server.on("/api/settings", HTTP_POST, std::bind(&WebServerManager::handlePostSettings, this));
  server.on("/api/reboot", HTTP_POST, std::bind(&WebServerManager::handleReboot, this));

  server.begin();
}

// Utility Functions

String getEncryptionType(int encType) {
  switch (encType) {
    case ENC_TYPE_NONE:
      return "OPEN";
    case ENC_TYPE_WEP:
      return "WEP";
    case ENC_TYPE_TKIP:
      return "WPA-PSK";
    case ENC_TYPE_CCMP:
      return "WPA2-PSK";
    case ENC_TYPE_AUTO:
      return "AUTO";
    default:
      return "UNKNOWN";
  }
}

void WebServerManager::sendJson(const DynamicJsonDocument &doc) {
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

// API Handlers

void WebServerManager::handleHealth() {
  server.send(200, "text/plain", "OK");
}

void WebServerManager::handleStatus() {
  DynamicJsonDocument doc(512);

  bool softAPEnabled = WiFi.getMode() & WIFI_AP;
  bool stationConnected = (WiFi.status() == WL_CONNECTED);

  doc["softAP"]["enabled"] = softAPEnabled;
  doc["softAP"]["ssid"] = WiFi.softAPSSID();
  doc["softAP"]["ip"] = softAPEnabled ? WiFi.softAPIP().toString() : "";

  doc["station"]["connected"] = stationConnected;
  doc["station"]["ssid"] = WiFi.SSID();
  doc["station"]["ip"] = stationConnected ? WiFi.localIP().toString() : "";

  sendJson(doc);
}

void WebServerManager::handleScan() {
  int n = WiFi.scanNetworks(false, true);
  bool isConnected = (WiFi.status() == WL_CONNECTED);
  String currentSSID = WiFi.SSID();

  std::vector<int> indices(n);
  for (int i = 0; i < n; ++i)
    indices[i] = i;

  std::sort(indices.begin(), indices.end(), [](int a, int b) {
    return WiFi.RSSI(a) > WiFi.RSSI(b);
  });

  DynamicJsonDocument doc(2048);
  JsonArray networks = doc.to<JsonArray>();

  for (int i = 0; i < n; ++i) {
    int idx = indices[i];
    JsonObject network = networks.createNestedObject();
    network["ssid"] = WiFi.SSID(idx);
    network["rssi"] = WiFi.RSSI(idx);
    network["enc"] = getEncryptionType(WiFi.encryptionType(idx));
    network["mac"] = WiFi.BSSIDstr(idx);
    network["channel"] = WiFi.channel(idx);
    network["connected"] = isConnected && (currentSSID == WiFi.SSID(idx));
  }

  sendJson(doc);
}

void WebServerManager::handleConnect() {
  if (!server.hasArg("plain")) {
    server.send(400, "text/plain", "Missing body");
    return;
  }

  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, server.arg("plain"));

  if (error) {
    server.send(400, "text/plain", "Invalid JSON");
    return;
  }

  if (!doc.containsKey("ssid")) {
    server.send(400, "text/plain", "Missing required field: ssid");
    return;
  }

  String ssid = doc["ssid"].as<String>();
  String password = doc.containsKey("password") ? doc["password"].as<String>() : "";

  if (wifiManager.connectToNetwork(ssid.c_str(), password.c_str())) {
    server.send(200, "text/plain", "Connected");
  } else {
    server.send(500, "text/plain", "Failed to connect to WiFi Network");
  }
}


void WebServerManager::handleDisconnect() {
  WiFi.disconnect();
  server.send(200, "text/plain", "Disconnected from WiFi Network");
}

void WebServerManager::handleGetSettings() {
  DynamicJsonDocument doc(512);

  doc["ap_ssid"] = ConfigStorage::readAPSSID();
  doc["ap_password"] = ConfigStorage::readAPPassword();
  doc["ap_channel"] = ConfigStorage::readAPChannel();
  doc["ap_hidden"] = ConfigStorage::readAPHidden();
  doc["ap_status"] = ConfigStorage::readAPStatus();

  sendJson(doc);
}

void WebServerManager::handlePostSettings() {
  Serial.println(server.arg("plain"));
  if (!server.hasArg("plain")) {
    server.send(400, "text/plain", "Missing body");
    return;
  }

  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, server.arg("plain"));

  if (error) {
    server.send(400, "text/plain", "Invalid JSON");
    return;
  }
  String missingFields;
  if (!doc.containsKey("ap_ssid"))
    missingFields += "ap_ssid, ";
  if (!doc.containsKey("ap_password"))
    missingFields += "ap_password, ";
  if (!doc.containsKey("ap_channel"))
    missingFields += "ap_channel, ";
  if (!doc.containsKey("ap_hidden"))
    missingFields += "ap_hidden, ";
  if (!doc.containsKey("ap_status"))
    missingFields += "ap_status, ";

  if (!missingFields.isEmpty()) {
    // Remove the trailing comma and space
    missingFields = missingFields.substring(0, missingFields.length() - 2);
    server.send(400, "text/plain", "Missing required fields: " + missingFields);
    return;
  }

  String APSSID = doc["ap_ssid"].as<String>();
  String APPassword = doc["ap_password"].as<String>();
  int APChannel = doc["ap_channel"].as<int>();
  bool APHidden = doc["ap_hidden"].as<bool>();
  bool APStatus = doc["ap_status"].as<bool>();

  Serial.println("------- New Settings -------");
  Serial.print("AP SSID: ");
  Serial.println(APSSID);
  Serial.print("AP Password: ");
  Serial.println(APPassword);
  Serial.print("AP Channel: ");
  Serial.println(APChannel);
  Serial.print("AP Hidden: ");
  Serial.println(APHidden ? "Yes" : "No");
  Serial.print("AP Status: ");
  Serial.println(APStatus ? "Enabled" : "Disabled");
  Serial.println("-----------------------------");

  ConfigStorage::saveAPSettings(APSSID, APPassword, APChannel, APHidden, APStatus);

  server.send(200, "text/plain", "Settings updated");
  delay(100);
  ESP.restart();
}

void WebServerManager::handleReset() {
  ConfigStorage::factoryReset();
  delay(100);
  ESP.restart();
}

void WebServerManager::handleReboot() {
  server.send(200, "text/plain", "Device is rebooting...");
  delay(100);
  ESP.restart();
}
