#ifndef WiFiManager_h
#define WiFiManager_h

#include <Arduino.h>
#include <ESP8266WiFi.h>

class WiFiManager {
public:
    bool autoConnect();
    void startAccessPoint();
    bool connectToNetwork(const char* ssid, const char* password);

private:
    void setupSoftAP();
};

#endif