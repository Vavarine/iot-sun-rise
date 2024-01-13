#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "LittleFS.h"
#include "secrets.h"
#include "WebServer/WebServer.h"
#include "utils/utils.h"
#include "constants.h"

WebServer webServer;

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
  Serial.println(WiFi.localIP());
}

void handleRoot() {
  blink(1, 100, DEBUG_LED);

  webServer.send(200, "text/plain", "hello from esp8266!");
}

void setup() {
  pinMode(DEBUG_LED, OUTPUT);
  Serial.begin(9600);

  blink(2, 100, DEBUG_LED);

  connectToWifi(SECRET_SSID, SECRET_PASSWORD);
  
  webServer.begin();

  webServer.on("/api", HTTP_GET, handleRoot);
}

void loop() {
  webServer.handleClient();
}
