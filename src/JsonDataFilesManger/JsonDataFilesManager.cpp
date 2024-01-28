#include "JsonDataFilesManager.h"
#include "LittleFS.h"

JsonDataFilesManager::JsonDataFilesManager(const String &baseDir) : baseDirectory(baseDir) {}

void JsonDataFilesManager::begin() {
  if(!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
}

void JsonDataFilesManager::save(const String &filename, const String &content) {
  const String path = baseDirectory + "/" + filename;
  File file = LittleFS.open(path, "w");
  file.print(content);
  file.close();
}

String JsonDataFilesManager::load(const String &filename) {
  const String path = baseDirectory + "/" + filename;
  File file = LittleFS.open(path, "r");
  String content = file.readString();
  file.close();

  return content;
}

void JsonDataFilesManager::remove(const String &filename) {
  const String path = baseDirectory + "/" + filename;
  LittleFS.remove(path);
}

void JsonDataFilesManager::list() {
  Dir dir = LittleFS.openDir(baseDirectory);

  while(dir.next()) {
    Serial.print(dir.fileName());
    Serial.print(" - ");
    File file = dir.openFile("r");
    Serial.println(file.size());
  }
}