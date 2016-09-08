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
  odd_field.setMembership(constants::odd_members);

  // Parameters
  ModularDevice::Parameter& long_array_parameter = modular_server_.createParameter(constants::long_array_parameter_name);
  long_array_parameter.setTypeLong();
  long_array_parameter.setTypeArray();

  ModularDevice::Parameter& length_parameter = modular_server_.createParameter(constants::length_parameter_name);
  length_parameter.setRange(constants::length_min,constants::length_max);

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
