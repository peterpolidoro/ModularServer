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


namespace RemoteDevice
{
class Command;
class RemoteDevice;

class Parameter
{
public:
  Parameter(char *name);
  void setName(char *name);
  void setUnits(char *units);
  void setTypeLong();
  void setTypeDouble();
  void setTypeString();
  void setRange(long min, long max);
  void setRange(double min, double max);
  void removeRange();
private:
  char name_[STRING_LENGTH_PARAMETER_NAME];
  char units_[STRING_LENGTH_PARAMETER_UNITS];
  ParameterType type_;
  NumberType min_;
  NumberType max_;
  boolean range_is_set_;
  boolean compareName(char *name_to_compare);
  char* getName();
  char* getUnits();
  ParameterType getType();
  boolean rangeIsSet();
  NumberType getMin();
  NumberType getMax();
  friend class Method;
  friend class RemoteDevice;
};
}
#endif
