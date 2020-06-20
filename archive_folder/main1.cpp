#include <TridentTD_OpenWeather.h>
#include <TM1637Display.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


#define ssid      "WLAN-188633"
#define pass      "52569188968217068708"


#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "asifnadeem"
#define AIO_KEY         "aio_KtWU11b1XocxIohZcd7I3Vf9Pm5a"

#define OpenWeather_APIKEY    "d1c79deba4ee09adfedf09953890adbc"

#define lat       48.13
#define lon       11.37

#define timezone  7


const int CLK = D1;
const int DIO = D2;
uint8_t C_char[] = {0x39};
uint8_t F_char[] = {0x71};
uint8_t H_char[] = {0x75};
uint8_t L_char[] = {0x38};

TridentTD_OpenWeather myPlace(OpenWeather_APIKEY);

TM1637Display display(CLK, DIO);


WiFiClientSecure client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe timefeed = Adafruit_MQTT_Subscribe(&mqtt, "time/seconds");

// set timezone offset from UTC
int timeZone = 2; // UTC - 4 eastern daylight time (nyc)
int interval = 1; // trigger every X hours
int hour = 0; // current hour

void timecallback(uint32_t current) {

  // stash previous hour
  int previous = hour;

  // adjust to local time zone
  current += (timeZone * 60 * 60);

  // calculate current hour
  hour = (current / 60 / 60) % 24;

  // only trigger on interval
  if((hour != previous) && (hour % interval) == 0) {
    Serial.println("Run your code here");
    myPlace.weatherCurrent();
    display.clear();
    display.setSegments(C_char);
    int currtemp = round(myPlace.readTemperature());
    display.showNumberDec(currtemp, false, 2, 2);
  }

}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.println(myPlace.getVersion());
  Serial.println();

  myPlace.wificonnect(ssid, pass);

  myPlace.setLocation(lat, lon );

  display.setBrightness(0x08);

  myPlace.weatherCurrent();

  Serial.println("--------------------------------------------");
  Serial.println("[OpenWeather] Location   : " + myPlace.latitude() + ", "+ myPlace.longitude() );
  Serial.println("[OpenWeather] Temperature: " + String(myPlace.readTemperature()));  // [metric] Celcius  or [imperial] Fahrenheit
  Serial.println("[OpenWeather] Feels-Like : " + String(myPlace.readTempFeelsLike()));  // [metric] Celcius  or [imperial] Fahrenheit
  Serial.println("[OpenWeather] Humidity   : " + String(myPlace.readHumidity()));     // %
  Serial.println("[OpenWeather] Pressure   : " + String(myPlace.readPressure()));     // hPa
  Serial.println("[OpenWeather] Weather    : " + myPlace.readWeather());
  Serial.println("[OpenWeather] Wind Speed : " + String(myPlace.readWindSpeed()));    // [metric] meter/sec  or [imperial] miles/hour
  Serial.println("[OpenWeather] Wind Deg   : " + String(myPlace.readWindDeg()));      // degrees
  Serial.println("[OpenWeather] Cloudiness : " + String(myPlace.readCloudiness()));   // %
  Serial.println("[OpenWeather] Sunrise    : " + String(myPlace.readSunrise(timezone)));
  Serial.println("[OpenWeather] Sunset     : " + String(myPlace.readSunset(timezone)));
  Serial.println("[OpenWeather] DateTime   : " + String(myPlace.readDateTime(timezone)));
  Serial.println("--------------------------------------------");

  display.clear();
  display.setSegments(C_char);
  int currtemp = round(myPlace.readTemperature());
  display.showNumberDec(currtemp, false, 2, 2);

  timefeed.setCallback(timecallback);
  mqtt.subscribe(&timefeed);

}
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }

  Serial.println("MQTT Connected!");
}

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // wait 10 seconds for subscription messages
  // since we have no other tasks in this example.
  mqtt.processPackets(10000);

  // keep the connection alive
  mqtt.ping();
}
