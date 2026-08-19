#ifndef WIFI_ADAPTER_H
#define WIFI_ADAPTER_H

#include <ESP8266WiFi.h>

class WifiAdapter
{
private:
    WiFiServer &server;

public:
    // Only DECLARE the constructor here (no body, no initializer list)
    WifiAdapter(WiFiServer &serverRef);

    // Only DECLARE the method here
    void setup_wifi_connection();

    void setup_server();
};

#endif // WIFI_ADAPTER_H