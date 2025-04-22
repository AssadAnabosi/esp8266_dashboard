#ifndef ConfigStorage_h
#define ConfigStorage_h

#include <Arduino.h>

/**
 * @brief A utility class to manage persistent configuration storage, such as WiFi and Access Point settings.
 */
class ConfigStorage
{
public:
  /**
   * @brief Initializes the configuration storage system. if not already initialized.
   */
  static void begin();

  /**
   * @brief Saves WiFi credentials to persistent storage.
   * @param ssid The WiFi SSID.
   * @param password The WiFi password.
   */
  static void saveWiFiCredentials(const String &ssid, const String &password);

  /**
   * @brief Reads the stored WiFi SSID from persistent storage.
   * @return The stored WiFi SSID.
   */
  static String readWiFiSSID();

  /**
   * @brief Reads the stored WiFi password from persistent storage.
   * @return The stored WiFi password.
   */
  static String readWiFiPassword();

  /**
   * @brief Saves Access Point (AP) settings and hostname to persistent storage.
   * @param ssid The AP SSID.
   * @param password The AP password.
   * @param channel The AP WiFi channel.
   * @param hidden Whether the AP should be hidden.
   * @param status Whether the AP should be enabled.
   * @param hostname The hostname for the device.
   */
  static void saveSettings(const String &ssid, const String &password, int channel, bool hidden, bool status, const String &hostname);

  /**
   * @brief Reads the stored hostname from persistent storage.
   * @return The stored hostname.
   */
  static String readHostname();

  /**
   * @brief Reads the stored AP SSID from persistent storage.
   * @return The stored AP SSID.
   */
  static String readAPSSID();

  /**
   * @brief Reads the stored AP password from persistent storage.
   * @return The stored AP password.
   */
  static String readAPPassword();

  /**
   * @brief Reads the stored AP WiFi channel from persistent storage.
   * @return The stored AP channel.
   */
  static int readAPChannel();

  /**
   * @brief Reads the stored AP hidden status from persistent storage.
   * @return True if the AP is hidden, false otherwise.
   */
  static bool readAPHidden();

  /**
   * @brief Reads the stored AP status (enabled/disabled) from persistent storage.
   * @return True if the AP is enabled, false otherwise.
   */
  static bool readAPStatus();

  /**
   * @brief Performs a factory reset by clearing all stored configuration.
   */
  static void factoryReset();
};

#endif
