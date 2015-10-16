// ----------------------------------------------------------------------------
// Parameter.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Parameter.h"


namespace ModularDevice
{
CONSTANT_STRING(default_parameter_name,"");
CONSTANT_STRING(default_parameter_units,"");

Parameter::Parameter()
{
  setName(default_parameter_name);
  setUnits(default_parameter_units);
  type_ = constants::LONG_PARAMETER;
  array_element_type_ = constants::LONG_PARAMETER;
  range_is_set_ = false;
}

Parameter::Parameter(const ConstantString &name)
{
  setName(name);
  setUnits(default_parameter_units);
  type_ = constants::LONG_PARAMETER;
  array_element_type_ = constants::LONG_PARAMETER;
  range_is_set_ = false;
}

void Parameter::setName(const ConstantString &name)
{
  name_ptr_ = &name;
}

void Parameter::setUnits(const ConstantString &units)
{
  units_ptr_ = &units;
}

void Parameter::setTypeLong()
{
  if (type_ != constants::ARRAY_PARAMETER)
  {
    type_ = constants::LONG_PARAMETER;
  }
  else
  {
    array_element_type_ = constants::LONG_PARAMETER;
  }
}

void Parameter::setTypeDouble()
{
  if (type_ != constants::ARRAY_PARAMETER)
  {
    type_ = constants::DOUBLE_PARAMETER;
  }
  else
  {
    array_element_type_ = constants::DOUBLE_PARAMETER;
  }
}

void Parameter::setTypeBool()
{
  if (type_ != constants::ARRAY_PARAMETER)
  {
    type_ = constants::BOOL_PARAMETER;
  }
  else
  {
    array_element_type_ = constants::BOOL_PARAMETER;
  }
}

void Parameter::setTypeString()
{
  if (type_ != constants::ARRAY_PARAMETER)
  {
    type_ = constants::STRING_PARAMETER;
  }
  else
  {
    array_element_type_ = constants::STRING_PARAMETER;
  }
}

void Parameter::setTypeObject()
{
  type_ = constants::OBJECT_PARAMETER;
}

void Parameter::setTypeArray()
{
  array_element_type_ = type_;
  type_ = constants::ARRAY_PARAMETER;
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

bool Parameter::compareName(const char *name_to_compare)
{
  char name[constants::STRING_LENGTH_PARAMETER_NAME] = {0};
  name_ptr_->copy(name);
  return String(name).equalsIgnoreCase(name_to_compare);
}

bool Parameter::compareName(const ConstantString &name_to_compare)
{
  return (&name_to_compare == name_ptr_);
}

const ConstantString* Parameter::getNamePointer()
{
  return name_ptr_;
}

const ConstantString* Parameter::getUnitsPointer()
{
  return units_ptr_;
}

constants::ParameterType Parameter::getType()
{
  return type_;
}

constants::ParameterType Parameter::getArrayElementType()
{
  return array_element_type_;
}

bool Parameter::rangeIsSet()
{
  return range_is_set_;
}

constants::NumberType Parameter::getMin()
{
  return min_;
}

constants::NumberType Parameter::getMax()
{
  return max_;
}
}
