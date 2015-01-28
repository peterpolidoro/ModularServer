// ----------------------------------------------------------------------------
// Callbacks.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Callbacks.h"

using namespace ArduinoJson::Parser;

namespace callbacks
{
// Callbacks must be non-blocking (avoid 'delay')
//
// modular_device.getParameterValue must be cast to either:
// char*
// long
// double
// JsonArray
// JsonObject
//
// For more info read about ArduinoJson v3 JsonParser JsonValues
//
// modular_device.getSavedVariableValue type must match the saved variable default type
// modular_device.setSavedVariableValue type must match the saved variable default type

void echoCallback()
{
  char* string = modular_device.getParameterValue(constants::string_parameter_name);
  modular_device.addToResponse("echo", string);
}

void lengthCallback()
{
  char* string = modular_device.getParameterValue(constants::string_parameter_name);
  modular_device.addToResponse("length", String(string).length());
}

void startsWithCallback()
{
  char* string = modular_device.getParameterValue(constants::string_parameter_name);
  char* string2 = modular_device.getParameterValue(constants::string2_parameter_name);
  modular_device.addBooleanToResponse("starts_with", String(string).startsWith(string2));
}

void repeatCallback()
{
  char* string = modular_device.getParameterValue(constants::string_parameter_name);
  long count = modular_device.getParameterValue(constants::count_parameter_name);
  modular_device.addKeyToResponse("strings");
  modular_device.startResponseArray();
  for (int i=0; i < count; i++)
  {
    modular_device.addToResponse(string);
  }
  modular_device.stopResponseArray();
}

void charsAtCallback()
{
  char* string = modular_device.getParameterValue(constants::string_parameter_name);
  JsonArray index_array = modular_device.getParameterValue(constants::index_array_parameter_name);
  modular_device.addKeyToResponse("result");
  modular_device.startResponseArray();
  for (JsonArrayIterator index_it=index_array.begin();
       index_it != index_array.end();
       ++index_it)
  {
    modular_device.startResponseObject();
    long index = *index_it;
    modular_device.addToResponse("index",index);
    char c = String(string).charAt(index);
    modular_device.addToResponse("char",c);
    modular_device.stopResponseObject();
  }
  modular_device.stopResponseArray();
}

void startingCharsCallback()
{
  char* string = modular_device.getParameterValue(constants::string_parameter_name);
  int starting_chars_count;
  // modular_device.getSavedVariableValue type must match the saved variable default type
  modular_device.getSavedVariableValue(constants::starting_chars_count_name,starting_chars_count);
  modular_device.addToResponse("starting_chars", String(string).substring(0,starting_chars_count));
}

void setStartingCharsCountCallback()
{
  // modular_device.setSavedVariableValue type must match the saved variable default type
  // modular_device.getParameterValue type might not match saved variable type
  // so recast type if necessary to make all types match
  int starting_chars_count = (long)modular_device.getParameterValue(constants::starting_chars_count_name);
  modular_device.setSavedVariableValue(constants::starting_chars_count_name,starting_chars_count);
}

void getStartingCharsCountCallback()
{
  int starting_chars_count;
  // modular_device.getSavedVariableValue type must match the saved variable default type
  modular_device.getSavedVariableValue(constants::starting_chars_count_name,starting_chars_count);
  modular_device.addToResponse("starting_chars_count",starting_chars_count);
}
}
