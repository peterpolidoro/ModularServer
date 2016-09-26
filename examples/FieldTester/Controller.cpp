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
  ModularDevice::Field & double_field = modular_server_.createField(constants::double_field_name,constants::double_default);

  ModularDevice::Field & bool_field = modular_server_.createField(constants::bool_field_name,constants::bool_default);
  bool_field.attachPreSetValueCallback(makeFunctor((Functor0 *)0,*this,&Controller::preSetFieldValueCallback));
  bool_field.attachPostSetValueCallback(makeFunctor((Functor0 *)0,*this,&Controller::postSetFieldValueCallback));

  ModularDevice::Field & long_array_field = modular_server_.createField(constants::long_array_field_name,constants::long_array_default);
  long_array_field.setRange(constants::long_array_element_min,constants::long_array_element_max);

  ModularDevice::Field & double_array_field = modular_server_.createField(constants::double_array_field_name,constants::double_array_default);
  double_array_field.setRange(constants::double_array_element_min,constants::double_array_element_max);

  ModularDevice::Field & bool_array_field = modular_server_.createField(constants::bool_array_field_name,constants::bool_array_default);
  bool_array_field.attachPreSetValueCallback(makeFunctor((Functor0 *)0,*this,&Controller::preSetFieldValueCallback));
  bool_array_field.attachPostSetValueCallback(makeFunctor((Functor0 *)0,*this,&Controller::postSetFieldValueCallback));
  bool_array_field.attachPreSetElementValueCallback(makeFunctor((Functor1<const size_t> *)0,*this,&Controller::preSetFieldElementValueCallback));
  bool_array_field.attachPostSetElementValueCallback(makeFunctor((Functor1<const size_t> *)0,*this,&Controller::postSetFieldElementValueCallback));

  ModularDevice::Field & string_field = modular_server_.createField(constants::string_field_name,constants::string_default);
  string_field.attachPreSetValueCallback(makeFunctor((Functor0 *)0,*this,&Controller::preSetFieldValueCallback));
  string_field.attachPostSetValueCallback(makeFunctor((Functor0 *)0,*this,&Controller::postSetFieldValueCallback));
  string_field.attachPreSetElementValueCallback(makeFunctor((Functor1<const size_t> *)0,*this,&Controller::preSetFieldElementValueCallback));
  string_field.attachPostSetElementValueCallback(makeFunctor((Functor1<const size_t> *)0,*this,&Controller::postSetFieldElementValueCallback));

  ModularDevice::Field & odd_field = modular_server_.createField(constants::odd_field_name,constants::odd_default);
  odd_field.setSubset(constants::odd_subset);

  ModularDevice::Field & mode_field = modular_server_.createField(constants::mode_field_name,constants::mode_ptr_default);
  mode_field.setSubset(constants::mode_ptr_subset);

  ModularDevice::Field & odd_array_field = modular_server_.createField(constants::odd_array_field_name,constants::odd_array_default);
  odd_array_field.setSubset(constants::odd_subset);

  // Parameters
  ModularDevice::Parameter & long_array_parameter = modular_server_.createParameter(constants::long_array_parameter_name);
  long_array_parameter.setTypeLong();
  long_array_parameter.setTypeArray();

  ModularDevice::Parameter & length_parameter = modular_server_.createParameter(constants::length_parameter_name);
  length_parameter.setRange(constants::length_min,constants::length_max);

  ModularDevice::Parameter & count_parameter = modular_server_.createParameter(constants::count_parameter_name);
  count_parameter.setTypeLong();
  count_parameter.setSubset(constants::count_subset);

  ModularDevice::Parameter & count_array_parameter = modular_server_.createParameter(constants::count_array_parameter_name);
  count_array_parameter.setTypeLong();
  count_array_parameter.setRange(constants::count_array_element_min,constants::count_array_element_max);
  count_array_parameter.setArrayLengthRange(constants::count_array_length_min,constants::count_array_length_max);
  count_array_parameter.setSubset(constants::count_subset);

  ModularDevice::Parameter & direction_parameter = modular_server_.createParameter(constants::direction_parameter_name);
  direction_parameter.setTypeString();
  direction_parameter.setSubset(constants::direction_ptr_subset);

  ModularDevice::Parameter & direction_array_parameter = modular_server_.createParameter(constants::direction_array_parameter_name);
  direction_array_parameter.setTypeString();
  direction_array_parameter.setArrayLengthRange(constants::direction_array_length_min,constants::direction_array_length_max);
  direction_array_parameter.setSubset(constants::direction_ptr_subset);

  // Methods
  ModularDevice::Method & get_doubled_method = modular_server_.createMethod(constants::get_doubled_method_name);
  get_doubled_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::getDoubledCallback));
  get_doubled_method.setReturnTypeDouble();

  ModularDevice::Method & get_bool_method = modular_server_.createMethod(constants::get_bool_method_name);
  get_bool_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::getBoolCallback));
  get_bool_method.setReturnTypeBool();

  ModularDevice::Method & get_long_array_fixed_method = modular_server_.createMethod(constants::get_long_array_fixed_method_name);
  get_long_array_fixed_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::getLongArrayFixedCallback));
  get_long_array_fixed_method.setReturnTypeArray();

  ModularDevice::Method & get_long_array_variable_method = modular_server_.createMethod(constants::get_long_array_variable_method_name);
  get_long_array_variable_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::getLongArrayVariableCallback));
  get_long_array_variable_method.setReturnTypeArray();

  ModularDevice::Method & set_long_array_fixed_method = modular_server_.createMethod(constants::set_long_array_fixed_method_name);
  set_long_array_fixed_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::setLongArrayFixedCallback));
  set_long_array_fixed_method.setReturnTypeBool();

  ModularDevice::Method & set_long_array_variable_method = modular_server_.createMethod(constants::set_long_array_variable_method_name);
  set_long_array_variable_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::setLongArrayVariableCallback));
  set_long_array_variable_method.setReturnTypeBool();

  ModularDevice::Method & set_long_array_parameter_method = modular_server_.createMethod(constants::set_long_array_parameter_method_name);
  set_long_array_parameter_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::setLongArrayParameterCallback));
  set_long_array_parameter_method.addParameter(long_array_parameter);
  set_long_array_parameter_method.setReturnTypeBool();

  ModularDevice::Method & get_string_all_method = modular_server_.createMethod(constants::get_string_all_method_name);
  get_string_all_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::getStringAllCallback));
  get_string_all_method.setReturnTypeString();

  ModularDevice::Method & get_string_some_method = modular_server_.createMethod(constants::get_string_some_method_name);
  get_string_some_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::getStringSomeCallback));
  get_string_some_method.addParameter(length_parameter);
  get_string_some_method.setReturnTypeString();

  ModularDevice::Method & get_count_method = modular_server_.createMethod(constants::get_count_method_name);
  get_count_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::getCountCallback));
  get_count_method.addParameter(count_parameter);
  get_count_method.setReturnTypeString();

  ModularDevice::Method & get_count_array_method = modular_server_.createMethod(constants::get_count_array_method_name);
  get_count_array_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::getCountArrayCallback));
  get_count_array_method.addParameter(count_array_parameter);
  get_count_array_method.setReturnTypeArray();

  ModularDevice::Method & get_direction_method = modular_server_.createMethod(constants::get_direction_method_name);
  get_direction_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::getDirectionCallback));
  get_direction_method.addParameter(direction_parameter);
  get_direction_method.setReturnTypeString();

  ModularDevice::Method & get_direction_array_method = modular_server_.createMethod(constants::get_direction_array_method_name);
  get_direction_array_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::getDirectionArrayCallback));
  get_direction_array_method.addParameter(direction_array_parameter);
  get_direction_array_method.setReturnTypeArray();

  ModularDevice::Method & check_mode_method = modular_server_.createMethod(constants::check_mode_method_name);
  check_mode_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::checkModeCallback));
  check_mode_method.setReturnTypeString();

  ModularDevice::Method & increment_mode_method = modular_server_.createMethod(constants::increment_mode_method_name);
  increment_mode_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::incrementModeCallback));
  increment_mode_method.setReturnTypeString();

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

