#ifndef ALARMS_MANAGER_H
#define ALARMS_MANAGER_H

#include "DataFilesManager/DataFilesManager.h"
#include <IRremoteESP8266.h>
#include <IRac.h>

class AlarmsManager {
public:
  AlarmsManager(DataFilesManager &dataFilesManager, void (*callback)() = 0) : dataFilesManager(dataFilesManager), callbackFunction(callback) {}
  void begin();
  void update();
  bool saveAlarms(const String &alarm);
  String loadAlarms();

private:
  DataFilesManager dataFilesManager;
  void (*callbackFunction)();
  void setupAlarms();
};

#endif
