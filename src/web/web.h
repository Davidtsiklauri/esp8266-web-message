#ifndef WEB_H
#define WEB_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

class Web
{
public:
    String currentMessage = "Hello World";
    bool idleScreenEnabled = true;
    bool needsDisplayUpdate = false;
    AsyncWebServer server;

    Web();
    void setup_routes();
};

#endif // WEB_H