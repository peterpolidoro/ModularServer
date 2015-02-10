// ----------------------------------------------------------------------------
// Parameter.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Parameter.h"

using namespace ArduinoJson;

namespace ModularDevice
{
FLASH_STRING(default_parameter_name,"");
FLASH_STRING(default_parameter_units,"");

Parameter::Parameter(const _FLASH_STRING &name=default_parameter_name)
{
  setName(name);
  setUnits(default_parameter_units);
  type_ = LONG_PARAMETER;
  array_element_type_ = LONG_PARAMETER;
  range_is_set_ = false;
}

void Parameter::setName(const _FLASH_STRING &name)
{
  name_ptr_ = &name;
}

void Parameter::setUnits(const _FLASH_STRING &units)
{
  units_ptr_ = &units;
}

void Parameter::setTypeLong()
{
  if (type_ != ARRAY_PARAMETER)
  {
    type_ = LONG_PARAMETER;
  }
  else
  {
    array_element_type_ = LONG_PARAMETER;
  }
}

void Parameter::setTypeDouble()
{
  if (type_ != ARRAY_PARAMETER)
  {
    type_ = DOUBLE_PARAMETER;
  }
  else
  {
    array_element_type_ = DOUBLE_PARAMETER;
  }
}

void Parameter::setTypeBool()
{
  if (type_ != ARRAY_PARAMETER)
  {
    type_ = BOOL_PARAMETER;
  }
  else
  {
    array_element_type_ = BOOL_PARAMETER;
  }
}

void Parameter::setTypeString()
{
  if (type_ != ARRAY_PARAMETER)
  {
    type_ = STRING_PARAMETER;
  }
  else
  {
    array_element_type_ = STRING_PARAMETER;
  }
}

void Parameter::setTypeObject()
{
  type_ = OBJECT_PARAMETER;
}

void Parameter::setTypeArray()
{
  array_element_type_ = type_;
  type_ = ARRAY_PARAMETER;
}

void Parameter::setRange(const double min, const double max)
{
  min_.d = min;
  max_.d = max;
  setTypeDouble();
  range_is_set_ = true;
}

void Parameter::setRange(const float min, const float max)
{
  min_.d = (double)min;
  max_.d = (double)max;
  setTypeDouble();
  range_is_set_ = true;
}

void Parameter::removeRange()
{
  range_is_set_ = false;
}

boolean Parameter::compareName(const char *name_to_compare)
{
  char name[STRING_LENGTH_PARAMETER_NAME] = {0};
  name_ptr_->copy(name);
  return String(name).equalsIgnoreCase(name_to_compare);
}

boolean Parameter::compareName(const _FLASH_STRING &name_to_compare)
{
  return (&name_to_compare == name_ptr_);
}

const _FLASH_STRING* Parameter::getNamePointer()
{
  return name_ptr_;
}

const _FLASH_STRING* Parameter::getUnitsPointer()
{
  return units_ptr_;
}

ParameterType Parameter::getType()
{
  return type_;
}

ParameterType Parameter::getArrayElementType()
{
  return array_element_type_;
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
}
