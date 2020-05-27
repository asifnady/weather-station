/**The MIT License (MIT)

Copyright (c) 2018 by ThingPulse Ltd., https://thingpulse.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <JsonListener.h>
#include <time.h>
#include <TridentTD_OpenWeather.h>


//BASE URL for HTTP GET
String OPEN_WEATHER_BASE_URL = "http://api.openweathermap.org/data/2.5/onecall?";

// See https://docs.thingpulse.com/how-tos/openweathermap-key/
String OPEN_WEATHER_MAP_APP_ID = "d1c79deba4ee09adfedf09953890adbc";

// GERMERING Latitude and Longitude
String OPEN_WEATHER_MAP_LOCATION_LAT = "48.13";
String OPEN_WEATHER_MAP_LOCATION_LON = "11.37";

String OPEN_WEATHER_MAP_LANGUAGE = "en";
String OPEN_WEATHER_UNIT = "metric";
/*EXCLUDE LIST: current,minutely,hourly,daily
Should be in comma delimited format without space
*/
String OPEN_WEATHER_WEATHER_DATA = "minutely,hourly,daily";

String OPEN_WEATHER_FINAL_URL = "http://api.openweathermap.org/data/2.5/onecall?";


/**
 * WiFi Settings
 */
const char* ESP_HOST_NAME = "esp-" + ESP.getFlashChipId();
const char* WIFI_SSID     = "WLAN-188633";
const char* WIFI_PASSWORD = "52569188968217068708";

// initiate the WifiClient
WiFiClient wifiClient;


void getCurrentWeather(){
  OPEN_WEATHER_FINAL_URL+= "lat=" + OPEN_WEATHER_MAP_LOCATION_LAT;
  OPEN_WEATHER_FINAL_URL+= "&lon=" + OPEN_WEATHER_MAP_LOCATION_LON;
  OPEN_WEATHER_FINAL_URL+= "&exclude=" + OPEN_WEATHER_WEATHER_DATA;
  OPEN_WEATHER_FINAL_URL+= "&appid=" + OPEN_WEATHER_MAP_APP_ID;
  OPEN_WEATHER_FINAL_URL+= "&units=" + OPEN_WEATHER_UNIT;
}

/**
 * Helping funtions
 */
void connectWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.println(WiFi.localIP());
  Serial.println();
}


/**
 * SETUP
 */
void setup() {
  OPEN_WEATHER_FINAL_URL+= "lat=" + OPEN_WEATHER_MAP_LOCATION_LAT;
  OPEN_WEATHER_FINAL_URL+= "&lon=" + OPEN_WEATHER_MAP_LOCATION_LON;
  OPEN_WEATHER_FINAL_URL+= "&exclude=" + OPEN_WEATHER_WEATHER_DATA;
  OPEN_WEATHER_FINAL_URL+= "&appid=" + OPEN_WEATHER_MAP_APP_ID;
  OPEN_WEATHER_FINAL_URL+= "&units=" + OPEN_WEATHER_UNIT;
  Serial.begin(115200);
  delay(500);
  connectWifi();
  if (WiFi.status() == WL_CONNECTED) {
     HTTPClient http;  //Object of class HTTPClient
     http.begin(OPEN_WEATHER_FINAL_URL);
     int httpCode = http.GET();
     Serial.println(OPEN_WEATHER_FINAL_URL);
     if(httpCode > 0) {
       if(httpCode == HTTP_CODE_OK) {
         Serial.println("HTTP_CODE_OK");
         String payload = http.getString();
         // TODO: Parsing
         const size_t capacity = 8*JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(8) + 16*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + 8*JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(12) + 2*JSON_OBJECT_SIZE(13) + 5*JSON_OBJECT_SIZE(14) + 1810;
         DynamicJsonDocument doc(capacity);

         deserializeJson(doc, payload.c_str());

         JsonArray daily = doc["daily"];
         JsonObject daily_0 = daily[0];
         long daily_0_dt = daily_0["dt"]; // 1590231600
         long daily_0_sunrise = daily_0["sunrise"]; // 1590204362
         long daily_0_sunset = daily_0["sunset"]; // 1590260212

         JsonObject daily_0_temp = daily_0["temp"];
         float daily_0_temp_day = daily_0_temp["day"]; // 22.18
         float daily_0_temp_min = daily_0_temp["min"]; // 13.88
         float daily_0_temp_max = daily_0_temp["max"]; // 22.21
         float daily_0_temp_night = daily_0_temp["night"]; // 14.77
         float daily_0_temp_eve = daily_0_temp["eve"]; // 18.8
         float daily_0_temp_morn = daily_0_temp["morn"]; // 13.88

         JsonObject daily_0_feels_like = daily_0["feels_like"];
         float daily_0_feels_like_day = daily_0_feels_like["day"]; // 18.16
         float daily_0_feels_like_night = daily_0_feels_like["night"]; // 3.05
         float daily_0_feels_like_eve = daily_0_feels_like["eve"]; // 6.24
         float daily_0_feels_like_morn = daily_0_feels_like["morn"]; // 14.57

         JsonObject daily_0_weather_0 = daily_0["weather"][0];
         int daily_0_weather_0_id = daily_0_weather_0["id"]; // 501
         const char* daily_0_weather_0_main = daily_0_weather_0["main"]; // "Rain"
         const char* daily_0_weather_0_description = daily_0_weather_0["description"];

         Serial.println(daily_0_temp_day);
         Serial.println(daily_0_temp_min);
         Serial.println(daily_0_temp_max);
         Serial.println(daily_0_temp_night);
         Serial.println(daily_0_temp_eve);
         Serial.println(daily_0_temp_morn);
      }
    }
  http.end();   //Close connection
  }


}


/**
 * LOOP
 */
void loop() {

}