void Controller::getDoubledCallback()
{
  double value;
  modular_server_.getFieldValue(constants::double_field_name,value);
  value *= 2;
  modular_server_.writeResultToResponse(value);
}

void Controller::getBoolCallback()
{
  bool value;
  modular_server_.getFieldValue(constants::bool_field_name,value);
  modular_server_.writeResultToResponse(value);
}

void Controller::getLongArrayFixedCallback()
{
  long long_array[constants::LONG_ARRAY_LENGTH];
  modular_server_.getFieldValue(constants::long_array_field_name,long_array);
  modular_server_.writeResultToResponse(long_array);
}

void Controller::getLongArrayVariableCallback()
{
  size_t array_length = modular_server_.getFieldArrayLength(constants::long_array_field_name);
  long long_array[array_length];
  modular_server_.getFieldValue(constants::long_array_field_name,long_array,array_length);
  modular_server_.writeResultToResponse(long_array,array_length);
}

void Controller::setLongArrayFixedCallback()
{
  long long_array[constants::LONG_ARRAY_LENGTH];
  long_array[0] = 1;
  long_array[1] = 2;
  long_array[2] = 102;
  long_array[3] = 103;
  bool success = modular_server_.setFieldValue(constants::long_array_field_name,long_array);
  modular_server_.writeResultToResponse(success);
}

void Controller::setLongArrayVariableCallback()
{
  size_t array_length = modular_server_.getFieldArrayLength(constants::long_array_field_name);
  long long_array[array_length-1];
  long_array[0] = -1;
  long_array[1] = -2;
  long_array[2] = 9;
  bool success = modular_server_.setFieldValue(constants::long_array_field_name,long_array,array_length-1);
  modular_server_.writeResultToResponse(success);
}

