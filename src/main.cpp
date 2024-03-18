#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "LittleFS.h"
#include <assert.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <Adafruit_ST7789.h>
#include <Adafruit_ST7735.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include "constants.h"
#include "secrets.h"
#include "WebServer/WebServer.h"
#include "DataFilesManager/DataFilesManager.h"
#include "AlarmsManager/AlarmsManager.h"
#include "utils/utils.h"
#include "fonts/whitrabt55pt7b.h"
#include "fonts/whitrabt40pt7b.h"
#include "fonts/whitrabt11pt7b.h"

#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15

#define TFT_CS D8
#define TFT_RST D2
#define TFT_DC D3

#define TFT_LED D1 // D1

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
const uint16_t kIrLed = 16;  // ESP8266 GPIO pin to use. Recommended: 4 (D2). 
const uint32_t kBaudRate = 9600;
IRsend irsend(kIrLed); 
WebServer webServer;
DataFilesManager dataFilesManager("/json-data-files");
JsonDocument doc;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

WiFiClient client;
HTTPClient http;

int TFT_LED_BRIGHTNESS;

int currentMinute = 0;
int currentHour = 0;

int currentTemperature = 0;
int forecastMaxTemperature = 0;

int forecastMinTemperature = 0;
String currentWeatherText = "";

String date = "";

// months
const char *months[] = {"janeiro", "fevereiro", "marco", "abril", "maio", "junho", "julho", "agosto", "setembro", "outubro", "novembro", "dezembro"};
// days
const char *days[] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};


void changeTftDisplayBrightness(int brightness) {
  dataFilesManager.save("brightness", String(brightness));
  TFT_LED_BRIGHTNESS = brightness;
  analogWrite(TFT_LED, brightness);
}

void setupTftDisplayBrightness() {
  pinMode(TFT_LED, OUTPUT);

  String brightness = dataFilesManager.load("brightness");

  if(brightness.length() > 0) {
    TFT_LED_BRIGHTNESS = brightness.toInt();
  } else {
    TFT_LED_BRIGHTNESS = 400;
  }

  changeTftDisplayBrightness(TFT_LED_BRIGHTNESS);
}

void connectToWifi(const char *ssid, const char *password) {
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  blink(3, 100, DEBUG_LED);
  Serial.println(" ");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(" ");
}

void handleIrSend() {
  deserializeJson(doc, webServer.arg("plain"));

  if(!doc.is<JsonObject>()) {
    webServer.send(400, "text/plain", "invalid json");
    return;
  }

  serializeJson(doc, Serial);
  Serial.println();

  uint32_t code = doc["code"].as<uint32_t>();

  irsend.sendNEC(code);

  webServer.send(200, "text/plain", "hello from esp8266!");
}

void handleRoot() {
  blink(1, 100, DEBUG_LED);

  webServer.send(200, "text/json", "{message: 'hello from esp8266!', success}");
}

void alarmCallback() {
  // Turn on
  irsend.sendNEC(0xFFE01F);
  // Set color to white
  irsend.sendNEC(0xFFC03F);

  changeTftDisplayBrightness(400);
  
  // reduce brightness
  for(int i = 0; i < 10; i++) {
    irsend.sendNEC(0xFF40BF);
  }

  // turn bightness up every 5 minutes
  for(int i = 0; i < 4; i++) {
    Alarm.timerOnce(60*(5*(i+1)), []() {
      irsend.sendNEC(0xFF00FF);
    });
  }

  Serial.println("Alarm callback");
}

AlarmsManager alarmsManager(dataFilesManager, timeClient, alarmCallback);

void setupTftDisplay() {
  analogWrite(TFT_LED, 400);
  tft.init(240, 320);
  tft.invertDisplay(false);
  tft.setTextWrap(false);
  tft.setRotation(1);

  setupTftDisplayBrightness();
}

void printInfoOnDisplay() {
  timeClient.update();

  currentMinute = timeClient.getMinutes();
  currentHour = timeClient.getHours();

  tft.fillScreen(ST7735_BLACK);  // fill screen with black color
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);  // set text color to white and black background

  // date
  tft.setFont(&whitrabt11pt7b);
  tft.setCursor(20, 20);
  tft.print(date);

  // hour
  tft.setFont(&whitrabt55pt7b);
  tft.setCursor(5, 110); // X, Y
  tft.print(currentHour < 10 ? "0" : "");
  tft.print(currentHour);
  tft.print("h");
  tft.print(currentMinute < 10 ? "0" : "");
  tft.print(currentMinute);

  // weather
  tft.setFont(&whitrabt11pt7b);
  tft.setCursor(20, 160);
  tft.print(currentWeatherText);

  // temperature
  tft.setFont(&whitrabt40pt7b);
  tft.setCursor(5, 230);
  tft.print(String(currentTemperature) + "'C");

  // forecast
  tft.setFont(&whitrabt11pt7b);
  tft.setCursor(200, 200);
  tft.print("min: " + String(forecastMinTemperature) + "'C");
  tft.setCursor(200, 225);
  tft.print("max: " + String(forecastMaxTemperature) + "'C");
}

