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
// modular_server.getFieldValue type must match the field default type
// modular_server.setFieldValue type must match the field default type
// modular_server.getFieldElementValue type must match the field array element default type
// modular_server.setFieldElementValue type must match the field array element default type

ModularDevice::ModularServer& modular_server = controller.getModularServer();

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
  // modular_server.getFieldValue type must match the field default type
  long starting_chars_count;
  modular_server.getFieldValue(constants::starting_chars_count_field_name,starting_chars_count);
  modular_server.writeResultToResponse(String(string).substring(0,starting_chars_count));
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

void testBoolCallback()
{
  bool test_bool;
  modular_server.getFieldValue(constants::test_bool_field_name,test_bool);
  modular_server.writeResultToResponse(test_bool);
}

void testLongArrayCallback()
{
  long test_long_array[constants::TEST_LONG_ARRAY_LENGTH];
  modular_server.getFieldValue(constants::test_long_array_field_name,test_long_array);
  modular_server.writeResultToResponse(test_long_array);
}

void testLongArrayVariableCallback()
{
  unsigned int array_length = modular_server.getFieldArrayLength(constants::test_long_array_field_name);
  long test_long_array[array_length];
  modular_server.getFieldValue(constants::test_long_array_field_name,test_long_array,array_length);
  modular_server.writeResultToResponse(test_long_array,array_length);
}
}
