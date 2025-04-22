#pragma once
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"
#include <ArduinoJson.h>

extern WiFiManager wifiManager;

/**
 * @brief Manages the web server functionality for device control and configuration.
 */
class WebServerManager
{
public:
    /**
     * @brief Constructs a WebServerManager instance.
     */
    WebServerManager();

    /**
     * @brief Initializes the web server and sets up the endpoints. Including the following:
     * - All Static files (HTML, CSS, JS) are served from SPIFFS.
     * - /api/health: Check device health (to see if the device is alive/connected)
     * - /api/status: Get device status (including current connections and MACs and IPs)
     * - /api/scan: Scan for available WiFi networks
     * - /api/connect: Connect to a WiFi network
     * - /api/disconnect: Disconnect from WiFi
     * - /api/settings: Get or update device settings
     * - /api/reboot: Reboot the device
     * - /api/reset: Reset the device to factory settings
     */
    void begin();

    /**
     * @brief Handles incoming client requests. Should be called frequently in the main loop.
     */
    void handleClient();

    /**
     * @brief Handles a request to check device health.
     */
    void handleHealth();

    /**
     * @brief Handles a request to fetch the device's current connections status, mac addresses ips.
     */
    void handleStatus();

    /**
     * @brief Handles a request to scan for available WiFi networks.
     */
    void handleScan();

    /**
     * @brief Handles a request to connect the device to a WiFi network.
     */
    void handleConnect();

    /**
     * @brief Handles a request to disconnect the device from WiFi.
     */
    void handleDisconnect();

    /**
     * @brief Handles a request to retrieve current device settings.
     */
    void handleGetSettings();

    /**
     * @brief Handles a request to update device settings.
     */
    void handlePostSettings();

    /**
     * @brief Handles a request to reboot the device.
     */
    void handleReboot();

    /**
     * @brief Handles a request to reset the device to factory settings.
     */
    void handleReset();

    /**
     * @brief Sends a JSON document as an HTTP response.
     * @param doc The JSON document to send.
     */
    void sendJson(const DynamicJsonDocument &doc);

private:
    ESP8266WebServer server;
};
