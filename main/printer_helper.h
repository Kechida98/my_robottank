#ifndef PRINTER_HELPER_H
#define PRINTER_HELPER_H

#include <stdio.h>

// ANSI fcolour codes
static const char *green   = "\033[0;32m";
static const char *yellow  = "\033[0;33m";
static const char *red     = "\033[0;31m";
static const char *blue    = "\033[0;34m"; 
static const char *magenta = "\033[0;35m";
static const char *white   = "\033[0;37m";
static const char *reset   = "\033[0m";

// Formate
#define PRINTFC(color, format, ...) printf("%s" format "%s\n", color, ##__VA_ARGS__, reset)

// Module-special macron
#define PRINTFC_MAIN(format, ...)         PRINTFC(yellow,  "Main: " format, ##__VA_ARGS__)
#define PRINTFC_WIFI_HANDLER(format, ...) PRINTFC(magenta, "WiFi: " format, ##__VA_ARGS__)
#define PRINTFC_MQTT_HANDLER(format, ...) PRINTFC(blue,    "MQTT: " format, ##__VA_ARGS__)

#endif // PRINTER_HELPER_H
