#include "styles.h"

const CssRule cssMap[] = {
    {"body", "font-family: Arial, sans-serif; background-color: #f4f6f9; text-align: center; padding-top: 50px; margin: 0;"},
    {".card", "background: #ffffff; padding: 30px; border-radius: 12px; box-shadow: 0 4px 12px rgba(0,0,0,0.1); display: inline-block; width: 320px; max-width: 90%;"},
    {"h2", "color: #333333; margin-bottom: 20px; font-size: 22px;"},
    {".msg-box", "font-size: 18px; color: #007bff; font-weight: bold; margin-bottom: 25px; word-break: break-word;"},
    {"input[type=text]", "width: 100%; padding: 12px 16px; margin-bottom: 15px; border: 2px solid #dcdcdc; border-radius: 8px; font-size: 16px; box-sizing: border-box; outline: none; transition: all 0.3s ease;"},
    {"input[type=text]:focus", "border-color: #007bff; box-shadow: 0 0 8px rgba(0, 123, 255, 0.25);"},
    {"button", "width: 100%; background-color: #007bff; color: #ffffff; padding: 12px; border: none; border-radius: 8px; font-size: 16px; font-weight: bold; cursor: pointer; transition: background-color 0.2s ease, transform 0.1s ease;"},
    {"button:hover", "background-color: #0056b3;"},
    {"button:active", "transform: scale(0.98);"}};

const int numCssRules = sizeof(cssMap) / sizeof(cssMap[0]);