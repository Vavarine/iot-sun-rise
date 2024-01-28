#ifndef JSON_DATA_FILES_MANAGER_H
#define JSON_DATA_FILES_MANAGER_H

#include <Arduino.h>

class JsonDataFilesManager { 
public:
  JsonDataFilesManager(const String &basedir);

  void begin();
  void save(const String &filename, const String &content);
  String load(const String &filename);
  void remove(const String &filename);
  void list();

private:
  String baseDirectory;
};

#endif
