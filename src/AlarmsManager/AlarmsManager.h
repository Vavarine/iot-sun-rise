#ifndef ALARMS_MANAGER_H
#define ALARMS_MANAGER_H

#include "DataFilesManager/DataFilesManager.h"
#include <IRremoteESP8266.h>
#include <IRac.h>

class AlarmsManager {
public:
  AlarmsManager(DataFilesManager &dataFilesManager, void (*callback)() = 0) : dataFilesManager(dataFilesManager) {}
  void begin(int currentHour, int currentMinute, int currentSecond, int currentDay, int currentMonth, int currentYear);
  void update();
  bool saveAlarms(const String &alarm);
  String loadAlarms();

private:
  DataFilesManager dataFilesManager;
  void (*callbackFunction)();
  void setupAlarms();
};

#endif
