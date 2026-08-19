#pragma once

#include <ESP8266WiFi.h>
#include "constants/styles.h"

class Web
{
public:
    String currentMessage = "Hello World";
    void setup_html(WiFiClient &client, const String &currentMessage)
    {
        // HTTP Headers
        client.println(F("HTTP/1.1 200 OK"));
        client.println(F("Content-Type: text/html"));
        client.println(F("Connection: close"));
        client.println();

        // HTML Head & Dynamic CSS Generation
        client.println(F("<!DOCTYPE HTML><html>"));
        client.println(F("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"));
        client.println(F("<style>"));

        // Loop through CSS Map Array to write style rules
        for (int i = 0; i < numCssRules; i++)
        {
            client.print(cssMap[i].selector);
            client.print(F(" { "));
            client.print(cssMap[i].rules);
            client.println(F(" }"));
        }

        client.println(F("</style></head>"));

        // HTML Body
        client.println(F("<body>"));
        client.println(F("  <div class=\"card\">"));
        client.println(F("    <h2>ESP8266 Display</h2>"));

        client.print(F("    <p class=\"msg-box\">Current: \""));
        client.print(currentMessage);
        client.println(F("\"</p>"));

        client.println(F("    <form action=\"/setMessage\" method=\"GET\">"));
        client.println(F("      <input type=\"text\" name=\"msg\" placeholder=\"Type new message\" required>"));
        client.println(F("      <button type=\"submit\">Send to Display</button>"));
        client.println(F("    </form>"));
        client.println(F("  </div>"));
        client.println(F("</body></html>"));
    };

    void parse_user_input(WiFiClient client)
    {
        String request = client.readStringUntil('\r');
        Serial.println(request);
        client.flush();

        if (request.indexOf("GET /setMessage?msg=") >= 0)
        {
            int startIdx = request.indexOf("msg=") + 4;
            int endIdx = request.indexOf(" HTTP/");
            if (startIdx != -1 && endIdx != -1)
            {
                String rawMsg = request.substring(startIdx, endIdx);
                currentMessage = urlDecode(rawMsg);
            }
        }
    };

    String urlDecode(String input)
    {
        String decoded = "";
        char temp[] = "0x00";
        for (unsigned int i = 0; i < input.length(); i++)
        {
            if (input[i] == '+')
            {
                decoded += ' ';
            }
            else if (input[i] == '%' && i + 2 < input.length())
            {
                temp[2] = input[i + 1];
                temp[3] = input[i + 2];
                decoded += (char)strtol(temp, NULL, 16);
                i += 2;
            }
            else
            {
                decoded += input[i];
            }
        }
        return decoded;
    }
};