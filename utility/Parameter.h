// ----------------------------------------------------------------------------
// Parameter.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_DEVICE_PARAMETER_H_
#define _MODULAR_DEVICE_PARAMETER_H_
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "JsonGenerator.h"
#include "Flash.h"
#include "Constants.h"


namespace ModularDevice
{
class Command;
class ModularDevice;

class Parameter
{
public:
  Parameter(const _FLASH_STRING &name);
  void setName(const _FLASH_STRING &name);
  void setUnits(const _FLASH_STRING &name);
  void setTypeLong();
  void setTypeDouble();
  void setTypeBool();
  void setTypeString();
  void setTypeObject();
  void setTypeArray();
  template<typename T>
  void setRange(const T min, const T max)
  {
    min_.l = (long)min;
    max_.l = (long)max;
    setTypeLong();
    range_is_set_ = true;
  }
  void setRange(const double min, const double max);
  void setRange(const float min, const float max);
  void removeRange();
private:
  const _FLASH_STRING *name_ptr_;
  const _FLASH_STRING *units_ptr_;
  constants::ParameterType type_;
  constants::ParameterType array_element_type_;
  constants::NumberType min_;
  constants::NumberType max_;
  boolean range_is_set_;
  boolean compareName(const char *name_to_compare);
  boolean compareName(const _FLASH_STRING &name_to_compare);
  const _FLASH_STRING* getNamePointer();
  const _FLASH_STRING* getUnitsPointer();
  constants::ParameterType getType();
  constants::ParameterType getArrayElementType();
  boolean rangeIsSet();
  constants::NumberType getMin();
  constants::NumberType getMax();
  friend class Method;
  friend class Server;
};
}
#endif
