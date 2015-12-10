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
// modular_server.getParameterValue must be cast to either:
// const char*
// long
// double
// bool
// ArduinoJson::JsonArray&
// ArduinoJson::JsonObject&
//
// For more info read about ArduinoJson parsing https://github.com/janelia-arduino/ArduinoJson
//
// modular_server.getSavedVariableValue type must match the saved variable default type
// modular_server.setSavedVariableValue type must match the saved variable default type

void echoCallback()
{
  const char* string = modular_server.getParameterValue(constants::string_parameter_name);
  bool double_echo = modular_server.getParameterValue(constants::double_echo_parameter_name);
  if (!double_echo)
  {
    modular_server.writeResultToResponse(string);
  }
  else
  {
    String echo = String(string) + String(string);
    modular_server.writeResultToResponse(echo);
  }
}

void lengthCallback()
{
  const char* string = modular_server.getParameterValue(constants::string_parameter_name);
  modular_server.writeResultToResponse(strlen(string));
}

void startsWithCallback()
{
  const char* string = modular_server.getParameterValue(constants::string_parameter_name);
  const char* string2 = modular_server.getParameterValue(constants::string2_parameter_name);
  modular_server.writeResultToResponse((bool)String(string).startsWith(string2));
}

void repeatCallback()
{
  const char* string = modular_server.getParameterValue(constants::string_parameter_name);
  long count = modular_server.getParameterValue(constants::count_parameter_name);
  modular_server.writeResultKeyToResponse();
  modular_server.beginResponseArray();
  for (int i=0; i < count; i++)
  {
    modular_server.writeToResponse(string);
  }
  modular_server.endResponseArray();
}

void charsAtCallback()
{
  const char* string = modular_server.getParameterValue(constants::string_parameter_name);
  ArduinoJson::JsonArray& index_array = modular_server.getParameterValue(constants::index_array_parameter_name);
  for (ArduinoJson::JsonArray::iterator it=index_array.begin();
       it != index_array.end();
       ++it)
  {
    long index = *it;
    if (index >= String(string).length())
    {
      modular_server.sendErrorResponse(constants::index_error);
      return;
    }
  }
  modular_server.writeResultKeyToResponse();
  modular_server.beginResponseArray();
  for (ArduinoJson::JsonArray::iterator it=index_array.begin();
       it != index_array.end();
       ++it)
  {
    modular_server.beginResponseObject();
    long index = *it;
    modular_server.writeToResponse("index",index);
    char c = string[index];
    modular_server.writeToResponse("char",c);
    modular_server.endResponseObject();
  }
  modular_server.endResponseArray();
}

void startingCharsCallback()
{
  const char* string = modular_server.getParameterValue(constants::string_parameter_name);
  int starting_chars_count;
  // modular_server.getSavedVariableValue type must match the saved variable default type
  modular_server.getSavedVariableValue(constants::starting_chars_count_name,starting_chars_count);
  modular_server.writeResultToResponse(String(string).substring(0,starting_chars_count));
}

void setStartingCharsCountCallback()
{
  // modular_server.setSavedVariableValue type must match the saved variable default type
  // modular_server.getParameterValue type might not match saved variable type
  // so recast type if necessary to make all types match
  int starting_chars_count = (long)modular_server.getParameterValue(constants::starting_chars_count_name);
  modular_server.setSavedVariableValue(constants::starting_chars_count_name,starting_chars_count);
}

void getStartingCharsCountCallback()
{
  int starting_chars_count;
  // modular_server.getSavedVariableValue type must match the saved variable default type
  modular_server.getSavedVariableValue(constants::starting_chars_count_name,starting_chars_count);
  modular_server.writeResultToResponse(starting_chars_count);
}

void setStoredStringCallback()
{
  const char* string = modular_server.getParameterValue(constants::string_parameter_name);
  controller.setStoredString(String(string));
}

void getStoredStringCallback()
{
  modular_server.writeResultToResponse(controller.getStoredString());
}
}
