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

void DisplayBuilder::draw_weather_icon()
{
    display.drawCircle(22, 9, 3, SSD1306_WHITE);
    display.drawLine(22, 4, 22, 5, SSD1306_WHITE);
    display.drawLine(27, 9, 28, 9, SSD1306_WHITE);
    display.drawLine(25, 6, 26, 5, SSD1306_WHITE);

    display.fillCircle(9, 20, 4, SSD1306_WHITE);
    display.fillCircle(15, 17, 6, SSD1306_WHITE);
    display.fillCircle(21, 20, 4, SSD1306_WHITE);
    display.fillRect(9, 20, 12, 4, SSD1306_WHITE);
}

// 1. Mini Sun Icon
void draw_mini_sun(Adafruit_SSD1306 &display, int x, int y)
{
    display.drawCircle(x + 5, y + 4, 3, SSD1306_WHITE);
    display.drawLine(x + 5, y, x + 5, y + 1, SSD1306_WHITE);     // Top
    display.drawLine(x + 5, y + 7, x + 5, y + 8, SSD1306_WHITE); // Bottom
    display.drawLine(x + 1, y + 4, x + 2, y + 4, SSD1306_WHITE); // Left
    display.drawLine(x + 8, y + 4, x + 9, y + 4, SSD1306_WHITE); // Right
}

// 2. Mini Cloud Icon
void draw_mini_cloud(Adafruit_SSD1306 &display, int x, int y)
{
    display.fillCircle(x + 2, y + 4, 2, SSD1306_WHITE);  // Left puff
    display.fillCircle(x + 5, y + 2, 3, SSD1306_WHITE);  // Center puff
    display.fillCircle(x + 8, y + 4, 2, SSD1306_WHITE);  // Right puff
    display.fillRect(x + 2, y + 4, 7, 2, SSD1306_WHITE); // Flat base
}

// 3. Mini Rain Icon (Cloud + Rain drops)
void draw_mini_rain(Adafruit_SSD1306 &display, int x, int y)
{
    // Draw cloud base
    display.fillCircle(x + 2, y + 3, 2, SSD1306_WHITE);
    display.fillCircle(x + 5, y + 1, 3, SSD1306_WHITE);
    display.fillCircle(x + 8, y + 3, 2, SSD1306_WHITE);
    display.fillRect(x + 2, y + 3, 7, 2, SSD1306_WHITE);

    // Draw falling rain streaks underneath
    display.drawLine(x + 3, y + 7, x + 2, y + 8, SSD1306_WHITE);
    display.drawLine(x + 6, y + 7, x + 5, y + 8, SSD1306_WHITE);
}

void DisplayBuilder::show_weather_screen(float current_temp, String condition,
                                         int hour1, float temp1,
                                         int hour2, float temp2,
                                         int hour3, float temp3)
{
    display.clearDisplay();

    // 1. TOP HALF: Current Weather (0 to 31 Y-axis)
    draw_weather_icon();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(42, 2);
    display.print((int)current_temp);

    // Degree symbol (°)
    int16_t degreeX = display.getCursorX() + 1;
    display.drawCircle(degreeX, 4, 2, SSD1306_WHITE);
    display.setCursor(degreeX + 5, 2);
    display.print("C");

    // Condition String
    display.setTextSize(1);
    display.setCursor(42, 20);
    if (condition.length() > 13)
    {
        condition = condition.substring(0, 13);
    }
    display.print(condition);

    // Horizontal Divider
    display.drawFastHLine(0, 31, 128, SSD1306_WHITE);

    // 2. BOTTOM HALF: Stacked Vertical List (3 Rows with dynamic icons)
    auto draw_stacked_row = [&](int y_pos, int hour, float temp, int icon_type)
    {
        char h_str[6];
        snprintf(h_str, sizeof(h_str), "%02d:00", hour);

        // Time on the left
        display.setCursor(4, y_pos);
        display.print(h_str);

        // Render appropriate mini icon in the middle slot (X offset: ~54)
        if (icon_type == 0)
        {
            draw_mini_sun(display, 54, y_pos);
        }
        else if (icon_type == 1)
        {
            draw_mini_cloud(display, 54, y_pos);
        }
        else
        {
            draw_mini_rain(display, 54, y_pos); // Fixed: using y_pos directly
        }

        // Temperature on the right
        display.setCursor(88, y_pos);
        display.print((int)temp);
        display.print("C");
    };
    // Render 3 stacked rows (Passing different icon types: 0=Sun, 1=Cloud, 2=Rain)
    display.setTextSize(1);
    draw_stacked_row(35, hour1, temp1, 0); // Row 1: Sun icon
    draw_stacked_row(45, hour2, temp2, 1); // Row 2: Cloud icon
    draw_stacked_row(55, hour3, temp3, 2); // Row 3: Rain icon

    display.display();
}