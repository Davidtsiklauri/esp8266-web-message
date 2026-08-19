#ifndef WEATHER_SERVICE_H
#define WEATHER_SERVICE_H

#include <Arduino.h>

struct WeatherData
{
    float temp = 0.0;
    String condition = "N/A";

    // 4 forecast slots covering 24 hours (6h steps)
    int forecast_hour_1 = 0;
    float temp_forecast_1 = 0.0;

    int forecast_hour_2 = 0;
    float temp_forecast_2 = 0.0;

    int forecast_hour_3 = 0;
    float temp_forecast_3 = 0.0;

    int forecast_hour_4 = 0;
    float temp_forecast_4 = 0.0;

    bool valid = false;
};

class WeatherService
{
public:
    static WeatherData fetch_weather(float lat = 41.6941, float lon = 44.8337);
};

#endif