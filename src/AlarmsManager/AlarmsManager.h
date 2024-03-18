#ifndef ALARMS_MANAGER_H
#define ALARMS_MANAGER_H

#include "DataFilesManager/DataFilesManager.h"
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <NTPClient.h>


class AlarmsManager {
public:
  AlarmsManager(DataFilesManager &dataFilesManager, NTPClient &timeClient, void (*callback)() = 0) : dataFilesManager(dataFilesManager), timeClient(timeClient), callbackFunction(callback) {}
  void begin();
  void update();
  bool saveAlarms(const String &alarm);
  String loadAlarms();

private:
  DataFilesManager dataFilesManager;
  NTPClient timeClient;
  void (*callbackFunction)();
  void setupAlarms();
  void setupTime();
};

#endif
