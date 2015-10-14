// ----------------------------------------------------------------------------
// JsonPrinter.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_DEVICE_JSON_PRINTER_H_
#define _MODULAR_DEVICE_JSON_PRINTER_H_
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Streaming.h"
#include "Array.h"
#include "ConstantVariable.h"
#include "GenericSerial.h"
#include "Constants.h"


namespace ModularDevice
{
class JsonDepthTracker
{
public:
  JsonDepthTracker();
  JsonDepthTracker(bool first_item, bool inside_object);
  bool first_item_;
  bool inside_object_;
};

class JsonPrinter
{
public:
  JsonPrinter(GenericSerial &serial);
  void setSerial(GenericSerial &serial);
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
  void addNull(const char *key)
  {
    addKey(key);
    addNull();
  }
  void addNull();
  void addBool(const char *key, const bool value)
  {
    addKey(key);
    addBool(value);
  }
  void addBool(const bool value);
private:
  GenericSerial generic_serial_;
  bool pretty_print_;
  int indent_level_;
  Array<JsonDepthTracker,constants::RESPONSE_DEPTH_MAX> jdt_array_;
  void indent();
  void stopItem();
  void stopArrayItem();
};
}
#endif
