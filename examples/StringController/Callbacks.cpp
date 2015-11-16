// ----------------------------------------------------------------------------
// Callbacks.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Callbacks.h"


namespace callbacks
{
// Callbacks must be non-blocking (avoid 'delay')
//
// modular_device.getParameterValue must be cast to either:
// const char*
// long
// double
// bool
// ArduinoJson::JsonArray&
// ArduinoJson::JsonObject&
//
// For more info read about ArduinoJson parsing https://github.com/janelia-arduino/ArduinoJson
//
// modular_device.getSavedVariableValue type must match the saved variable default type
// modular_device.setSavedVariableValue type must match the saved variable default type

void echoCallback()
{
  const char* string = modular_device.getParameterValue(constants::string_parameter_name);
  bool double_echo = modular_device.getParameterValue(constants::double_echo_parameter_name);
  if (!double_echo)
  {
    modular_device.addResultToResponse(string);
  }
  else
  {
    String echo = String(string) + String(string);
    modular_device.addResultToResponse(echo);
  }
}

void lengthCallback()
{
  const char* string = modular_device.getParameterValue(constants::string_parameter_name);
  modular_device.addResultToResponse(strlen(string));
}

void startsWithCallback()
{
  const char* string = modular_device.getParameterValue(constants::string_parameter_name);
  const char* string2 = modular_device.getParameterValue(constants::string2_parameter_name);
  modular_device.addResultToResponse((bool)String(string).startsWith(string2));
}

void repeatCallback()
{
  const char* string = modular_device.getParameterValue(constants::string_parameter_name);
  long count = modular_device.getParameterValue(constants::count_parameter_name);
  modular_device.addResultKeyToResponse();
  modular_device.startResponseArray();
  for (int i=0; i < count; i++)
  {
    modular_device.addToResponse(string);
  }
  modular_device.stopResponseArray();
}

void charsAtCallback()
{
  const char* string = modular_device.getParameterValue(constants::string_parameter_name);
  ArduinoJson::JsonArray& index_array = modular_device.getParameterValue(constants::index_array_parameter_name);
  for (ArduinoJson::JsonArray::iterator it=index_array.begin();
       it != index_array.end();
       ++it)
  {
    long index = *it;
    if (index >= String(string).length())
    {
      modular_device.sendErrorResponse(constants::index_error);
      return;
    }
  }
  modular_device.addResultKeyToResponse();
  modular_device.startResponseArray();
  for (ArduinoJson::JsonArray::iterator it=index_array.begin();
       it != index_array.end();
       ++it)
  {
    modular_device.startResponseObject();
    long index = *it;
    modular_device.addToResponse("index",index);
    char c = string[index];
    modular_device.addToResponse("char",c);
    modular_device.stopResponseObject();
  }
  modular_device.stopResponseArray();
}

void startingCharsCallback()
{
  const char* string = modular_device.getParameterValue(constants::string_parameter_name);
  int starting_chars_count;
  // modular_device.getSavedVariableValue type must match the saved variable default type
  modular_device.getSavedVariableValue(constants::starting_chars_count_name,starting_chars_count);
  modular_device.addResultToResponse(String(string).substring(0,starting_chars_count));
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
  modular_device.addResultToResponse(starting_chars_count);
}

void setStoredStringCallback()
{
  const char* string = modular_device.getParameterValue(constants::string_parameter_name);
  controller.setStoredString(String(string));
}

void getStoredStringCallback()
{
  modular_device.addResultToResponse(controller.getStoredString());
}
}
