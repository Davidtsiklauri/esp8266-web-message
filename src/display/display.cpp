#include "display/display.h"

DisplayBuilder::DisplayBuilder()
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1)
{
}

void DisplayBuilder::setup_display()
{
    Wire.begin(OLED_SDA, OLED_SCL);
    Wire.setClock(100000);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3D))
        {
            Serial.println("SSD1306 allocation failed!");
            return;
        }
    }

    configTime("GET-4", "pool.ntp.org", "time.nist.gov", "time.google.com");

    Serial.print("Synchronizing NTP time");

    time_t now = time(nullptr);
    int retries = 0;
    while (now < 1000000000L && retries < 40)
    {
        delay(250);
        Serial.print(".");
        now = time(nullptr);
        retries++;
    }

    if (now > 1000000000L)
    {
        Serial.println("\nTime synced successfully!");
    }
    else
    {
        Serial.println("\nTime sync timed out! Check UDP/Internet connection.");
    }
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
    const int max_width = 128;
    const int max_height = 46;
    const int start_y = 18;

    int best_size = 1;

    for (int size = 4; size >= 1; size--)
    {
        int16_t x1, y1;
        uint16_t w, h;

        display.setTextSize(size);
        display.getTextBounds(msg, 0, start_y, &x1, &y1, &w, &h);

        if (w <= max_width && h <= max_height)
        {
            best_size = size;
            break;
        }
    }

    display.setTextSize(best_size);
    display.setCursor(0, start_y);
}

void DisplayBuilder::draw_happy_face()
{
    display.drawCircle(28, 36, 22, SSD1306_WHITE);

    display.fillCircle(20, 28, 3, SSD1306_WHITE);
    display.fillCircle(36, 28, 3, SSD1306_WHITE);

    display.drawCircleHelper(28, 36, 12, 4, SSD1306_WHITE);
    display.drawCircleHelper(28, 36, 12, 8, SSD1306_WHITE);
}

void DisplayBuilder::show_idle_screen()
{
    display.clearDisplay();

    draw_happy_face();

    time_t now = time(nullptr);
    struct tm *timeinfo = localtime(&now);

    char timeStr[9];
    char dateStr[11];

    if (timeinfo->tm_year > 70)
    {
        strftime(timeStr, sizeof(timeStr), "%H:%M", timeinfo);
        strftime(dateStr, sizeof(dateStr), "%b %d", timeinfo);
    }
    else
    {
        strcpy(timeStr, "Sync...");
        strcpy(dateStr, "NTP...");
    }

    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(2);
    display.setCursor(60, 20);
    display.print(timeStr);

    display.setTextSize(1);
    display.setCursor(60, 42);
    display.print(dateStr);

    display.display();
}

// Weather Module
void DisplayBuilder::draw_weather_icon()
{
    display.drawCircle(34, 22, 6, SSD1306_WHITE);
    display.drawLine(34, 12, 34, 14, SSD1306_WHITE);
    display.drawLine(42, 22, 44, 22, SSD1306_WHITE);
    display.drawLine(40, 16, 42, 14, SSD1306_WHITE);

    display.fillCircle(18, 40, 8, SSD1306_WHITE);
    display.fillCircle(28, 35, 11, SSD1306_WHITE);
    display.fillCircle(38, 40, 7, SSD1306_WHITE);
    display.fillRect(18, 40, 20, 8, SSD1306_WHITE);
}

void DisplayBuilder::show_weather_screen(float temp, String condition)
{
    display.clearDisplay();

    draw_weather_icon();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(60, 18);
    display.print((int)temp);

    int16_t degreeX = display.getCursorX() + 2;
    display.drawCircle(degreeX, 20, 2, SSD1306_WHITE);
    display.setCursor(degreeX + 6, 18);
    display.print("C");

    display.setTextSize(1);
    display.setCursor(60, 42);

    if (condition.length() > 10)
    {
        condition = condition.substring(0, 10);
    }
    display.print(condition);

    display.display();
}