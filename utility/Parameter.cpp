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
Parameter::Parameter()
{
  setName(constants::empty_constant_string);
  setUnits(constants::empty_constant_string);
  type_ = JsonPrinter::LONG_TYPE;
  array_element_type_ = JsonPrinter::LONG_TYPE;
  range_is_set_ = false;
}

Parameter::Parameter(const ConstantString &name)
{
  setName(name);
  setUnits(constants::empty_constant_string);
  type_ = JsonPrinter::LONG_TYPE;
  array_element_type_ = JsonPrinter::LONG_TYPE;
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
  if (type_ != JsonPrinter::ARRAY_TYPE)
  {
    type_ = JsonPrinter::LONG_TYPE;
  }
  else
  {
    array_element_type_ = JsonPrinter::LONG_TYPE;
  }
}

void Parameter::setTypeDouble()
{
  if (type_ != JsonPrinter::ARRAY_TYPE)
  {
    type_ = JsonPrinter::DOUBLE_TYPE;
  }
  else
  {
    array_element_type_ = JsonPrinter::DOUBLE_TYPE;
  }
}

void Parameter::setTypeBool()
{
  if (type_ != JsonPrinter::ARRAY_TYPE)
  {
    type_ = JsonPrinter::BOOL_TYPE;
  }
  else
  {
    array_element_type_ = JsonPrinter::BOOL_TYPE;
  }
}

void Parameter::setTypeString()
{
  if (type_ != JsonPrinter::ARRAY_TYPE)
  {
    type_ = JsonPrinter::STRING_TYPE;
  }
  else
  {
    array_element_type_ = JsonPrinter::STRING_TYPE;
  }
}

void Parameter::setTypeObject()
{
  type_ = JsonPrinter::OBJECT_TYPE;
}

void Parameter::setTypeArray()
{
  array_element_type_ = type_;
  type_ = JsonPrinter::ARRAY_TYPE;
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

JsonPrinter::JsonTypes Parameter::getType()
{
  return type_;
}

JsonPrinter::JsonTypes Parameter::getArrayElementType()
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
