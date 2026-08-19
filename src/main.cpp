#include <ESP8266WiFi.h>
#include <Wire.h>
#include "constants/styles.h"
#include "web/web.h"
#include "display/display.h"
#include "wifi/wifi_connection.h"

DisplayBuilder display_builder;

WiFiServer server(80);
Web web = Web();
WifiAdapter wifi_adapter(server);

void setup()
{
  Serial.begin(9600);

  wifi_adapter.setup_wifi_connection();
  wifi_adapter.setup_server();
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
