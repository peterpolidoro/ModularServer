// ----------------------------------------------------------------------------
// Parameter.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Parameter.h"

using namespace ArduinoJson;

Parameter::Parameter(char *name="")
{
  setName(name);
  setUnits("");
  setTypeLong();
  range_is_set_ = false;
}

void Parameter::setName(char *name)
{
  strncpy(name_,name,STRING_LENGTH_PARAMETER_NAME);
}

void Parameter::setUnits(char *units)
{
  strncpy(units_,units,STRING_LENGTH_PARAMETER_UNITS);
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
  return String(name_).equalsIgnoreCase(name_to_compare);
}

char* Parameter::getName()
{
  return name_;
}

char* Parameter::getUnits()
{
  return units_;
}
