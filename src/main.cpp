#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "LittleFS.h"
#include "secrets.h"
#include "WebServer/WebServer.h"

#define LED D4

WebServer webServer;

void blink(int times, int delayMs, int LED = D4) {
  for (int i = 0; i < times; i++) {
    delay(delayMs);
    digitalWrite(LED, LOW);
    delay(delayMs);
    digitalWrite(LED, HIGH);
  }
}

void connectToWifi(const char *ssid, const char *password)
{
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  blink(3, 100);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void handleRoot() {
  webServer.send(200, "text/plain", "hello from esp8266!");
}

void setup()
{
  pinMode(LED, OUTPUT);
  blink(2, 100);

  Serial.begin(9600);

  connectToWifi(SECRET_SSID, SECRET_PASSWORD);
  
  webServer.begin();

  webServer.on("/api", HTTP_GET, handleRoot);
}

void loop()
{
  webServer.handleClient();
}
