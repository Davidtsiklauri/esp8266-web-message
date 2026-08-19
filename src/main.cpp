#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LittleFS.h>
#include "web/web.h"
#include "display/display.h"
#include "wifi/wifi_connection.h"

DisplayBuilder display_builder;
Web web;
WifiAdapter wifi_adapter;

void setup()
{
  Serial.begin(9600);

  // 1. Mount LittleFS File System
  if (!LittleFS.begin())
  {
    Serial.println("An error occurred while mounting LittleFS");
    return;
  }

  // 2. Setup WiFi & Display
  wifi_adapter.setup_wifi_connection();
  display_builder.setup_display();

  // 3. Start Web Server
  web.setup_routes();
}

void loop()
{
  // Allow background WiFi/TCP async processing to execute safely
  yield();

  // Handle web-triggered display updates on the main thread (avoids I2C interrupt crashes)
  if (web.needsDisplayUpdate)
  {
    web.needsDisplayUpdate = false;

    if (web.idleScreenEnabled)
    {
      display_builder.show_idle_screen();
    }
    else
    {
      display_builder.update_display(web.currentMessage);
    }
  }

  // Periodic clock / idle screen refresh
  if (web.idleScreenEnabled)
  {
    static unsigned long last_tick = 0;
    if (millis() - last_tick >= 10000)
    {
      last_tick = millis();
      display_builder.show_idle_screen();
    }
  }
}