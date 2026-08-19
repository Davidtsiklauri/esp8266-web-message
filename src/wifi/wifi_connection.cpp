#include "wifi_connection.h"

const char *ssid = "Wi-Fi ";
const char *password = "123456789";

void WifiAdapter::setup_wifi_connection()
{
    WiFi.begin(ssid, password);

    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }

    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: http://");
    Serial.println(WiFi.localIP());
}