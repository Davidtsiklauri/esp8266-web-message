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

  if (!LittleFS.begin())
  {
    Serial.println("An error occurred while mounting LittleFS");
    return;
  }

  wifi_adapter.setup_wifi_connection();
  display_builder.setup_display();

  web.setup_routes();
}

void loop()
{
  yield();

  if (web.needsDisplayUpdate)
  {
    web.needsDisplayUpdate = false;

    switch (web.currentMode)
    {
    case MODE_IDLE:
      display_builder.show_idle_screen();
      break;

    case MODE_WEATHER:
      display_builder.show_weather_screen(24.0, "Sunny");
      break;

    case MODE_MESSAGE:
    default:
      display_builder.update_display(web.currentMessage);
      break;
    }
  }

  static unsigned long last_tick = 0;
  if (millis() - last_tick >= 10000)
  {
    last_tick = millis();

    if (web.currentMode == MODE_IDLE)
    {
      display_builder.show_idle_screen();
    }
    else if (web.currentMode == MODE_WEATHER)
    {
      display_builder.show_weather_screen(24.0, "Sunny");
    }
  }
}