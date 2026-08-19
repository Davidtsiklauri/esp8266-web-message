#include "display/display.h"

// Constructor initializes the member object directly
DisplayBuilder::DisplayBuilder()
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1)
{
}

void DisplayBuilder::setup_display()
{
    Wire.begin(OLED_SDA, OLED_SCL);
    Wire.setClock(100000);

    // SSD1306_SWITCHCAPVCC (value 0x02) replaces the magic number '2'
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println("Failed at 0x3C, trying 0x3D...");
        if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3D))
        {
            Serial.println("SSD1306 allocation failed at both addresses!");
            return;
        }
    }

    Serial.println("OLED successfully initialized!");
}

void DisplayBuilder::update_display(String msg)
{
    display.clearDisplay();
    display.setTextWrap(true);

    // 1. Draw Fixed Header
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Web Message:");
    display.println("---------------------");

    calculate_text_size(msg);

    display.print(msg);
    display.display();
}

void DisplayBuilder::calculate_text_size(String msg)
{
    // Header takes about 18 pixels vertically.
    // Remaining available area: Width = 128px, Height = 46px (64 - 18)
    const int max_width = 128;
    const int max_height = 46;
    const int start_y = 18;

    // 2. Find the largest text size that fits
    int best_size = 1;

    for (int size = 4; size >= 1; size--)
    {
        int16_t x1, y1;
        uint16_t w, h;

        display.setTextSize(size);
        display.getTextBounds(msg, 0, start_y, &x1, &y1, &w, &h);

        // If the calculated bounding box fits within the remaining screen height/width
        if (w <= max_width && h <= max_height)
        {
            best_size = size;
            break; // Stop at the largest working size
        }
    }

    display.setTextSize(best_size);
    display.setCursor(0, start_y);
}