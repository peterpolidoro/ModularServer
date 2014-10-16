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

// Generator::JsonObject<JSON_OBJECT_SIZE_PARAMETER_HELP> Parameter::help()
// {
//   help_json_object = Generator::JsonObject<JSON_OBJECT_SIZE_PARAMETER_HELP>();
//   help_json_object["name"] = getName();
//   if (strcmp(units_,"") != 0)
//   {
//     help_json_object["units"] = units_;
//   }
//   return help_json_object;
//   // char help_string[STRING_LENGTH_PARAMETER_HELP];
//   // help_json_object.printTo(help_string,STRING_LENGTH_PARAMETER_HELP);
//   // String help = getName();
//   // if (strcmp(units_,"") != 0){
//   //   help += String(" (") + String(units_) + String(")");
//   // }
//   // char help_string[STRING_LENGTH_PARAMETER_HELP];
//   // help.toCharArray(help_string,STRING_LENGTH_PARAMETER_HELP);
//   // return help_string;
// }
