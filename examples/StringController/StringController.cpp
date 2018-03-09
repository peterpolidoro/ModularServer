// ----------------------------------------------------------------------------
// StringController.cpp
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "StringController.h"


void StringController::setup()
{
  // Server Setup
  modular_server_.setup();

  // Pin Setup

  // Add Server Streams
  modular_server_.addServerStream(Serial);

  // Set Device ID
  modular_server_.setDeviceName(constants::device_name);
  modular_server_.setFormFactor(constants::form_factor);

  // Add Hardware
  modular_server_.addHardware(constants::hardware_info,
                              interrupts_);

  // Interrupts

  // Add Firmware
  modular_server_.addFirmware(constants::firmware_info,
                              properties_,
                              parameters_,
                              functions_,
                              callbacks_);

  // Properties
  modular_server::Property & serial_number_property = modular_server_.property(modular_server::constants::serial_number_property_name);
  serial_number_property.setDefaultValue(constants::serial_number_default_new);

  modular_server::Property & starting_chars_count_property = modular_server_.createProperty(constants::starting_chars_count_property_name,constants::starting_chars_count_default);
  starting_chars_count_property.setRange(constants::starting_chars_count_min,constants::starting_chars_count_max);

  modular_server_.createProperty(constants::stored_string_property_name,constants::stored_string_default);

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

  // Functions
  modular_server::Function & echo_function = modular_server_.createFunction(constants::echo_function_name);
  echo_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&StringController::echoHandler));
  echo_function.addParameter(string_parameter);
  echo_function.addParameter(double_echo_parameter);
  echo_function.setResultTypeString();

  modular_server::Function & length_function = modular_server_.createFunction(constants::length_function_name);
  length_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&StringController::lengthHandler));
  length_function.addParameter(string_parameter);
  length_function.setResultTypeLong();
  length_function.setResultUnits(constants::characters_units);

  modular_server::Function & starts_with_function = modular_server_.createFunction(constants::starts_with_function_name);
  starts_with_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&StringController::startsWithHandler));
  starts_with_function.addParameter(string_parameter);
  starts_with_function.addParameter(string2_parameter);
  starts_with_function.setResultTypeBool();

  modular_server::Function & repeat_function = modular_server_.createFunction(constants::repeat_function_name);
  repeat_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&StringController::repeatHandler));
  repeat_function.addParameter(string_parameter);
  repeat_function.addParameter(count_parameter);
  repeat_function.setResultTypeArray();
  repeat_function.setResultTypeString();

  modular_server::Function & chars_at_function = modular_server_.createFunction(constants::chars_at_function_name);
  chars_at_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&StringController::charsAtHandler));
  chars_at_function.addParameter(string_parameter);
  chars_at_function.addParameter(index_array_parameter);
  chars_at_function.setResultTypeArray();
  chars_at_function.setResultTypeString();

  modular_server::Function & starting_chars_function = modular_server_.createFunction(constants::starting_chars_function_name);
  starting_chars_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&StringController::startingCharsHandler));
  starting_chars_function.addParameter(string_parameter);
  starting_chars_function.setResultTypeString();

  modular_server::Function & set_stored_string_function = modular_server_.createFunction(constants::set_stored_string_function_name);
  set_stored_string_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&StringController::setStoredStringHandler));
  set_stored_string_function.addParameter(string_parameter);

  modular_server::Function & get_stored_string_function = modular_server_.createFunction(constants::get_stored_string_function_name);
  get_stored_string_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&StringController::getStoredStringHandler));
  get_stored_string_function.setResultTypeString();

  // Callbacks

  // Begin Streams
  Serial.begin(constants::baud);

  // Start Modular Device Server
  modular_server_.startServer();
}

void StringController::update()
{
  modular_server_.handleServerRequests();
}

