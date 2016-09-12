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

void getDoubledCallback()
{
  double value;
  modular_server.getFieldValue(constants::double_field_name,value);
  value *= 2;
  modular_server.writeResultToResponse(value);
}

void getBoolCallback()
{
  bool value;
  modular_server.getFieldValue(constants::bool_field_name,value);
  modular_server.writeResultToResponse(value);
}

void getLongArrayFixedCallback()
{
  long long_array[constants::LONG_ARRAY_LENGTH];
  modular_server.getFieldValue(constants::long_array_field_name,long_array);
  modular_server.writeResultToResponse(long_array);
}

void getLongArrayVariableCallback()
{
  size_t array_length = modular_server.getFieldArrayLength(constants::long_array_field_name);
  long long_array[array_length];
  modular_server.getFieldValue(constants::long_array_field_name,long_array,array_length);
  modular_server.writeResultToResponse(long_array,array_length);
}

void setLongArrayFixedCallback()
{
  long long_array[constants::LONG_ARRAY_LENGTH];
  long_array[0] = 1;
  long_array[1] = 2;
  long_array[2] = 102;
  long_array[3] = 103;
  bool success = modular_server.setFieldValue(constants::long_array_field_name,long_array);
  modular_server.writeResultToResponse(success);
}

void setLongArrayVariableCallback()
{
  size_t array_length = modular_server.getFieldArrayLength(constants::long_array_field_name);
  long long_array[array_length-1];
  long_array[0] = -1;
  long_array[1] = -2;
  long_array[2] = 9;
  bool success = modular_server.setFieldValue(constants::long_array_field_name,long_array,array_length-1);
  modular_server.writeResultToResponse(success);
}

void setLongArrayParameterCallback()
{
  ArduinoJson::JsonArray& long_array = modular_server.getParameterValue(constants::long_array_parameter_name);
  bool success = modular_server.setFieldValue(constants::long_array_field_name,long_array);
  modular_server.writeResultToResponse(success);
}

void getStringAllCallback()
{
  size_t array_length = modular_server.getFieldArrayLength(constants::string_field_name);
  char string[array_length];
  modular_server.getFieldValue(constants::string_field_name,string,array_length);
  modular_server.writeResultToResponse(string);
}

void getStringSomeCallback()
{
  long length = modular_server.getParameterValue(constants::length_parameter_name);
  size_t array_length = length + 1;
  char string[array_length];
  modular_server.getFieldValue(constants::string_field_name,string,array_length);
  modular_server.writeResultToResponse(string);
}

void getCountCallback()
{
  long count = modular_server.getParameterValue(constants::count_parameter_name);
  switch (count)
  {
    case 10:
      modular_server.writeResultToResponse("ten");
      break;
    case 20:
      modular_server.writeResultToResponse("twenty");
      break;
    case 30:
      modular_server.writeResultToResponse("thirty");
      break;
  }
}

}
