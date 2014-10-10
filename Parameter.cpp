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
  setUnits("");
}

void Parameter::setName(char *name)
{
  strncpy(name_,name,PARAMETER_NAME_STRING_LENGTH_MAX);
}

void Parameter::setUnits(char *units)
{
  strncpy(units_,units,PARAMETER_UNITS_STRING_LENGTH_MAX);
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

// Generator::JsonObject<PARAMETER_HELP_JSON_OBJECT_SIZE> Parameter::help()
// {
//   help_json_object = Generator::JsonObject<PARAMETER_HELP_JSON_OBJECT_SIZE>();
//   help_json_object["name"] = getName();
//   if (strcmp(units_,"") != 0)
//   {
//     help_json_object["units"] = units_;
//   }
//   return help_json_object;
//   // char help_string[PARAMETER_HELP_STRING_LENGTH_MAX];
//   // help_json_object.printTo(help_string,PARAMETER_HELP_STRING_LENGTH_MAX);
//   // String help = getName();
//   // if (strcmp(units_,"") != 0){
//   //   help += String(" (") + String(units_) + String(")");
//   // }
//   // char help_string[PARAMETER_HELP_STRING_LENGTH_MAX];
//   // help.toCharArray(help_string,PARAMETER_HELP_STRING_LENGTH_MAX);
//   // return help_string;
// }
