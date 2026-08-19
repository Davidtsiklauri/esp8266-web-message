#include <ESP8266WiFi.h>
#include <Wire.h>
#include "constants/styles.h"
#include "web/web.h"
#include "display/display.h"

const char *ssid = "Wi-Fi ";
const char *password = "123456789";

DisplayBuilder display_builder;

WiFiServer server(80);
Web web = Web();

void setup_wifi_connection()
{

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
}

void setup_server()
{
  server.begin();
  Serial.println("Server started");
  Serial.print("Use this URL: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void setup()
{
  Serial.begin(9600);

  setup_wifi_connection();
  setup_server();
  display_builder.setup_display();

  display_builder.update_display("hello World");
}

void loop()
{
  WiFiClient client = server.accept();

  if (!client)
  {
    return;
  }

  Serial.println("New client connected");

  // Wait until client sends data
  while (!client.available())
  {
    delay(1);
  }

  web.parse_user_input(client);

  display_builder.update_display(web.currentMessage);

  web.setup_html(client, web.currentMessage);

  delay(1);
  Serial.println("Client disconnected\n");
}
