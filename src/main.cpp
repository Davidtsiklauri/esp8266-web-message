#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LittleFS.h>
#include "web/web.h"
#include "display/display.h"
#include "wifi/wifi_connection.h"
#include "weather/weather_service.h"

DisplayBuilder display_builder;
Web web;
WifiAdapter wifi_adapter;

WeatherData cached_weather;
unsigned long last_weather_fetch = 0;

const unsigned long WEATHER_FETCH_INTERVAL = 6 * 60 * 60 * 1000UL;

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

  cached_weather = WeatherService::fetch_weather();
  last_weather_fetch = millis();
}

void loop()
{
  yield();

  if (millis() - last_weather_fetch >= WEATHER_FETCH_INTERVAL)
  {
    last_weather_fetch = millis();
    cached_weather = WeatherService::fetch_weather();
  }

  if (web.needsDisplayUpdate)
  {
    web.needsDisplayUpdate = false;

    switch (web.currentMode)
    {
    case MODE_IDLE:
      display_builder.show_idle_screen();
      break;

    case MODE_WEATHER:
      display_builder.show_weather_screen(
          cached_weather.temp,
          cached_weather.condition,
          cached_weather.forecast_hour_1,
          cached_weather.temp_forecast_1,
          cached_weather.forecast_hour_2,
          cached_weather.temp_forecast_2,
          cached_weather.forecast_hour_3,
          cached_weather.temp_forecast_3);
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
      display_builder.show_weather_screen(
          cached_weather.temp,
          cached_weather.condition,
          cached_weather.forecast_hour_1,
          cached_weather.temp_forecast_1,
          cached_weather.forecast_hour_2,
          cached_weather.temp_forecast_2,
          cached_weather.forecast_hour_3,
          cached_weather.temp_forecast_3);
    }
  }
}