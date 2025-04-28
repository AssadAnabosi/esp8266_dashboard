#ifndef WiFiManager_h
#define WiFiManager_h

#include <Arduino.h>
#include <ESP8266WiFi.h>

/**
 * @brief Manages WiFi connections and Access Point (AP) setup.
 */
class WiFiManager
{
public:
    /**
     * @brief Attempts to automatically connect to a previously known WiFi network.
     * @return True if the connection is successful, false otherwise.
     */
    bool autoConnect();

    /**
     * @brief Attempts to connect to a specified WiFi network.
     * @param ssid The SSID of the network.
     * @param password The password for the network.
     * @return True if the connection is successful, false otherwise.
     */
    bool connectToNetwork(const char *ssid, const char *password);

private:
    /**
     * @brief Sets up the device as a Soft Access Point (using saved settings, if enabled).
     */
    void setupSoftAP();
};

#endif
