#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "LittleFS.h"
#include <assert.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <ArduinoJson.h>

#include "secrets.h"
#include "WebServer/WebServer.h"
#include "JsonDataFilesManger/JsonDataFilesManager.h"
#include "utils/utils.h"
#include "constants.h"

const uint16_t kIrLed = 4 ;  // ESP8266 GPIO pin to use. Recommended: 4 (D2). 
const uint16_t kRecvPin = 5;

const uint32_t kBaudRate = 9600;

IRsend irsend(kIrLed); 

WebServer webServer;
JsonDataFilesManager jsonFilesManager("/json-data-files");
JsonDocument doc;

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
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.print(WiFi.localIP());
  Serial.println("");
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

void handleSaveAlarms() {
  deserializeJson(doc, webServer.arg("plain"));

  if(!doc.is<JsonObject>()) {
    webServer.send(400, "text/plain", "invalid json");
    return;
  }

  Serial.println("Saving alarms to file");

  jsonFilesManager.save("/alarms.json", doc.as<String>());

  webServer.send(200, "text/json", "{\"success\": true}");
}

void handleLoadAlarms() {
  Serial.println("Loading alarms from file");

  String alarms = jsonFilesManager.load("/alarms.json");

  webServer.send(200, "text/json", alarms);
}

void setup() {
  Serial.begin(kBaudRate, SERIAL_8N1, SERIAL_TX_ONLY);

  while (!Serial) {
    delay(50);
  }
  
  assert(irutils::lowLevelSanityCheck() == 0);

  irsend.begin();

  pinMode(DEBUG_LED, OUTPUT);

  blink(2, 100, DEBUG_LED);

  connectToWifi(SECRET_SSID, SECRET_PASSWORD);
  webServer.begin();
  jsonFilesManager.begin();

  webServer.on("/api", HTTP_GET, handleRoot);
  webServer.on("/api/ir-send", HTTP_POST, handleIrSend);
  webServer.on("/api/alarms", HTTP_POST, handleSaveAlarms);
  webServer.on("/api/alarms", HTTP_GET, handleLoadAlarms);
}

void loop() {
  webServer.handleClient();
}
