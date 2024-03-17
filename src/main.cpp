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
#include "fonts/whitrabt55pt7b.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "secrets.h"
#include "WebServer/WebServer.h"
#include "DataFilesManager/DataFilesManager.h"
#include "AlarmsManager/AlarmsManager.h"
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

int TFT_LED_BRIGHTNESS;


void changeTftBrightness(int brightness) {
  dataFilesManager.save("test", String(brightness));
  TFT_LED_BRIGHTNESS = brightness;
  analogWrite(TFT_LED, brightness);
}

void setupTftBrightness() {
  pinMode(TFT_LED, OUTPUT);

  String brightness = dataFilesManager.load("test");

  if(brightness.length() > 0) {
    TFT_LED_BRIGHTNESS = brightness.toInt();
  } else {
    TFT_LED_BRIGHTNESS = 400;
  }

  changeTftBrightness(TFT_LED_BRIGHTNESS);
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

  changeTftBrightness(400);
  
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

int currentMinute = 0;
int currentHour = 0;

void setupTft() {
  analogWrite(TFT_LED, 400);
  tft.init(240, 320);
  tft.invertDisplay(false);
  tft.setTextWrap(false);
  tft.setRotation(1);
  tft.setFont(&whitrabt55pt7b);

  setupTftBrightness();
}

void printTime() {
  timeClient.update();

  currentMinute = timeClient.getMinutes();
  currentHour = timeClient.getHours();

  tft.fillScreen(ST7735_BLACK);  // fill screen with black color
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);  // set text color to white and black background

  tft.setCursor(5, 80); // X, Y
  tft.print(currentHour < 10 ? "0" : "");
  tft.print(currentHour);
  tft.print("h");
  tft.print(currentMinute < 10 ? "0" : "");
  tft.print(currentMinute);
  // tft.print("00h00");

  // tft.setTextSize(2);
  // tft.setCursor(10, 100);
  // tft.print(alarmsManager.getFormattedDate());
}

void handleSaveAlarms() {
  if(alarmsManager.saveAlarms(webServer.arg("plain"))) {
    webServer.send(200, "text/json", "{\"success\": true}");
  } else {
    webServer.send(500, "text/json", "{\"success\": false}");
  }
}

void handleLoadAlarms() {
  String alarms = alarmsManager.loadAlarms();

  webServer.send(200, "text/json", alarms);
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

  // Setups TFT screen and prints time
  setupTft();
  printTime();

  // Prints time to screen every minute
  Alarm.timerOnce(60 - timeClient.getSeconds(), []() {
    printTime();
    Alarm.timerRepeat(60, printTime);
  });

  // dims the screen at 10pm
  Alarm.alarmRepeat(22, 0, 0, []() {
    changeTftBrightness(5);
  });

  // brightens the screen at 10am
  Alarm.alarmRepeat(10, 0, 0, []() {
    changeTftBrightness(400);
  });

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
