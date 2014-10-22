// ----------------------------------------------------------------------------
// Parameter.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Parameter.h"

using namespace ArduinoJson;

namespace RemoteDevice
{
FLASH_STRING(default_parameter_name,"");
FLASH_STRING(default_parameter_units,"");

Parameter::Parameter(_FLASH_STRING& name=default_parameter_name)
{
  setName(name);
  setUnits(default_parameter_units);
  setTypeLong();
  range_is_set_ = false;
}

void Parameter::setName(_FLASH_STRING& name)
{
  name_ptr_ = &name;
}

void Parameter::setUnits(_FLASH_STRING& units)
{
  units_ptr_ = &units;
}

void Parameter::setTypeLong()
{
  type_ = LONG_PARAMETER;
}

void Parameter::setTypeDouble()
{
  type_ = DOUBLE_PARAMETER;
}

void Parameter::setTypeString()
{
  type_ = STRING_PARAMETER;
}

void Parameter::setRange(long min, long max)
{
  min_.l = min;
  max_.l = max;
  setTypeLong();
  range_is_set_ = true;
}

void Parameter::setRange(int min, int max)
{
  min_.l = (long)min;
  max_.l = (long)max;
  setTypeLong();
  range_is_set_ = true;
}

void Parameter::setRange(double min, double max)
{
  min_.d = min;
  max_.d = max;
  setTypeDouble();
  range_is_set_ = true;
}

void Parameter::removeRange()
{
  range_is_set_ = false;
}

ParameterType Parameter::getType()
{
  return type_;
}

boolean Parameter::rangeIsSet()
{
  return range_is_set_;
}

NumberType Parameter::getMin()
{
  return min_;
}

NumberType Parameter::getMax()
{
  return max_;
}

boolean Parameter::compareName(char *name_to_compare)
{
  char name[STRING_LENGTH_PARAMETER_NAME] = {0};
  name_ptr_->copy(name);
  return String(name).equalsIgnoreCase(name_to_compare);
}

_FLASH_STRING* Parameter::getNamePointer()
{
  return name_ptr_;
}

_FLASH_STRING* Parameter::getUnitsPointer()
{
  return units_ptr_;
}
}