// Handlers must be non-blocking (avoid 'delay')
//
// modular_server_.parameter(parameter_name).getValue(value) value type must be either:
// fixed-point number (int, long, etc.)
// floating-point number (float, double)
// bool
// const char *
// ArduinoJson::JsonArray *
// ArduinoJson::JsonObject *
// const ConstantString *
//
// For more info read about ArduinoJson parsing https://github.com/janelia-arduino/ArduinoJson
//
// modular_server_.property(property_name).getValue(value) value type must match the property default type
// modular_server_.property(property_name).setValue(value) value type must match the property default type
// modular_server_.property(property_name).getElementValue(element_index,value) value type must match the property array element default type
// modular_server_.property(property_name).setElementValue(element_index,value) value type must match the property array element default type

void StringController::echoHandler()
{
  const char * string;
  modular_server_.parameter(constants::string_parameter_name).getValue(string);
  bool double_echo;
  modular_server_.parameter(constants::double_echo_parameter_name).getValue(double_echo);
  modular_server::Response & response = modular_server_.response();
  if (!double_echo)
  {
    response.returnResult(string);
  }
  else
  {
    String echo = String(string) + String(string);
    response.returnResult(echo);
  }
}

void StringController::lengthHandler()
{
  const char * string;
  modular_server_.parameter(constants::string_parameter_name).getValue(string);
  modular_server_.response().returnResult(strlen(string));
}

void StringController::startsWithHandler()
{
  const char * string;
  modular_server_.parameter(constants::string_parameter_name).getValue(string);
  const char * string2;
  modular_server_.parameter(constants::string2_parameter_name).getValue(string2);
  modular_server_.response().returnResult((bool)String(string).startsWith(string2));
}

void StringController::repeatHandler()
{
  const char * string;
  modular_server_.parameter(constants::string_parameter_name).getValue(string);
  long count;
  modular_server_.parameter(constants::count_parameter_name).getValue(count);
  modular_server::Response & response = modular_server_.response();
  response.writeResultKey();
  response.beginArray();
  for (int i=0; i < count; i++)
  {
    response.write(string);
  }
  response.endArray();
}

void StringController::charsAtHandler()
{
  const char * string;
  modular_server_.parameter(constants::string_parameter_name).getValue(string);
  ArduinoJson::JsonArray * index_array_ptr;
  modular_server_.parameter(constants::index_array_parameter_name).getValue(index_array_ptr);
  modular_server::Response & response = modular_server_.response();
  for (ArduinoJson::JsonArray::iterator it=index_array_ptr->begin();
       it != index_array_ptr->end();
       ++it)
  {
    size_t index = *it;
    if (index >= String(string).length())
    {
      response.returnError(constants::index_error);
      return;
    }
  }
  response.writeResultKey();
  response.beginArray();
  for (ArduinoJson::JsonArray::iterator it=index_array_ptr->begin();
       it != index_array_ptr->end();
       ++it)
  {
    response.beginObject();
    long index = *it;
    response.write("index",index);
    char c = string[index];
    response.write("char",c);
    response.endObject();
  }
  response.endArray();
}

void StringController::startingCharsHandler()
{
  const char * string;
  modular_server_.parameter(constants::string_parameter_name).getValue(string);
  long starting_chars_count;
  modular_server_.property(constants::starting_chars_count_property_name).getValue(starting_chars_count);
  modular_server_.response().returnResult(String(string).substring(0,starting_chars_count));
}

void StringController::setStoredStringHandler()
{
  const char * string;
  modular_server_.parameter(constants::string_parameter_name).getValue(string);
  size_t array_length = strlen(string) + 1;
  modular_server_.property(constants::stored_string_property_name).setValue(string,array_length);
}

void StringController::getStoredStringHandler()
{
  modular_server::Property & property = modular_server_.property(constants::stored_string_property_name);
  size_t array_length = property.getArrayLength();
  char stored_string[array_length];
  property.getValue(stored_string,array_length);
  modular_server_.response().returnResult(stored_string);
}
