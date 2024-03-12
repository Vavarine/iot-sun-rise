#include <TimeAlarms.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "AlarmsManager.h"
#include <ArduinoJson.h>

JsonDocument alarmsDoc;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// months
const char *months[] = {"janeiro", "fevereiro", "março", "abril", "maio", "junho", "julho", "agosto", "setembro", "outubro", "novembro", "dezembro"};
// days
const char *days[] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};
    
void AlarmsManager::begin() {
  dataFilesManager.begin();
  timeClient.begin();
  
  setupTime();
  setupAlarms();
}

void AlarmsManager::setTimeCallback(void (*callback)()) {
  Alarm.timerRepeat(1, callback);
}

int AlarmsManager::getHour() {
  return timeClient.getHours();
}

int AlarmsManager::getMinute() {
  return timeClient.getMinutes();
}

int AlarmsManager::getSecond() {
  return timeClient.getSeconds();
}

void AlarmsManager::setupTime() {
  timeClient.update();
  timeClient.setTimeOffset(-10800); // GMT-3

  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);  

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
  Serial.print("Current date: ");
  Serial.println(currentDate);

  setTime(currentHour, currentMinute, currentSecond, monthDay, currentMonth, currentYear);
}

String AlarmsManager::getFormattedTime() {
  return timeClient.getFormattedTime();
}

String AlarmsManager::getFormattedDate() {
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  int currentYear = ptm->tm_year+1900;
  int currentDay = ptm->tm_wday;

  return String(days[currentDay]) + ", " + String(monthDay) + " de " + String(months[currentMonth-1]) + " de " + String(currentYear);
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
