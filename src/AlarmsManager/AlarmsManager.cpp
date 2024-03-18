#include <TimeAlarms.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "AlarmsManager.h"
#include <ArduinoJson.h>

JsonDocument alarmsDoc;

int* alarmIds = nullptr;
// Track the current size and capacity of the array
int alarmsIdsSize = 0;
int alarmsIdsCapacity = 0;

// Function to add a new element to the array
void addAlarmId(int newValue) {
    // Check if we need to resize the array
    if (alarmsIdsSize >= alarmsIdsCapacity) {
        // Double the capacity (or use any other resizing strategy)
        alarmsIdsCapacity = (alarmsIdsCapacity == 0) ? 1 : alarmsIdsCapacity * 2;
        
        // Create a new array with the increased capacity
        int* newArray = new int[alarmsIdsCapacity];
        
        // Copy elements from the old array to the new one
        for (int i = 0; i < alarmsIdsSize; i++) {
            newArray[i] = alarmIds[i];
        }
        
        // Delete the old array
        delete[] alarmIds;
        
        // Update the pointer to point to the new array
        alarmIds = newArray;
    }
    
    // Add the new value to the array
    alarmIds[alarmsIdsSize] = newValue;
    
    // Increment the size
    alarmsIdsSize++;
}

void resetAlarmIds() {
  if(alarmsIdsSize > 0) {
    // Reseting alarms
    Serial.printf("Total Alarms ID´s before cleaning: %d \n", Alarm.count());     //Total id´s before
    for(int i = 0; i < alarmsIdsSize; i++) {
      Alarm.free(alarmIds[i]);
    }
    Serial.printf("Total Alarm ID´s after cleaning: %d \n", Alarm.count());     //Total id´s after, must be 0
  }

  if(alarmIds != nullptr) {
    delete[] alarmIds;
  }

  alarmIds = nullptr;
  alarmsIdsSize = 0;
  alarmsIdsCapacity = 0;
}

void AlarmsManager::begin() {
  dataFilesManager.begin();
  
  setupTime();
  setupAlarms();
}

void AlarmsManager::setupTime() {
  timeClient.begin();
  timeClient.update();
  timeClient.setTimeOffset(-10800); // GMT-3

  // Set time
  time_t epochTime = timeClient.getEpochTime();
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  int currentSecond = timeClient.getSeconds();
  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  int currentYear = ptm->tm_year+1900;

  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);

  setTime(currentHour, currentMinute, currentSecond, monthDay, currentMonth, currentYear);
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

  resetAlarmIds();

  JsonArray alarmsArray = alarmsDoc["alarms"].as<JsonArray>();

  for(JsonVariant alarm : alarmsArray) {
    int hour = alarm["time"]["hour"];
    int minute = alarm["time"]["minute"];

    if(!alarm["enabled"]) continue;

    if(alarm["days"]["sunday"]) {
      addAlarmId(Alarm.alarmRepeat(dowSunday, hour, minute, 0, callbackFunction));
    }

    if(alarm["days"]["monday"]) {
      addAlarmId(Alarm.alarmRepeat(dowMonday, hour, minute, 0, callbackFunction));
    }

    if(alarm["days"]["tuesday"]) {
      addAlarmId(Alarm.alarmRepeat(dowTuesday, hour, minute, 0, callbackFunction));
    }

    if(alarm["days"]["wednesday"]) {
      addAlarmId(Alarm.alarmRepeat(dowWednesday, hour, minute, 0, callbackFunction));
    }

    if(alarm["days"]["thursday"]) {
      addAlarmId(Alarm.alarmRepeat(dowThursday, hour, minute, 0, callbackFunction));
    }

    if(alarm["days"]["friday"]) {
      addAlarmId(Alarm.alarmRepeat(dowFriday, hour, minute, 0, callbackFunction));
    }

    if(alarm["days"]["saturday"]) {
      addAlarmId(Alarm.alarmRepeat(dowSaturday, hour, minute, 0, callbackFunction));
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
