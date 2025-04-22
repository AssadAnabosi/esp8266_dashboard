#include "ConfigStorage.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

#define CONFIG_FILE "/config.json"

void ConfigStorage::begin()
{
  if (!LittleFS.exists(CONFIG_FILE))
  {
    File f = LittleFS.open(CONFIG_FILE, "w");
    if (f)
    {
      StaticJsonDocument<512> doc;
      doc["wifi_ssid"] = "";
      doc["wifi_password"] = "";
      doc["ap_ssid"] = "ESP-Access-Point";
      doc["ap_password"] = "12345678";
      doc["ap_channel"] = 6;
      doc["ap_hidden"] = false;
      doc["ap_status"] = true;
      doc["hostname"] = "ESP8266-IoT";
      serializeJson(doc, f);
      f.close();
    }
  }
  Serial.println("----- Current Settings -----");
  Serial.print("WiFi SSID: ");
  Serial.println(ConfigStorage::readWiFiSSID());
  Serial.print("WiFi Password: ");
  Serial.println(ConfigStorage::readWiFiPassword());
  Serial.print("Hostname: ");
  Serial.println(ConfigStorage::readHostname());

  Serial.print("AP SSID: ");
  Serial.println(ConfigStorage::readAPSSID());
  Serial.print("AP Password: ");
  Serial.println(ConfigStorage::readAPPassword());
  Serial.print("AP Channel: ");
  Serial.println(ConfigStorage::readAPChannel());
  Serial.print("AP Hidden: ");
  Serial.println(ConfigStorage::readAPHidden() ? "Yes" : "No");
  Serial.print("AP Status: ");
  Serial.println(ConfigStorage::readAPStatus() ? "Enabled" : "Disabled");
  Serial.println("-----------------------------");
}

void ConfigStorage::saveWiFiCredentials(const String &ssid, const String &password)
{
  File f = LittleFS.open(CONFIG_FILE, "r");
  if (!f)
    return;
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, f);
  f.close();
  if (error)
    return;

  doc["wifi_ssid"] = ssid;
  doc["wifi_password"] = password;

  f = LittleFS.open(CONFIG_FILE, "w");
  if (f)
  {
    serializeJson(doc, f);
    f.close();
  }
}

String ConfigStorage::readWiFiSSID()
{
  File f = LittleFS.open(CONFIG_FILE, "r");
  if (!f)
    return "";
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, f);
  f.close();
  if (error)
    return "";
  return doc["wifi_ssid"] | "";
}

String ConfigStorage::readWiFiPassword()
{
  File f = LittleFS.open(CONFIG_FILE, "r");
  if (!f)
    return "";
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, f);
  f.close();
  if (error)
    return "";
  return doc["wifi_password"] | "";
}

void ConfigStorage::saveSettings(const String &ssid, const String &password, int channel, bool hidden, bool status, const String &hostname)
{
  File f = LittleFS.open(CONFIG_FILE, "r");
  if (!f)
    return;
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, f);
  f.close();
  if (error)
    return;

  doc["ap_ssid"] = ssid;
  doc["ap_password"] = password;
  doc["ap_channel"] = channel;
  doc["ap_hidden"] = hidden;
  doc["ap_status"] = status;
  doc["hostname"] = hostname;

  f = LittleFS.open(CONFIG_FILE, "w");
  if (f)
  {
    serializeJson(doc, f);
    f.close();
  }
}

String ConfigStorage::readHostname()
{
  File f = LittleFS.open(CONFIG_FILE, "r");
  if (!f)
    return "ESP8266-IoT";
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, f);
  f.close();
  if (error)
    return "ESP8266-IoT";
  return doc["hostname"] | "ESP8266-IoT";
}

String ConfigStorage::readAPSSID()
{
  File f = LittleFS.open(CONFIG_FILE, "r");
  if (!f)
    return "";
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, f);
  f.close();
  if (error)
    return "";
  return doc["ap_ssid"] | "";
}

String ConfigStorage::readAPPassword()
{
  File f = LittleFS.open(CONFIG_FILE, "r");
  if (!f)
    return "";
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, f);
  f.close();
  if (error)
    return "";
  return doc["ap_password"] | "";
}

int ConfigStorage::readAPChannel()
{
  File f = LittleFS.open(CONFIG_FILE, "r");
  if (!f)
    return 6;
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, f);
  f.close();
  if (error)
    return 6;
  return doc["ap_channel"] | 6;
}

bool ConfigStorage::readAPHidden()
{
  File f = LittleFS.open(CONFIG_FILE, "r");
  if (!f)
    return false;
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, f);
  f.close();
  if (error)
    return false;
  return doc["ap_hidden"] | false;
}

bool ConfigStorage::readAPStatus()
{
  File f = LittleFS.open(CONFIG_FILE, "r");
  if (!f)
    return true;
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, f);
  f.close();
  if (error)
    return true;
  return doc["ap_status"] | true;
}

void ConfigStorage::factoryReset()
{
  if (LittleFS.exists(CONFIG_FILE))
  {
    LittleFS.remove(CONFIG_FILE);
  }
  begin(); // recreate file with defaults
}