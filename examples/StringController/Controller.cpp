// ----------------------------------------------------------------------------
// Controller.cpp
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Controller.h"


Controller::Controller()
{
}

void Controller::setup()
{
  // Pin Setup

  // Device Info
  modular_server_.setDeviceName(constants::device_name);
  modular_server_.setModelNumber(constants::model_number);
  modular_server_.setFirmwareName(constants::firmware_name);
  modular_server_.setFirmwareVersion(constants::firmware_major,constants::firmware_minor,constants::firmware_patch);
  modular_server_.setHardwareName(constants::hardware_name);
  modular_server_.setHardwareVersion(constants::hardware_major,constants::hardware_minor);

  // Add Server Streams
  modular_server_.addServerStream(Serial);

  // Set Storage
  modular_server_.setFieldStorage(fields_);
  modular_server_.setParameterStorage(parameters_);
  modular_server_.setMethodStorage(methods_);

  // Fields
  ModularDevice::Field & starting_chars_count_field = modular_server_.createField(constants::starting_chars_count_field_name,constants::starting_chars_count_default);
  starting_chars_count_field.setRange(constants::starting_chars_count_min,constants::starting_chars_count_max);

  ModularDevice::Field & stored_string_field = modular_server_.createField(constants::stored_string_field_name,constants::stored_string_default);

  // Parameters
  ModularDevice::Parameter & string_parameter = modular_server_.createParameter(constants::string_parameter_name);
  string_parameter.setTypeString();

  ModularDevice::Parameter & string2_parameter = modular_server_.copyParameter(string_parameter,constants::string2_parameter_name);

  ModularDevice::Parameter & count_parameter = modular_server_.createParameter(constants::count_parameter_name);
  count_parameter.setRange(constants::count_min,constants::count_max);

  ModularDevice::Parameter & index_array_parameter = modular_server_.createParameter(constants::index_array_parameter_name);
  index_array_parameter.setRange(constants::index_array_element_min,constants::index_array_element_max);
  index_array_parameter.setArrayLengthRange(constants::index_array_length_min,constants::index_array_length_max);

  ModularDevice::Parameter & double_echo_parameter = modular_server_.createParameter(constants::double_echo_parameter_name);
  double_echo_parameter.setTypeBool();

  // Methods
  ModularDevice::Method & echo_method = modular_server_.createMethod(constants::echo_method_name);
  echo_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::echoCallback));
  echo_method.addParameter(string_parameter);
  echo_method.addParameter(double_echo_parameter);
  echo_method.setReturnTypeString();

  ModularDevice::Method & length_method = modular_server_.createMethod(constants::length_method_name);
  length_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::lengthCallback));
  length_method.addParameter(string_parameter);
  length_method.setReturnTypeLong();

  ModularDevice::Method & starts_with_method = modular_server_.createMethod(constants::starts_with_method_name);
  starts_with_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::startsWithCallback));
  starts_with_method.addParameter(string_parameter);
  starts_with_method.addParameter(string2_parameter);
  starts_with_method.setReturnTypeBool();

  ModularDevice::Method & repeat_method = modular_server_.createMethod(constants::repeat_method_name);
  repeat_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::repeatCallback));
  repeat_method.addParameter(string_parameter);
  repeat_method.addParameter(count_parameter);
  repeat_method.setReturnTypeArray();

  ModularDevice::Method & chars_at_method = modular_server_.createMethod(constants::chars_at_method_name);
  chars_at_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::charsAtCallback));
  chars_at_method.addParameter(string_parameter);
  chars_at_method.addParameter(index_array_parameter);
  chars_at_method.setReturnTypeArray();

  ModularDevice::Method & starting_chars_method = modular_server_.createMethod(constants::starting_chars_method_name);
  starting_chars_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::startingCharsCallback));
  starting_chars_method.addParameter(string_parameter);
  starting_chars_method.setReturnTypeString();

  ModularDevice::Method & set_stored_string_method = modular_server_.createMethod(constants::set_stored_string_method_name);
  set_stored_string_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::setStoredStringCallback));
  set_stored_string_method.addParameter(string_parameter);

  ModularDevice::Method & get_stored_string_method = modular_server_.createMethod(constants::get_stored_string_method_name);
  get_stored_string_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::getStoredStringCallback));
  get_stored_string_method.setReturnTypeString();

  // Setup Streams
  Serial.begin(constants::baudrate);

  // Start Modular Device Server
  modular_server_.startServer();
}

void Controller::update()
{
  modular_server_.handleServerRequests();
}

// Callbacks must be non-blocking (avoid 'delay')
//
// modular_server_.getParameterValue must be cast to either:
// const char *
// long
// double
// bool
// ArduinoJson::JsonArray &
// ArduinoJson::JsonObject &
//
// For more info read about ArduinoJson parsing https://github.com/janelia-arduino/ArduinoJson
//
// modular_server_.getFieldValue type must match the field default type
// modular_server_.setFieldValue type must match the field default type
// modular_server_.getFieldElementValue type must match the field array element default type
// modular_server_.setFieldElementValue type must match the field array element default type

void Controller::echoCallback()
{
  const char * string = modular_server_.getParameterValue(constants::string_parameter_name);
  bool double_echo = modular_server_.getParameterValue(constants::double_echo_parameter_name);
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

void Controller::lengthCallback()
{
  const char * string = modular_server_.getParameterValue(constants::string_parameter_name);
  modular_server_.writeResultToResponse(strlen(string));
}

void Controller::startsWithCallback()
{
  const char * string = modular_server_.getParameterValue(constants::string_parameter_name);
  const char * string2 = modular_server_.getParameterValue(constants::string2_parameter_name);
  modular_server_.writeResultToResponse((bool)String(string).startsWith(string2));
}

void Controller::repeatCallback()
{
  const char * string = modular_server_.getParameterValue(constants::string_parameter_name);
  long count = modular_server_.getParameterValue(constants::count_parameter_name);
  modular_server_.writeResultKeyToResponse();
  modular_server_.beginResponseArray();
  for (int i=0; i < count; i++)
  {
    modular_server_.writeToResponse(string);
  }
  modular_server_.endResponseArray();
}

void Controller::charsAtCallback()
{
  const char * string = modular_server_.getParameterValue(constants::string_parameter_name);
  ArduinoJson::JsonArray & index_array = modular_server_.getParameterValue(constants::index_array_parameter_name);
  for (ArduinoJson::JsonArray::iterator it=index_array.begin();
       it != index_array.end();
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
  for (ArduinoJson::JsonArray::iterator it=index_array.begin();
       it != index_array.end();
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

void Controller::startingCharsCallback()
{
  const char * string = modular_server_.getParameterValue(constants::string_parameter_name);
  // modular_server_.getFieldValue type must match the field default type
  long starting_chars_count;
  modular_server_.getFieldValue(constants::starting_chars_count_field_name,starting_chars_count);
  modular_server_.writeResultToResponse(String(string).substring(0,starting_chars_count));
}

void Controller::setStoredStringCallback()
{
  const char * string = modular_server_.getParameterValue(constants::string_parameter_name);
  size_t array_length = strlen(string) + 1;
  modular_server_.setFieldValue(constants::stored_string_field_name,string,array_length);
}

void Controller::getStoredStringCallback()
{
  size_t array_length = modular_server_.getFieldArrayLength(constants::stored_string_field_name);
  char stored_string[array_length];
  modular_server_.getFieldValue(constants::stored_string_field_name,stored_string,array_length);
  modular_server_.writeResultToResponse(stored_string);
}
