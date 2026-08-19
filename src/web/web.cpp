#pragma once

#include <ESP8266WiFi.h>
#include "constants/styles.h"

class Web
{
    String currentMessage = "Hello World";

public:
    void setup_html(WiFiClient &client, const String &currentMessage)
    {
        // HTTP Headers
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();

        // HTML Head & Dynamic CSS Generation via Loop
        client.println("<!DOCTYPE HTML><html>");
        client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
        client.println("<style>");

        // Loop through CSS Map Array to write style rules
        for (int i = 0; i < numCssRules; i++)
        {
            client.print(cssMap[i].selector);
            client.print(" { ");
            client.print(cssMap[i].rules);
            client.println(" }");
        }

        client.println("</style></head>");

        // HTML Body
        client.println("<body>");
        client.println("  <div class=\"card\">");
        client.println("    <h2>ESP8266 Display</h2>");

        client.print("    <p class=\"msg-box\">Current: \"");
        client.print(currentMessage);
        client.println("\"</p>");

        client.println("    <form action=\"/setMessage\" method=\"GET\">");
        client.println("      <input type=\"text\" name=\"msg\" placeholder=\"Type new message\" required>");
        client.println("      <button type=\"submit\">Send to Display</button>");
        client.println("    </form>");
        client.println("  </div>");
        client.println("</body></html>");
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