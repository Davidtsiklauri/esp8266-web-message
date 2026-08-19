#include "weather_service.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

WeatherData WeatherService::fetch_weather(float lat, float lon)
{
    WeatherData data;

    if (WiFi.status() != WL_CONNECTED)
    {
        return data;
    }

    WiFiClient client;
    HTTPClient http;

    String url = "http://api.open-meteo.com/v1/forecast?latitude=" + String(lat, 4) +
                 "&longitude=" + String(lon, 4) +
                 "&current_weather=true&hourly=temperature_2m&forecast_days=2";

    http.begin(client, url);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK)
    {
        String payload = http.getString();

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (!error)
        {
            data.temp = doc["current_weather"]["temperature"];
            int code = doc["current_weather"]["weathercode"];

            if (code == 0)
                data.condition = "Sunny";
            else if (code >= 1 && code <= 3)
                data.condition = "Partly Cld";
            else if (code >= 45 && code <= 48)
                data.condition = "Foggy";
            else if (code >= 51 && code <= 67)
                data.condition = "Rain";
            else if (code >= 71 && code <= 77)
                data.condition = "Snow";
            else if (code >= 80 && code <= 82)
                data.condition = "Showers";
            else if (code >= 95)
                data.condition = "Thunder";
            else
                data.condition = "Cloudy";

            time_t now = time(nullptr);
            struct tm *timeinfo = localtime(&now);
            int current_hour = (timeinfo && timeinfo->tm_year > 70) ? timeinfo->tm_hour : 12;

            int next_block = ((current_hour / 6) + 1) * 6;

            int idx_1 = next_block;
            int idx_2 = next_block + 6;
            int idx_3 = next_block + 12;

            data.forecast_hour_1 = idx_1 % 24;
            data.temp_forecast_1 = doc["hourly"]["temperature_2m"][idx_1];

            data.forecast_hour_2 = idx_2 % 24;
            data.temp_forecast_2 = doc["hourly"]["temperature_2m"][idx_2];

            data.forecast_hour_3 = idx_3 % 24;
            data.temp_forecast_3 = doc["hourly"]["temperature_2m"][idx_3];

            data.valid = true;
        }
    }

    http.end();
    return data;
}