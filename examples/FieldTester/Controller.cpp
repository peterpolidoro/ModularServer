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
  modular_server_.setName(constants::device_name);
  modular_server_.setModelNumber(constants::model_number);
  modular_server_.setFirmwareVersion(constants::firmware_major,constants::firmware_minor,constants::firmware_patch);

  // Add Server Streams
  modular_server_.addServerStream(Serial);
  modular_server_.addServerStream(constants::serial2);

  // Set Storage
  modular_server_.setFieldStorage(fields_);
  modular_server_.setParameterStorage(parameters_);
  modular_server_.setMethodStorage(methods_);

  // Fields
  ModularDevice::Field& double_field = modular_server_.createField(constants::double_field_name,constants::double_default);

  ModularDevice::Field& bool_field = modular_server_.createField(constants::bool_field_name,constants::bool_default);

  ModularDevice::Field& long_array_field = modular_server_.createField(constants::long_array_field_name,constants::long_array_default);
  long_array_field.setRange(constants::long_array_element_min,constants::long_array_element_max);

  ModularDevice::Field& double_array_field = modular_server_.createField(constants::double_array_field_name,constants::double_array_default);
  double_array_field.setRange(constants::double_array_element_min,constants::double_array_element_max);

  ModularDevice::Field& bool_array_field = modular_server_.createField(constants::bool_array_field_name,constants::bool_array_default);

  ModularDevice::Field& string_field = modular_server_.createField(constants::string_field_name,constants::string_default);

  ModularDevice::Field& odd_field = modular_server_.createField(constants::odd_field_name,constants::odd_default);
  odd_field.setSubset(constants::odd_subset);

  ModularDevice::Field& mode_field = modular_server_.createField(constants::mode_field_name,constants::mode_ptr_default);
  mode_field.setSubset(constants::mode_ptr_subset);

  // Parameters
  ModularDevice::Parameter& long_array_parameter = modular_server_.createParameter(constants::long_array_parameter_name);
  long_array_parameter.setTypeLong();
  long_array_parameter.setTypeArray();

  ModularDevice::Parameter& length_parameter = modular_server_.createParameter(constants::length_parameter_name);
  length_parameter.setRange(constants::length_min,constants::length_max);

  ModularDevice::Parameter& count_parameter = modular_server_.createParameter(constants::count_parameter_name);
  count_parameter.setTypeLong();
  count_parameter.setSubset(constants::count_subset);

  ModularDevice::Parameter& count_array_parameter = modular_server_.createParameter(constants::count_array_parameter_name);
  count_array_parameter.setTypeLong();
  count_array_parameter.setRange(constants::count_array_element_min,constants::count_array_element_max);
  count_array_parameter.setArrayLengthRange(constants::count_array_length_min,constants::count_array_length_max);
  count_array_parameter.setSubset(constants::count_subset);

  ModularDevice::Parameter& direction_parameter = modular_server_.createParameter(constants::direction_parameter_name);
  direction_parameter.setTypeString();
  direction_parameter.setSubset(constants::direction_ptr_subset);

  ModularDevice::Parameter& direction_array_parameter = modular_server_.createParameter(constants::direction_array_parameter_name);
  direction_array_parameter.setTypeString();
  direction_array_parameter.setArrayLengthRange(constants::direction_array_length_min,constants::direction_array_length_max);
  direction_array_parameter.setSubset(constants::direction_ptr_subset);

  // Methods
  ModularDevice::Method& get_doubled_method = modular_server_.createMethod(constants::get_doubled_method_name);
  get_doubled_method.attachCallback(callbacks::getDoubledCallback);
  get_doubled_method.setReturnTypeDouble();

  ModularDevice::Method& get_bool_method = modular_server_.createMethod(constants::get_bool_method_name);
  get_bool_method.attachCallback(callbacks::getBoolCallback);
  get_bool_method.setReturnTypeBool();

  ModularDevice::Method& get_long_array_fixed_method = modular_server_.createMethod(constants::get_long_array_fixed_method_name);
  get_long_array_fixed_method.attachCallback(callbacks::getLongArrayFixedCallback);
  get_long_array_fixed_method.setReturnTypeArray();

  ModularDevice::Method& get_long_array_variable_method = modular_server_.createMethod(constants::get_long_array_variable_method_name);
  get_long_array_variable_method.attachCallback(callbacks::getLongArrayVariableCallback);
  get_long_array_variable_method.setReturnTypeArray();

  ModularDevice::Method& set_long_array_fixed_method = modular_server_.createMethod(constants::set_long_array_fixed_method_name);
  set_long_array_fixed_method.attachCallback(callbacks::setLongArrayFixedCallback);
  set_long_array_fixed_method.setReturnTypeBool();

  ModularDevice::Method& set_long_array_variable_method = modular_server_.createMethod(constants::set_long_array_variable_method_name);
  set_long_array_variable_method.attachCallback(callbacks::setLongArrayVariableCallback);
  set_long_array_variable_method.setReturnTypeBool();

  ModularDevice::Method& set_long_array_parameter_method = modular_server_.createMethod(constants::set_long_array_parameter_method_name);
  set_long_array_parameter_method.attachCallback(callbacks::setLongArrayParameterCallback);
  set_long_array_parameter_method.addParameter(long_array_parameter);
  set_long_array_parameter_method.setReturnTypeBool();

  ModularDevice::Method& get_string_all_method = modular_server_.createMethod(constants::get_string_all_method_name);
  get_string_all_method.attachCallback(callbacks::getStringAllCallback);
  get_string_all_method.setReturnTypeString();

  ModularDevice::Method& get_string_some_method = modular_server_.createMethod(constants::get_string_some_method_name);
  get_string_some_method.attachCallback(callbacks::getStringSomeCallback);
  get_string_some_method.addParameter(length_parameter);
  get_string_some_method.setReturnTypeString();

  ModularDevice::Method& get_count_method = modular_server_.createMethod(constants::get_count_method_name);
  get_count_method.attachCallback(callbacks::getCountCallback);
  get_count_method.addParameter(count_parameter);
  get_count_method.setReturnTypeString();

  ModularDevice::Method& get_count_array_method = modular_server_.createMethod(constants::get_count_array_method_name);
  get_count_array_method.attachCallback(callbacks::getCountArrayCallback);
  get_count_array_method.addParameter(count_array_parameter);
  get_count_array_method.setReturnTypeArray();

  ModularDevice::Method& get_direction_method = modular_server_.createMethod(constants::get_direction_method_name);
  get_direction_method.attachCallback(callbacks::getDirectionCallback);
  get_direction_method.addParameter(direction_parameter);
  get_direction_method.setReturnTypeString();

  ModularDevice::Method& get_direction_array_method = modular_server_.createMethod(constants::get_direction_array_method_name);
  get_direction_array_method.attachCallback(callbacks::getDirectionArrayCallback);
  get_direction_array_method.addParameter(direction_array_parameter);
  get_direction_array_method.setReturnTypeArray();

  ModularDevice::Method& check_mode_method = modular_server_.createMethod(constants::check_mode_method_name);
  check_mode_method.attachCallback(callbacks::checkModeCallback);
  check_mode_method.setReturnTypeString();

  // Setup Streams
  Serial.begin(constants::baudrate);
  constants::serial2.begin(constants::baudrate);

  // Start Modular Device Server
  modular_server_.startServer();
}

void Controller::update()
{
  modular_server_.handleServerRequests();
}

ModularDevice::ModularServer& Controller::getModularServer()
{
  return modular_server_;
}

Controller controller;
