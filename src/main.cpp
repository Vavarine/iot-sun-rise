#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
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

#include "secrets.h"
#include "WebServer/WebServer.h"
#include "DataFilesManager/DataFilesManager.h"
#include "utils/utils.h"
#include "constants.h"

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

void connectToWifi(const char *ssid, const char *password) {
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  analogWriteRange(1023);
  analogWrite(TFT_LED, 400); // Brilho do led

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

void setupTft() {
  pinMode(TFT_LED, OUTPUT);
  analogWriteRange(1023);
  analogWrite(TFT_LED, 1000); // Brilho do led

  tft.init(240, 320);
  tft.invertDisplay(false);
  tft.setTextWrap(false);
  tft.setRotation(1);
  tft.fillScreen(ST7735_BLACK);  // fill screen with black color
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);  // set text color to white and black background
  tft.setTextSize(3);
  tft.drawFastHLine(0, 44, tft.width(), ST7735_BLUE);

  tft.setCursor(20, 20); // X, Y
  tft.write("Hello world xd");
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

  Serial.print("Sending code: ");
  Serial.println(code);

  irsend.sendNEC(code);

  webServer.send(200, "text/plain", "hello from esp8266!");
}

void handleRoot() {
  blink(1, 100, DEBUG_LED);

  irsend.sendNEC(0xFF609F);
  webServer.send(200, "text/json", "{message: 'hello from esp8266!', success}");
}

void alarmCallback() {
  // Turn on
  irsend.sendNEC(0xFFE01F);
  // Set color to white
  irsend.sendNEC(0xFFC03F);


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

void setupAlarms() {
  Serial.println("Setting up alarms");

  String alarms = dataFilesManager.load("alarms");
  DeserializationError error = deserializeJson(doc, alarms);

  if(error) {
    Serial.println("Error deserializing alarms");
    
    return;
  }

  if(!doc.containsKey("alarms")) {
    Serial.println("No alarms found");

    return;
  }

  Serial.println("Alarms found");

  if(Alarm.count() > 0) {
    // Reseting alarms
    Serial.printf("Total Alarms ID´s before cleaning: %d \n", Alarm.count());     //Total id´s before
    for (byte id = 0; id < 255; id++)  { Alarm.free(id);  }    //Cron_IDs are numbered from 0 to 254  
    Serial.printf("Total Alarm ID´s after cleaning: %d \n", Alarm.count());     //Total id´s after, must be 0
  }

  JsonArray alarmsArray = doc["alarms"].as<JsonArray>();

  for(JsonVariant alarm : alarmsArray) {
    int hour = alarm["time"]["hour"];
    int minute = alarm["time"]["minute"];

    if(!alarm["enabled"]) continue;

    if(alarm["days"]["sunday"]) {
      Alarm.alarmRepeat(dowSunday, hour, minute, 0, alarmCallback);
    }

    if(alarm["days"]["monday"]) {
      Alarm.alarmRepeat(dowMonday, hour, minute, 0, alarmCallback);
    }

    if(alarm["days"]["tuesday"]) {
      Alarm.alarmRepeat(dowTuesday, hour, minute, 0, alarmCallback);
    }

    if(alarm["days"]["wednesday"]) {
      Alarm.alarmRepeat(dowWednesday, hour, minute, 0, alarmCallback);
    }

    if(alarm["days"]["thursday"]) {
      Alarm.alarmRepeat(dowThursday, hour, minute, 0, alarmCallback);
    }

    if(alarm["days"]["friday"]) {
      Alarm.alarmRepeat(dowFriday, hour, minute, 0, alarmCallback);
    }

    if(alarm["days"]["saturday"]) {
      Alarm.alarmRepeat(dowSaturday, hour, minute, 0, alarmCallback);
    }
  }

  Serial.println("Final Alarms set up: " + String(Alarm.count()));
}

void handleSaveAlarms() {
  DeserializationError error = deserializeJson(doc, webServer.arg("plain"));

  if(error) {
    Serial.println("Error deserializing alarms");
    Serial.println(error.c_str());

    webServer.send(400, "text/json", "{\"error\": \"invalid json\"}");

    return;
  }

  Serial.println("Saving alarms to file");
  dataFilesManager.save("alarms", doc.as<String>());
  setupAlarms();

  webServer.send(200, "text/json", "{\"success\": true}");
}

void handleLoadAlarms() {
  Serial.println("Loading alarms from file");

  String alarms = dataFilesManager.load("alarms");

  webServer.send(200, "text/json", alarms);
}

void setup() {
  Serial.begin(kBaudRate);
  
  Serial.println("Starting up");

  assert(irutils::lowLevelSanityCheck() == 0);

  irsend.begin();

  pinMode(DEBUG_LED, OUTPUT);

  blink(2, 100, DEBUG_LED);

  setupTft();

  connectToWifi(SECRET_SSID, SECRET_PASSWORD);

  // Setup NTP
  timeClient.begin();
  timeClient.update();
  timeClient.setTimeOffset(-10800); // GMT-3

  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);  

  // Set time
  time_t epochTime = timeClient.getEpochTime();
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  int currentSecond = timeClient.getSeconds();
  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  int currentYear = ptm->tm_year+1900;

  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  Serial.print("Current date: ");
  Serial.println(currentDate);
  
  // setTime(8,29,0,1,1,11);
  setTime(currentHour, currentMinute, currentSecond, monthDay, currentMonth, currentYear);

  dataFilesManager.begin();
  webServer.begin();
  setupAlarms();
  // alarmCallback();

  webServer.on("/api", HTTP_GET, handleRoot);
  webServer.on("/api/ir-send", HTTP_POST, handleIrSend);
  webServer.on("/api/alarms", HTTP_POST, handleSaveAlarms);
  webServer.on("/api/alarms", HTTP_GET, handleLoadAlarms);
}

void loop() {
  webServer.handleClient();
  Alarm.delay(1);
}
