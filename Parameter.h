// ----------------------------------------------------------------------------
// Parameter.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef PARAMETER_H
#define PARAMETER_H
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "JsonGenerator.h"
#include "Constants.h"


class Command;
class RemoteDevice;

class Parameter
{
public:
  Parameter(char *name);
  void setName(char *name);
  void setUnits(char *units);
private:
  char name_[STRING_LENGTH_PARAMETER_NAME];
  char units_[STRING_LENGTH_PARAMETER_UNITS];
  boolean compareName(char *name_to_compare);
  char* getName();
  char* getUnits();
  friend class Method;
  friend class RemoteDevice;
};
#endif
