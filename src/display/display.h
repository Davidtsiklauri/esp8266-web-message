#ifndef DISPLAY_BUILDER_H
#define DISPLAY_BUILDER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Define I2C pins according to your board specification
#define OLED_SDA 12 // GPIO12 (D6)
#define OLED_SCL 14 // GPIO14 (D5)

class DisplayBuilder
{
public:
    DisplayBuilder();
    void setup_display();
    void update_display(String msg);

private:
    Adafruit_SSD1306 display;
    void calculate_text_size(String msg);
};

#endif // DISPLAY_BUILDER_H