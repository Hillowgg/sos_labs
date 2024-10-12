#pragma once

typedef const char* Color;

static Color blueColor  = "\033[34m";
static Color greenColor = "\033[32m";
static Color cyanColor  = "\033[36m";
static const char* end  = "\033[0m";

// makeColorful returns dynamically allocated string
char* makeColorful(char *str, Color color);

