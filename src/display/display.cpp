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

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3D))
        {
            Serial.println("SSD1306 allocation failed!");
            return;
        }
    }

    // Configure timezone (e.g., UTC+4 for Georgia/Tbilisi: "GET-4")
    // Replace "GET-4" with your local POSIX string if different (e.g. "EST5EDT" or "GMT0")
    configTime("GET-4", "pool.ntp.org", "time.nist.gov", "time.google.com");

    Serial.print("Synchronizing NTP time");

    // Wait up to 10 seconds for NTP sync
    time_t now = time(nullptr);
    int retries = 0;
    while (now < 1000000000L && retries < 40) // Wait until timestamp > year 2001
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

void DisplayBuilder::draw_happy_face()
{
    // Draw Face Circle (Center: x=28, y=36, radius=22)
    display.drawCircle(28, 36, 22, SSD1306_WHITE);

    // Eyes
    display.fillCircle(20, 28, 3, SSD1306_WHITE);
    display.fillCircle(36, 28, 3, SSD1306_WHITE);

    // Smile Arc
    display.drawCircleHelper(28, 36, 12, 4, SSD1306_WHITE); // Bottom-left arc
    display.drawCircleHelper(28, 36, 12, 8, SSD1306_WHITE); // Bottom-right arc
}

void DisplayBuilder::show_idle_screen()
{
    display.clearDisplay();

    // 1. Draw Happy Face on Left Side
    draw_happy_face();

    // 2. Fetch Time
    time_t now = time(nullptr);
    struct tm *timeinfo = localtime(&now);

    char timeStr[9];  // HH:MM
    char dateStr[11]; // Mon DD

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

    // 3. Render Clock on Right Side
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(2);
    display.setCursor(60, 20);
    display.print(timeStr);

    display.setTextSize(1);
    display.setCursor(60, 42);
    display.print(dateStr);

    display.display();
}