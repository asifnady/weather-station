#include <TridentTD_OpenWeather.h>
#include <TM1637Display.h>
#include <PubSubClient.h>

const char* mqttServer = "io.adafruit.com";
const int mqttPort = 1883;
const char* mqttUser = "asifnadeem";
const char* mqttPassword = "aio_AwiV95Wyp9dneXTK5Rk5pcdQkHx4";

#define ssid      "WLAN-188633"
#define pass      "52569188968217068708"


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
WiFiClient espclient;
PubSubClient client(espclient);

// set timezone offset from UTC
int timeZone = 2; // UTC - 4 eastern daylight time (nyc)
int interval = 1; // trigger every X hours
int hour = 0; // current hour
int last_min = -1;

void reconnect();
void callback(char* topic, byte* payload, unsigned int length);

void timecallback_minute(int current){
  // adjust to local time zone
current += (timeZone * 60 * 60);
int curr_hour = (current / 60 / 60) % 24;
int curr_min  = (current / 60 ) % 60;
int curr_sec  = (current) % 60;

Serial.print("Time: ");
Serial.print(curr_hour); Serial.print(':');
Serial.print(curr_min); Serial.print(':');
Serial.println(curr_sec);

// only trigger on minute change
if(curr_min != last_min) {
  last_min = curr_min;

  Serial.println("This will print out every minute!");
  }
}
void timecallback(int current) {

  // stash previous hour
  int previous = hour;

  // adjust to local time zone
  current += (timeZone * 60 * 60);

  // calculate current hour
  hour = (current / 60 / 60) % 24;
  Serial.println("Run your code here");

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

  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
      Serial.println("connected");
      client.subscribe("time/seconds");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);

    }
  }

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

}

void loop() {
  if (!client.connected()) {
    Serial.println("here");
    reconnect();
  }
  client.loop();
  delay(1000);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
      Serial.println("connected");
      // ... and resubscribe
      client.subscribe("time/seconds");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  payload[length] = '\0';
  String s = String((char*)payload);
  int time = s.toInt();
  Serial.println(time);
  timecallback_minute(time);

}
