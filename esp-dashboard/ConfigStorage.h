#ifndef ConfigStorage_h
#define ConfigStorage_h

#include <Arduino.h>

class ConfigStorage {
public:
  static void begin();
  
  static void saveWiFiCredentials(const String& ssid, const String& password);
  static String readWiFiSSID();
  static String readWiFiPassword();

  static void saveAPSettings(const String& ssid, const String& password, int channel, bool hidden, bool status);
  static String readAPSSID();
  static String readAPPassword();
  static int readAPChannel();
  static bool readAPHidden();
  static bool readAPStatus();
  static void factoryReset();
};

#endif
