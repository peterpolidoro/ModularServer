// ----------------------------------------------------------------------------
// JsonPrinter.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef JSON_PRINTER_H
#define JSON_PRINTER_H
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Streaming.h"
#include "Constants.h"

// #include <stdarg.h>

// #define DP_DOUBLE_PREC 12
// #define DP_STR_LEN 30

namespace RemoteDevice
{
class JsonPrinter {
 public:
  JsonPrinter(Stream &stream=Serial);
  void setStream(Stream &stream);
  void startObject();
  void stopObject();
  void startArray();
  void stopArray();
  void setCompactPrint();
  void setPrettyPrint();
  void addKey(const char *key);
  template<typename T>
  void add(T value);
  template<typename T>
  void add(const char *key, T value)
  {
    addKey(key);
    add(value);
  }
  // void addEmptyItem(char *key);
  // void addFltItem(char *key, float value);
  // void addDblItem(char *key, double value);
  // void addLongTuple(char *key, uint8_t num, ...);
  // int length();
 private:
  Stream *stream_ptr_;
  int items_count_;
  boolean pretty_print_;
  int indent_level_;
  boolean object_mode_;
  void indent();
  void stopItem();
  void stopArrayItem();
};
}
#endif
