#include "wifi_connection.h" // Must include your header!

const char *ssid = "Wi-Fi ";
const char *password = "123456789";

// Define the constructor using WifiAdapter::
WifiAdapter::WifiAdapter(WiFiServer &serverRef) : server(serverRef) {}

// Define the method using WifiAdapter::
void WifiAdapter::setup_wifi_connection()
{
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
}

void WifiAdapter::setup_server()
{
    server.begin();
    Serial.println("Server started");
    Serial.print("Use this URL: http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
}