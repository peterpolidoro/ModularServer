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
  pinMode(constants::led_pin, OUTPUT);

  // Device Info
  modular_server_.setDeviceName(constants::device_name);
  modular_server_.setFormFactor(constants::form_factor);
  modular_server_.addFirmwareInfo(constants::firmware_info);
  modular_server_.addHardwareInfo(constants::hardware_info);

  // Add Server Streams
  modular_server_.addServerStream(Serial);

  // Set Storage
  modular_server_.addFieldStorage(fields_);
  modular_server_.addParameterStorage(parameters_);
  modular_server_.addMethodStorage(methods_);

  // Fields

  // Parameters
  ModularDevice::Parameter & duration_on_parameter = modular_server_.createParameter(constants::duration_on_parameter_name);
  duration_on_parameter.setUnits(constants::seconds_unit);
  duration_on_parameter.setRange(constants::duration_min,constants::duration_max);

  ModularDevice::Parameter & duration_off_parameter = modular_server_.copyParameter(duration_on_parameter,constants::duration_off_parameter_name);

  ModularDevice::Parameter & count_parameter = modular_server_.createParameter(constants::count_parameter_name);
  count_parameter.setRange(constants::count_min,constants::count_max);

  // Methods
  ModularDevice::Method & led_on_method = modular_server_.createMethod(constants::led_on_method_name);
  led_on_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::setLedOnCallback));

  ModularDevice::Method & led_off_method = modular_server_.createMethod(constants::led_off_method_name);
  led_off_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::setLedOffCallback));

  ModularDevice::Method & get_led_pin_method = modular_server_.createMethod(constants::get_led_pin_method_name);
  get_led_pin_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::getLedPinCallback));
  get_led_pin_method.setReturnTypeLong();

  ModularDevice::Method & blink_led_method = modular_server_.createMethod(constants::blink_led_method_name);
  blink_led_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Controller::blinkLedCallback));
  blink_led_method.addParameter(duration_on_parameter);
  blink_led_method.addParameter(duration_off_parameter);
  blink_led_method.addParameter(count_parameter);

  // Setup Streams
  Serial.begin(constants::baudrate);

  // Start Modular Device Server
  modular_server_.startServer();
}

void Controller::update()
{
  modular_server_.handleServerRequests();
  non_block_blink.update();
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

void Controller::setLedOnCallback()
{
  non_block_blink.stop();
  digitalWrite(constants::led_pin, HIGH);
}

void Controller::setLedOffCallback()
{
  non_block_blink.stop();
  digitalWrite(constants::led_pin, LOW);
}

void Controller::getLedPinCallback()
{
  modular_server_.writeResultToResponse(constants::led_pin);
}

void Controller::blinkLedCallback()
{
  double duration_on = modular_server_.getParameterValue(constants::duration_on_parameter_name);
  double duration_off = modular_server_.getParameterValue(constants::duration_off_parameter_name);
  long count = modular_server_.getParameterValue(constants::count_parameter_name);
  non_block_blink.setDurationOn(duration_on);
  non_block_blink.setDurationOff(duration_off);
  non_block_blink.setCount(count);
  non_block_blink.start();
}
