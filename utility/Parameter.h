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
#include "Flash.h"
#include "Constants.h"


namespace RemoteDevice
{
class Command;
class RemoteDevice;

class Parameter
{
public:
  Parameter(_FLASH_STRING& name);
  void setName(_FLASH_STRING& name);
  void setUnits(_FLASH_STRING& name);
  void setTypeLong();
  void setTypeDouble();
  void setTypeString();
  void setRange(long min, long max);
  void setRange(int min, int max);
  void setRange(double min, double max);
  void removeRange();
private:
  _FLASH_STRING *name_ptr_;
  _FLASH_STRING *units_ptr_;
  ParameterType type_;
  NumberType min_;
  NumberType max_;
  boolean range_is_set_;
  boolean compareName(char *name_to_compare);
  _FLASH_STRING* getNamePointer();
  _FLASH_STRING* getUnitsPointer();
  ParameterType getType();
  boolean rangeIsSet();
  NumberType getMin();
  NumberType getMax();
  friend class Method;
  friend class Server;
};
}
#endif
