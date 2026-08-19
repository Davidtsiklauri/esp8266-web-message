#ifndef DISPLAY_BUILDER_H
#define DISPLAY_BUILDER_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 12
#define OLED_SCL 14

class DisplayBuilder
{
public:
    DisplayBuilder();
    void setup_display();
    void update_display(String msg);
    void show_idle_screen();

    // Displays Current Weather + Next 3 forecast blocks (Total: 4 items)
    void show_weather_screen(float current_temp, String condition,
                             int hour1, float temp1,
                             int hour2, float temp2,
                             int hour3, float temp3);

private:
    Adafruit_SSD1306 display;
    void calculate_text_size(String msg);
    void draw_happy_face();
    void draw_weather_icon();
};

#endif