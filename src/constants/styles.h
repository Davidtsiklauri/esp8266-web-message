#ifndef STYLES_H
#define STYLES_H

#include <Arduino.h>

struct CssRule
{
    const char *selector;
    const char *rules;
};

// Declarations to share with main.cpp
extern const CssRule cssMap[];
extern const int numCssRules;

#endif // STYLES_H