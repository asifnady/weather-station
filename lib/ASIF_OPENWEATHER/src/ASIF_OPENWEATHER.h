#ifndef _ASIF_OPEN_WEATHER_H_
#define _ASIF_OPEN_WEATHER_H_

#include <Arduino.h>
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
#elif defined (ESP32)
  #include <WiFi.h>
  #include <HTTPClient.h>
#endif

#define  DEBUG_MODE

#ifdef DEBUG_MODE
    #define DEBUG_PRINTER Serial
    #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
    #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
    #define DEBUG_PRINT(...) { }
    #define DEBUG_PRINTLN(...) { }
#endif
