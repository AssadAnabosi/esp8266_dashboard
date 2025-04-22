#include "WiFiManager.h"
#include "ConfigStorage.h"

#define AP_IP IPAddress(192, 168, 4, 1)

void WiFiManager::setupSoftAP()
{
  if (!ConfigStorage::readAPStatus())
  {
    return; // AP is disabled
  }

  String APSSID = ConfigStorage::readAPSSID();
  String APPassword = ConfigStorage::readAPPassword();
  int APChannel = ConfigStorage::readAPChannel();
  bool APHidden = ConfigStorage::readAPHidden();

  WiFi.softAPConfig(AP_IP, AP_IP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(APSSID.c_str(), APPassword.c_str(), APChannel, APHidden ? 1 : 0);
  Serial.println("AP IP address: ");
  Serial.println(WiFi.softAPIP());
}

bool WiFiManager::autoConnect()
{
  String ssid = ConfigStorage::readWiFiSSID();
  String password = ConfigStorage::readWiFiPassword();
  String hostname = ConfigStorage::readHostname();

  WiFi.mode(WIFI_AP_STA);

  setupSoftAP();

  if (ssid.length() == 0)
  {
    return false;
  }

  Serial.println("Attempting to Auto Connect to " + ssid + "...");
  WiFi.hostname(hostname);
  WiFi.begin(ssid.c_str(), password.c_str());

  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000)
  {
    delay(100);
  }
  return WiFi.status() == WL_CONNECTED;
}

bool WiFiManager::connectToNetwork(const char *ssid, const char *password)
{
  String hostname = ConfigStorage::readHostname();
  WiFi.disconnect();
  WiFi.mode(WIFI_AP_STA);

  setupSoftAP();

  WiFi.hostname(hostname);
  WiFi.begin(ssid, password);

  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000)
  {
    delay(100);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    ConfigStorage::saveWiFiCredentials(ssid, password);
    return true;
  }
  else
  {
    return false;
  }
}