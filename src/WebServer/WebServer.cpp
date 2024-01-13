#include <ESP8266WebServer.h>
#include "LittleFS.h"
#include "WebServer.h"
#include "utils/utils.h"
#include "constants.h"

WebServer::WebServer(int port) : server(port) {}

void WebServer::begin() {
  server.begin();

  if(!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  server.onNotFound([this]() {
    this->handleNotFound();
  });

  Serial.println("HTTP server started");
}

void WebServer::handleClient() {
  server.handleClient();
}

void WebServer::on(const String &uri, HTTPMethod method, ESP8266WebServer::THandlerFunction handler) {
  Serial.println(method + " " + uri);
  server.on(uri, method, handler);
}

void WebServer::send(int code, const String &content_type, const String &content) {
  server.send(code, content_type, content);
}

void WebServer::handleNotFound() {
  if (!handleFileRead(server.uri())) {
    // return index.html if it doesn't exist
    handleFileRead("/index.html");
  }
}

bool WebServer::handleFileRead(String path) {
  blink(1, 100, DEBUG_LED);

  if (path.endsWith("/")) {
    path += "index.html";
  }

  Serial.println("handleFileRead: " + path);

  String contentType = getContentType(path);

  if (LittleFS.exists(path)) {
    File file = LittleFS.open(path, "r");
    server.streamFile(file, contentType);

    file.close();
    return true;
  }

  Serial.println("\tFile Not Found");
  return false;
}

String WebServer::getContentType(String filename) {
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
