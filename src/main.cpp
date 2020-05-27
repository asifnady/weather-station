#include <TridentTD_OpenWeather.h>
#include <TM1637Display.h>

#define ssid      "WLAN-188633"
#define pass      "52569188968217068708"

#define OpenWeather_APIKEY    "d1c79deba4ee09adfedf09953890adbc"

#define lat       48.13
#define lon       11.37

#define timezone  7


const int CLK = D1;
const int DIO = D2;

TridentTD_OpenWeather myPlace(OpenWeather_APIKEY);

TM1637Display display(CLK, DIO);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.println(myPlace.getVersion());
  Serial.println();

  myPlace.wificonnect(ssid, pass);

  myPlace.setLocation(lat, lon );

  display.setBrightness(0x08);


  /* OPTIONAL to set "unit_type"
     by default   "metric"   ( Celcius, meter/sec)
     or change to "imperial" ( Fahrenheit, miles/hour) */
  //myPlace.setUnit("metric");
  //myPlace.setUnit("imperial");

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

  display.showNumberDec(myPlace.readTemperature());

}

void loop() {
  // put your main code here, to run repeatedly:

}
