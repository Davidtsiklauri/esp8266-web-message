#ifndef WEB_H
#define WEB_H

#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

// Define display states
enum DisplayMode
{
    MODE_MESSAGE,
    MODE_IDLE,
    MODE_WEATHER
};

class Web
{
public:
    Web();
    void setup_routes();

    AsyncWebServer server;
    String currentMessage = "Ready!";
    DisplayMode currentMode = MODE_MESSAGE;
    bool needsDisplayUpdate = true;
};

#endif