void Controller::setLongArrayParameterCallback()
{
  ArduinoJson::JsonArray & long_array = modular_server_.getParameterValue(constants::long_array_parameter_name);
  bool success = modular_server_.setFieldValue(constants::long_array_field_name,long_array);
  modular_server_.writeResultToResponse(success);
}

void Controller::getStringAllCallback()
{
  size_t array_length = modular_server_.getFieldArrayLength(constants::string_field_name);
  char string[array_length];
  modular_server_.getFieldValue(constants::string_field_name,string,array_length);
  modular_server_.writeResultToResponse(string);
}

void Controller::getStringSomeCallback()
{
  long length = modular_server_.getParameterValue(constants::length_parameter_name);
  size_t array_length = length + 1;
  char string[array_length];
  modular_server_.getFieldValue(constants::string_field_name,string,array_length);
  modular_server_.writeResultToResponse(string);
}

void Controller::getCountCallback()
{
  long count = modular_server_.getParameterValue(constants::count_parameter_name);
  switch (count)
  {
    case 10:
    {
      modular_server_.writeResultToResponse("ten");
      break;
    }
    case 20:
    {
      modular_server_.writeResultToResponse("twenty");
      break;
    }
    case 30:
    {
      modular_server_.writeResultToResponse("thirty");
      break;
    }
    case 40:
    {
      modular_server_.writeResultToResponse("forty");
      break;
    }
    case 50:
    {
      modular_server_.writeResultToResponse("fifty");
      break;
    }
    case 60:
    {
      modular_server_.writeResultToResponse("sixty");
      break;
    }
    case 70:
    {
      modular_server_.writeResultToResponse("seventy");
      break;
    }
    default:
    {
      modular_server_.writeResultToResponse("???");
      break;
    }
  }
}

void Controller::getCountArrayCallback()
{
  ArduinoJson::JsonArray & count_array = modular_server_.getParameterValue(constants::count_array_parameter_name);
  modular_server_.writeResultToResponse(&count_array);
}

void Controller::getDirectionCallback()
{
  const char * direction = modular_server_.getParameterValue(constants::direction_parameter_name);
  modular_server_.writeResultToResponse(direction);
}

void Controller::getDirectionArrayCallback()
{
  ArduinoJson::JsonArray & direction_array = modular_server_.getParameterValue(constants::direction_array_parameter_name);
  modular_server_.writeResultToResponse(&direction_array);
}

void Controller::checkModeCallback()
{
  const ConstantString * mode_ptr;
  modular_server_.getFieldValue(constants::mode_field_name,mode_ptr);
  if (mode_ptr == &constants::mode_rising)
  {
    modular_server_.writeResultToResponse("Mode set to rising!");
  }
  else if (mode_ptr == &constants::mode_falling)
  {
    modular_server_.writeResultToResponse("Mode set to falling!");
  }
  else if (mode_ptr == &constants::mode_change)
  {
    modular_server_.writeResultToResponse("Mode set to change!");
  }
  else
  {
    modular_server_.writeResultToResponse("???");
  }
}

void Controller::incrementModeCallback()
{
  const ConstantString * mode_ptr;
  modular_server_.getFieldValue(constants::mode_field_name,mode_ptr);
  if (mode_ptr == &constants::mode_rising)
  {
    modular_server_.setFieldValue(constants::mode_field_name,&constants::mode_falling);
    modular_server_.writeResultToResponse("Mode was set to rising, now set to falling!");
  }
  else if (mode_ptr == &constants::mode_falling)
  {
    modular_server_.setFieldValue(constants::mode_field_name,&constants::mode_change);
    modular_server_.writeResultToResponse("Mode was set to falling, now set to change!");
  }
  else if (mode_ptr == &constants::mode_change)
  {
    modular_server_.setFieldValue(constants::mode_field_name,&constants::mode_rising);
    modular_server_.writeResultToResponse("Mode was set to change, now set to rising!");
  }
  else
  {
    modular_server_.writeResultToResponse("???");
  }
}

void Controller::preSetFieldValueCallback()
{
  // Writing to Serial like this makes response invalid!
  // Do not do this in a real device!
  // Uncomment next line just to demonstrate setFieldValueCallback

  // Serial << " setting field value! ";
}

void Controller::postSetFieldValueCallback()
{
  // Writing to Serial like this makes response invalid!
  // Do not do this in a real device!
  // Uncomment next line just to demonstrate setFieldValueCallback

  // Serial << " set field value! ";
}

void Controller::preSetFieldElementValueCallback(const size_t element_index)
{
  // Writing to Serial like this makes response invalid!
  // Do not do this in a real device!
  // Uncomment next line just to demonstrate setFieldElementValueCallback

  // Serial << " setting field element value at index " << element_index << "! ";
}

void Controller::postSetFieldElementValueCallback(const size_t element_index)
{
  // Writing to Serial like this makes response invalid!
  // Do not do this in a real device!
  // Uncomment next line just to demonstrate setFieldElementValueCallback

  // Serial << " set field element value at index " << element_index << "! ";
}
