// ----------------------------------------------------------------------------
// Parameter.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _PARAMETER_H_
#define _PARAMETER_H_
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "JsonGenerator.h"

using namespace ArduinoJson;

const int PARAMETER_NAME_STRING_LENGTH_MAX = 16;
const int PARAMETER_UNITS_STRING_LENGTH_MAX = 8;
// const int PARAMETER_HELP_STRING_LENGTH_MAX = 64;

const int PARAMETER_HELP_JSON_OBJECT_SIZE = 8;

class Command;
class DeviceInterface;

class Parameter
{
public:
  Parameter(char *name);
  void setName(char *name);
  void setUnits(char *units);
private:
  char name_[PARAMETER_NAME_STRING_LENGTH_MAX];
  char units_[PARAMETER_UNITS_STRING_LENGTH_MAX];
  boolean compareName(char *name_to_compare);
  char* getName();
  char* getUnits();
  // Generator::JsonObject<PARAMETER_HELP_JSON_OBJECT_SIZE> help_json_object;
  // Generator::JsonObject<PARAMETER_HELP_JSON_OBJECT_SIZE> help();
  friend class Command;
  friend class DeviceInterface;
};
#endif
