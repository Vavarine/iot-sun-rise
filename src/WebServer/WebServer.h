#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <ESP8266WebServer.h>
#include <Espalexa.h>

#include "LittleFS.h"
#include "utils/User.h"

enum Auth { REQUIRE_AUTH };

class WebServer {
public:
  WebServer(Espalexa* espalexa);
  typedef std::function<void(User* user)> TAuthenticatedHandlerFunction;

  void begin();
  void handleClient();

  void on(const String &uri, HTTPMethod method, ESP8266WebServer::THandlerFunction handler);
  void on(const String &uri, HTTPMethod method, Auth auth, ESP8266WebServer::THandlerFunction handler);
  void on(const String &uri, HTTPMethod method, Auth auth, TAuthenticatedHandlerFunction handler);

  void send(int code);
  void send(int code, const String &content_type, const String &content);
  void setCookie(const String name, const String value);

  String body();
  String getCookie(const String name);
  String arg(const String &name);

private:
  ESP8266WebServer server;
  Espalexa* espalexa;

  void handleNotFound();

  bool handleFileRead(String path);

  String getContentType(String filename);

  bool ensureAuthenticated();
  String getJWTPayload();
  User* getUser();
};

#endif