void updateDateTime() {
  timeClient.update();

  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;

  date = String(days[ptm->tm_wday]) + ", " + String(monthDay) + " de " + String(months[currentMonth-1]); 
}

void updateWeather() {
  http.begin(client, "http://api.openweathermap.org/data/3.0/onecall?lat=" + String(SECRET_LAT) + "&lon=" + String(SECRET_LON) + "&appid=" + String(SECRET_OPENWEATHER_API_KEY) + "&units=metric&lang=pt_br&exclude=minutely,hourly,alerts");

  int httpCode = http.GET();
  String payload = "{}";

  if(httpCode == 200) {
    payload = http.getString();
  } else {
    payload = http.getString();
    Serial.println("Error on HTTP request");
    Serial.println(httpCode);
    return;
  }

  http.end();
  
  DeserializationError error = deserializeJson(doc, payload);

  if(error) {
    Serial.println("Error deserializing weather");
    Serial.println(error.c_str());
    return;
  }

  JsonObject weather = doc.as<JsonObject>();

  currentTemperature = weather["current"]["temp"].as<int>();
  currentWeatherText = weather["current"]["weather"][0]["description"].as<String>();
  forecastMinTemperature = weather["daily"][0]["temp"]["min"].as<int>();
  forecastMaxTemperature = weather["daily"][0]["temp"]["max"].as<int>();

  dataFilesManager.save("temperature", String(currentTemperature));
  dataFilesManager.save("weatherText", currentWeatherText);
  dataFilesManager.save("maxTemperature", String(forecastMaxTemperature));
  dataFilesManager.save("minTemperature", String(forecastMinTemperature));
}

void setupWeather() {
  String loadedTemperature = dataFilesManager.load("temperature");
  String loadedWeatherText = dataFilesManager.load("weatherText");
  String loadedMaxTemperature = dataFilesManager.load("maxTemperature");
  String loadedMinTemperature = dataFilesManager.load("minTemperature");

  if(loadedTemperature.length() > 0 && loadedWeatherText.length() > 0 && loadedMaxTemperature.length() > 0 && loadedMinTemperature.length() > 0){
    currentTemperature = loadedTemperature.toInt();
    currentWeatherText = loadedWeatherText;
    forecastMaxTemperature = loadedMaxTemperature.toInt();
    forecastMinTemperature = loadedMinTemperature.toInt();
  } else {
    updateWeather();
  }
}

void handleLoadAlarms() {
  String alarms = alarmsManager.loadAlarms();

  webServer.send(200, "text/json", alarms);
}

void setupSystemTasksAlarms() {
  // Prints time to screen every minute
  Alarm.timerOnce(60 - timeClient.getSeconds(), []() {
    printInfoOnDisplay();
    Alarm.timerRepeat(60, printInfoOnDisplay);
  });

  // updates weather every 30 minutes
  Alarm.timerRepeat(60*30, updateWeather);

  // updates date every day
  Alarm.alarmRepeat(0, 0, 0, []() {
    updateDateTime();
  });

  // dims the screen at 10pm
  Alarm.alarmRepeat(22, 0, 0, []() {
    changeTftDisplayBrightness(5);
  });

  // brightens the screen at 10am
  Alarm.alarmRepeat(10, 0, 0, []() {
    changeTftDisplayBrightness(400);
  });
}

void handleSaveAlarms() {
  if(alarmsManager.saveAlarms(webServer.arg("plain"))) {
    webServer.send(200, "text/json", "{\"success\": true}");
  } else {
    webServer.send(500, "text/json", "{\"success\": false}");
  }
}

void setup() {
  Serial.begin(kBaudRate);
  Serial.println("Starting up");

  assert(irutils::lowLevelSanityCheck() == 0);
  
  pinMode(DEBUG_LED, OUTPUT);
  blink(1, 100, DEBUG_LED);
  
  irsend.begin();

  // Connects to wifi
  connectToWifi(SECRET_SSID, SECRET_PASSWORD);

  // Setups time client
  timeClient.begin();
  timeClient.update();
  timeClient.setTimeOffset(-10800); // GMT-3

  // Setups alarms manager and data files manager
  dataFilesManager.begin();
  alarmsManager.begin();

  // Gets current weather and forecast
  updateWeather();
  setupWeather();
  updateDateTime();

  // Setups TFT screen and prints time
  setupTftDisplay();
  printInfoOnDisplay();

  // Setups system tasks
  setupSystemTasksAlarms();

  // Setups web server
  webServer.begin();

  // Setups routes
  webServer.on("/api", HTTP_GET, handleRoot);
  webServer.on("/api/ir-send", HTTP_POST, handleIrSend);
  webServer.on("/api/alarms", HTTP_POST, handleSaveAlarms);
  webServer.on("/api/alarms", HTTP_GET, handleLoadAlarms);
}

void loop() {
  alarmsManager.update();
  webServer.handleClient();
}
