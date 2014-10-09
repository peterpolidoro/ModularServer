// ----------------------------------------------------------------------------
// Parameter.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Parameter.h"

Parameter::Parameter(char *name="")
{
  setName(name);
}

void Parameter::setName(char *name)
{
  strncpy(name_,name,PARAMETER_NAME_LENGTH_MAX);
}

void Parameter::setUnits(char *units)
{
  strncpy(units_,units,PARAMETER_UNITS_LENGTH_MAX);
}

boolean Parameter::compareName(char *name_to_compare)
{
  return String(name_).equalsIgnoreCase(name_to_compare);
}

char* Parameter::getName()
{
  return name_;
}
