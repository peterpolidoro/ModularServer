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
}

void Parameter::setName(char *name)
{
  strncpy(name_,name,STRING_LENGTH_PARAMETER_NAME);
}

void Parameter::setUnits(char *units)
{
  strncpy(units_,units,STRING_LENGTH_PARAMETER_UNITS);
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
