// ----------------------------------------------------------------------------
// StringController.cpp
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "StringController.h"


void StringController::setup()
{
  // Pin Setup

  // Add Server Streams
  modular_server_.addServerStream(Serial);

  // Set Device ID
  modular_server_.setDeviceName(constants::device_name);
  modular_server_.setFormFactor(constants::form_factor);

  // Add Hardware Info
  modular_server_.addHardwareInfo(constants::hardware_info);

  // Add Firmware
  modular_server_.addFirmware(constants::firmware_info,
                              fields_,
                              parameters_,
                              methods_);

  // Fields
  modular_server::Field & starting_chars_count_field = modular_server_.createField(constants::starting_chars_count_field_name,constants::starting_chars_count_default);
  starting_chars_count_field.setRange(constants::starting_chars_count_min,constants::starting_chars_count_max);

  modular_server::Field & stored_string_field = modular_server_.createField(constants::stored_string_field_name,constants::stored_string_default);

  // Parameters
  modular_server::Parameter & string_parameter = modular_server_.createParameter(constants::string_parameter_name);
  string_parameter.setTypeString();

  modular_server::Parameter & string2_parameter = modular_server_.copyParameter(string_parameter,constants::string2_parameter_name);

  modular_server::Parameter & count_parameter = modular_server_.createParameter(constants::count_parameter_name);
  count_parameter.setRange(constants::count_min,constants::count_max);

  modular_server::Parameter & index_array_parameter = modular_server_.createParameter(constants::index_array_parameter_name);
  index_array_parameter.setRange(constants::index_array_element_min,constants::index_array_element_max);
  index_array_parameter.setArrayLengthRange(constants::index_array_length_min,constants::index_array_length_max);

  modular_server::Parameter & double_echo_parameter = modular_server_.createParameter(constants::double_echo_parameter_name);
  double_echo_parameter.setTypeBool();

  // Methods
  modular_server::Method & echo_method = modular_server_.createMethod(constants::echo_method_name);
  echo_method.attachCallback(makeFunctor((Functor0 *)0,*this,&StringController::echoCallback));
  echo_method.addParameter(string_parameter);
  echo_method.addParameter(double_echo_parameter);
  echo_method.setReturnTypeString();

  modular_server::Method & length_method = modular_server_.createMethod(constants::length_method_name);
  length_method.attachCallback(makeFunctor((Functor0 *)0,*this,&StringController::lengthCallback));
  length_method.addParameter(string_parameter);
  length_method.setReturnTypeLong();

  modular_server::Method & starts_with_method = modular_server_.createMethod(constants::starts_with_method_name);
  starts_with_method.attachCallback(makeFunctor((Functor0 *)0,*this,&StringController::startsWithCallback));
  starts_with_method.addParameter(string_parameter);
  starts_with_method.addParameter(string2_parameter);
  starts_with_method.setReturnTypeBool();

  modular_server::Method & repeat_method = modular_server_.createMethod(constants::repeat_method_name);
  repeat_method.attachCallback(makeFunctor((Functor0 *)0,*this,&StringController::repeatCallback));
  repeat_method.addParameter(string_parameter);
  repeat_method.addParameter(count_parameter);
  repeat_method.setReturnTypeArray();

  modular_server::Method & chars_at_method = modular_server_.createMethod(constants::chars_at_method_name);
  chars_at_method.attachCallback(makeFunctor((Functor0 *)0,*this,&StringController::charsAtCallback));
  chars_at_method.addParameter(string_parameter);
  chars_at_method.addParameter(index_array_parameter);
  chars_at_method.setReturnTypeArray();

  modular_server::Method & starting_chars_method = modular_server_.createMethod(constants::starting_chars_method_name);
  starting_chars_method.attachCallback(makeFunctor((Functor0 *)0,*this,&StringController::startingCharsCallback));
  starting_chars_method.addParameter(string_parameter);
  starting_chars_method.setReturnTypeString();

  modular_server::Method & set_stored_string_method = modular_server_.createMethod(constants::set_stored_string_method_name);
  set_stored_string_method.attachCallback(makeFunctor((Functor0 *)0,*this,&StringController::setStoredStringCallback));
  set_stored_string_method.addParameter(string_parameter);

  modular_server::Method & get_stored_string_method = modular_server_.createMethod(constants::get_stored_string_method_name);
  get_stored_string_method.attachCallback(makeFunctor((Functor0 *)0,*this,&StringController::getStoredStringCallback));
  get_stored_string_method.setReturnTypeString();

  // Begin Streams
  Serial.begin(constants::baudrate);

  // Start Modular Device Server
  modular_server_.startServer();
}

