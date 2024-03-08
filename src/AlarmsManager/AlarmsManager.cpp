#include "AlarmsManager.h"
#include <ArduinoJson.h>
#include <TimeAlarms.h>

JsonDocument alarmsDoc;

AlarmsManager::AlarmsManager(DataFilesManager &dataFilesManager, void (*callback)()) :
    dataFilesManager(dataFilesManager), callbackFunction(callback) {}
    
void AlarmsManager::begin(int currentHour, int currentMinute, int currentSecond, int currentDay, int currentMonth, int currentYear) {
  dataFilesManager.begin();
  setTime(currentHour, currentMinute, currentSecond, currentDay, currentMonth, currentYear);
  setupAlarms();
}

void AlarmsManager::setupAlarms() {
  Serial.println("Setting up alarms");

  String alarms = dataFilesManager.load("alarms");
  DeserializationError error = deserializeJson(alarmsDoc, alarms);

  if(error) {
    Serial.println("Error deserializing alarms");
    
    return;
  }

  if(!alarmsDoc.containsKey("alarms")) {
    Serial.println("No alarms found");

    return;
  }

  Serial.println("Alarms found");

  if(Alarm.count() > 0) {
    // Reseting alarms
    Serial.printf("Total Alarms ID´s before cleaning: %d \n", Alarm.count());     //Total id´s before
    for (byte id = 0; id < 255; id++)  { Alarm.free(id);  }    //Cron_IDs are numbered from 0 to 254  
    Serial.printf("Total Alarm ID´s after cleaning: %d \n", Alarm.count());     //Total id´s after, must be 0
  }

  JsonArray alarmsArray = alarmsDoc["alarms"].as<JsonArray>();

  for(JsonVariant alarm : alarmsArray) {
    int hour = alarm["time"]["hour"];
    int minute = alarm["time"]["minute"];

    if(!alarm["enabled"]) continue;

    if(alarm["days"]["sunday"]) {
      Alarm.alarmRepeat(dowSunday, hour, minute, 0, callbackFunction);
    }

    if(alarm["days"]["monday"]) {
      Alarm.alarmRepeat(dowMonday, hour, minute, 0, callbackFunction);
    }

    if(alarm["days"]["tuesday"]) {
      Alarm.alarmRepeat(dowTuesday, hour, minute, 0, callbackFunction);
    }

    if(alarm["days"]["wednesday"]) {
      Alarm.alarmRepeat(dowWednesday, hour, minute, 0, callbackFunction);
    }

    if(alarm["days"]["thursday"]) {
      Alarm.alarmRepeat(dowThursday, hour, minute, 0, callbackFunction);
    }

    if(alarm["days"]["friday"]) {
      Alarm.alarmRepeat(dowFriday, hour, minute, 0, callbackFunction);
    }

    if(alarm["days"]["saturday"]) {
      Alarm.alarmRepeat(dowSaturday, hour, minute, 0, callbackFunction);
    }
  }

  Serial.println("Final Alarms set up: " + String(Alarm.count()));
}

bool AlarmsManager::saveAlarms(const String &alarms) {
  DeserializationError error = deserializeJson(alarmsDoc, alarms);

  if(error) {
    Serial.println("Error deserializing alarms");
    Serial.println(error.c_str());

    return false;
  }

  Serial.println("Saving alarms to file");
  // Serial.println(alarms);

  dataFilesManager.save("alarms", alarmsDoc.as<String>());

  setupAlarms();

  return true;
}

String AlarmsManager::loadAlarms() {
  return dataFilesManager.load("alarms");
}

void AlarmsManager::update() {
  Alarm.delay(1);
}
