#include "DataFilesManager.h"
#include "LittleFS.h"
#include "FS.h"

DataFilesManager::DataFilesManager(const String &baseDir) : baseDirectory(baseDir) {}

void DataFilesManager::begin() {
  if(!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
}

void DataFilesManager::save(const String &filename, const String &content) {
  const String path = baseDirectory + "/" + filename;

  Serial.println("Writing file: " + path);

  File file = LittleFS.open(path, "w");
  
  if(!file){
      Serial.println("- failed to open file for writing");
      return;
  }
  if(file.print(content)){
      Serial.println("- file written");
  } else {
      Serial.println("- write failed");
  }

  file.close();
}

String DataFilesManager::load(const String &filename) {
  const String path = baseDirectory + "/" + filename;

  Serial.println("Loading file: " + path);

  if(!LittleFS.exists(path)) {
    Serial.println("File does not exist: " + path);

    return "";
  }

  File file = LittleFS.open(path, "r");
  String content = file.readString();
  file.close();

  return content;
}

void DataFilesManager::remove(const String &filename) {
  const String path = baseDirectory + "/" + filename;
  LittleFS.remove(path);
}

void DataFilesManager::list() {
  Dir dir = LittleFS.openDir(baseDirectory);

  while(dir.next()) {
    Serial.print(dir.fileName());
    Serial.print(" - ");
    File file = dir.openFile("r");
    Serial.println(file.size());
  }
}