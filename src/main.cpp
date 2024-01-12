#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "LittleFS.h"
#include "secrets.h"

#define LED D4

ESP8266WebServer server(80);

void blink(int times, int delayTime)
{
  for (int i = 0; i < times; i++)
  {
    delay(delayTime);
    digitalWrite(LED, LOW);
    delay(delayTime);
    digitalWrite(LED, HIGH);
  }
}

String getContentType(String filename)
{
  // if (server.hasArg("download")) return "application/octet-stream";
  if (filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".ico"))
    return "image/x-icon";
  else if (filename.endsWith(".png"))
    return "image/png";
  else if (filename.endsWith(".svg"))
    return "image/svg+xml";

  return "text/plain";
}

bool handleFileRead(String path)
{
  blink(1, 100);

  if (path.endsWith("/"))
    path += "index.html";

  Serial.println("handleFileRead: " + path);

  String contentType = getContentType(path);

  if (LittleFS.exists(path))
  {
    File file = LittleFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);

    Serial.println(String("\tSent file: ") + path);
    Serial.println(String("\tSent size: ") + sent);

    file.close();
    return true;
  }

  Serial.println("\tFile Not Found");
  return false;
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

void handleRoot()
{
  server.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound()
{
  if (!handleFileRead(server.uri()))
    // return index.html if it doesn't exist
    handleFileRead("/index.html");
}

void serverSetup()
{
  if (!LittleFS.begin())
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  server.on("/api", handleRoot);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void setup()
{
  pinMode(LED, OUTPUT);
  blink(2, 100);

  Serial.begin(9600);

  connectToWifi(SECRET_SSID, SECRET_PASSWORD);
  serverSetup();
}

void loop()
{
  server.handleClient();
}