void StringController::update()
{
  modular_server_.handleServerRequests();
}

// Callbacks must be non-blocking (avoid 'delay')
//
// modular_server_.parameter(parameter_name).getValue(value) value type must be either:
// fixed-point number (int, long, etc.)
// floating-point number (float, double)
// bool
// const char *
// ArduinoJson::JsonArray *
// ArduinoJson::JsonObject *
//
// For more info read about ArduinoJson parsing https://github.com/janelia-arduino/ArduinoJson
//
// modular_server_.field(field_name).getValue(value) value type must match the field default type
// modular_server_.field(field_name).setValue(value) value type must match the field default type
// modular_server_.field(field_name).getElementValue(value) value type must match the field array element default type
// modular_server_.field(field_name).setElementValue(value) value type must match the field array element default type

void StringController::echoCallback()
{
  const char * string;
  modular_server_.parameter(constants::string_parameter_name).getValue(string);
  bool double_echo;
  modular_server_.parameter(constants::double_echo_parameter_name).getValue(double_echo);
  if (!double_echo)
  {
    modular_server_.writeResultToResponse(string);
  }
  else
  {
    String echo = String(string) + String(string);
    modular_server_.writeResultToResponse(echo);
  }
}

void StringController::lengthCallback()
{
  const char * string;
  modular_server_.parameter(constants::string_parameter_name).getValue(string);
  modular_server_.writeResultToResponse(strlen(string));
}

void StringController::startsWithCallback()
{
  const char * string;
  modular_server_.parameter(constants::string_parameter_name).getValue(string);
  const char * string2;
  modular_server_.parameter(constants::string2_parameter_name).getValue(string2);
  modular_server_.writeResultToResponse((bool)String(string).startsWith(string2));
}

void StringController::repeatCallback()
{
  const char * string;
  modular_server_.parameter(constants::string_parameter_name).getValue(string);
  long count;
  modular_server_.parameter(constants::count_parameter_name).getValue(count);
  modular_server_.writeResultKeyToResponse();
  modular_server_.beginResponseArray();
  for (int i=0; i < count; i++)
  {
    modular_server_.writeToResponse(string);
  }
  modular_server_.endResponseArray();
}

void StringController::charsAtCallback()
{
  const char * string;
  modular_server_.parameter(constants::string_parameter_name).getValue(string);
  ArduinoJson::JsonArray * index_array_ptr;
  modular_server_.parameter(constants::index_array_parameter_name).getValue(index_array_ptr);
  for (ArduinoJson::JsonArray::iterator it=index_array_ptr->begin();
       it != index_array_ptr->end();
       ++it)
  {
    long index = *it;
    if (index >= String(string).length())
    {
      modular_server_.sendErrorResponse(constants::index_error);
      return;
    }
  }
  modular_server_.writeResultKeyToResponse();
  modular_server_.beginResponseArray();
  for (ArduinoJson::JsonArray::iterator it=index_array_ptr->begin();
       it != index_array_ptr->end();
       ++it)
  {
    modular_server_.beginResponseObject();
    long index = *it;
    modular_server_.writeToResponse("index",index);
    char c = string[index];
    modular_server_.writeToResponse("char",c);
    modular_server_.endResponseObject();
  }
  modular_server_.endResponseArray();
}

void StringController::startingCharsCallback()
{
  const char * string;
  modular_server_.parameter(constants::string_parameter_name).getValue(string);
  long starting_chars_count;
  modular_server_.field(constants::starting_chars_count_field_name).getValue(starting_chars_count);
  modular_server_.writeResultToResponse(String(string).substring(0,starting_chars_count));
}

void StringController::setStoredStringCallback()
{
  const char * string;
  modular_server_.parameter(constants::string_parameter_name).getValue(string);
  size_t array_length = strlen(string) + 1;
  modular_server_.field(constants::stored_string_field_name).setValue(string,array_length);
}

void StringController::getStoredStringCallback()
{
  modular_server::Field & field = modular_server_.field(constants::stored_string_field_name);
  size_t array_length = field.getArrayLength();
  char stored_string[array_length];
  field.getValue(stored_string,array_length);
  modular_server_.writeResultToResponse(stored_string);
}
