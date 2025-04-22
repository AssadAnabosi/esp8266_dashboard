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
     * @brief Initializes the web server and sets up the endpoints.
     */
    void begin();

    /**
     * @brief Handles incoming client requests. Should be called frequently in the main loop.
     */
    void handleClient();

    /**
     * @brief  Sends back OK response indicating the server is running.
     */
    void handleHealth();

    /**
     * @brief JSON response of current connections status, and ips.
     */
    void handleStatus();

    /**
     * @brief Scan for available WiFi networks and returns the JSON response.
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
     * @brief JSON response of current device settings.
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
