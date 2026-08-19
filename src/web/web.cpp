#include "web.h"

Web::Web() : server(80) {}

void Web::setup_routes()
{
    if (!LittleFS.begin())
    {
        Serial.println("LittleFS mount failed!");
        return;
    }

    // 1. Serve index.html & inject %CURRENT_MSG% safely
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/index.html", "text/html", false, [this](const String &var) -> String
                              {
            if (var == "CURRENT_MSG")
            {
                return this->currentMessage;
            }
            return String(); }); });

    // 2. Serve static style.css directly from LittleFS
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/style.css", "text/css"); });

    // 3. Handle /setMessage route
    server.on("/setMessage", HTTP_GET, [this](AsyncWebServerRequest *request)
              {
        if (request->hasParam("msg"))
        {
            this->currentMessage = request->getParam("msg")->value();
            this->currentMode = MODE_MESSAGE;
            this->needsDisplayUpdate = true;
        }
        request->redirect("/"); });

    // 4. Handle /toggleIdle route
    server.on("/toggleIdle", HTTP_GET, [this](AsyncWebServerRequest *request)
              {
        // Toggle between Clock (IDLE) and Message mode
        if (this->currentMode == MODE_IDLE) {
            this->currentMode = MODE_MESSAGE;
        } else {
            this->currentMode = MODE_IDLE;
        }
        this->needsDisplayUpdate = true;
        request->redirect("/"); });

    // 5. Handle /toggleWeather route
    // 5. Handle /toggleWeather route
    server.on("/toggleWeather", HTTP_GET, [this](AsyncWebServerRequest *request)
              {
        // Toggle between Weather and Message mode
        if (this->currentMode == MODE_WEATHER) {
            this->currentMode = MODE_MESSAGE;
        } else {
            this->currentMode = MODE_WEATHER;
        } // <--- You were missing this closing brace
        
        this->needsDisplayUpdate = true;
        request->redirect("/"); });
    // Catch-all 404 handler
    server.onNotFound([](AsyncWebServerRequest *request)
                      { request->send(404, "text/plain", "Not found"); });

    server.begin();
    Serial.println("Async Web Server started!");
}