// ----------------------------------------------------------------------------
// BoardLedController.cpp
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "BoardLedController.h"


void BoardLedController::setup()
{
  // Pin Setup
  pinMode(constants::led_pin, OUTPUT);

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
                              methods_,
                              callbacks_);

  // Fields

  // Parameters
  modular_server::Parameter & duration_on_parameter = modular_server_.createParameter(constants::duration_on_parameter_name);
  duration_on_parameter.setUnits(constants::seconds_unit);
  duration_on_parameter.setRange(constants::duration_min,constants::duration_max);

  modular_server::Parameter & duration_off_parameter = modular_server_.copyParameter(duration_on_parameter,constants::duration_off_parameter_name);

  modular_server::Parameter & count_parameter = modular_server_.createParameter(constants::count_parameter_name);
  count_parameter.setRange(constants::count_min,constants::count_max);

  // Methods
  modular_server::Method & led_on_method = modular_server_.createMethod(constants::led_on_method_name);
  led_on_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&BoardLedController::setLedOnFunctor));

  modular_server::Method & led_off_method = modular_server_.createMethod(constants::led_off_method_name);
  led_off_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&BoardLedController::setLedOffFunctor));

  modular_server::Method & get_led_pin_method = modular_server_.createMethod(constants::get_led_pin_method_name);
  get_led_pin_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&BoardLedController::getLedPinFunctor));
  get_led_pin_method.setReturnTypeLong();

  modular_server::Method & blink_led_method = modular_server_.createMethod(constants::blink_led_method_name);
  blink_led_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&BoardLedController::blinkLedFunctor));
  blink_led_method.addParameter(duration_on_parameter);
  blink_led_method.addParameter(duration_off_parameter);
  blink_led_method.addParameter(count_parameter);

  // Methods

  // Callbacks

  // Begin Streams
  Serial.begin(constants::baudrate);

  // Start Modular Device Server
  modular_server_.startServer();
}

void BoardLedController::update()
{
  modular_server_.handleServerRequests();
  non_block_blink.update();
}

// Functors must be non-blocking (avoid 'delay')
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

void BoardLedController::setLedOnFunctor()
{
  non_block_blink.stop();
  digitalWrite(constants::led_pin, HIGH);
}

void BoardLedController::setLedOffFunctor()
{
  non_block_blink.stop();
  digitalWrite(constants::led_pin, LOW);
}

void BoardLedController::getLedPinFunctor()
{
  modular_server_.response().returnResult(constants::led_pin);
}

void BoardLedController::blinkLedFunctor()
{
  float duration_on;
  modular_server_.parameter(constants::duration_on_parameter_name).getValue(duration_on);
  double duration_off;
  modular_server_.parameter(constants::duration_off_parameter_name).getValue(duration_off);
  int count;
  modular_server_.parameter(constants::count_parameter_name).getValue(count);
  non_block_blink.setDurationOn(duration_on);
  non_block_blink.setDurationOff(duration_off);
  non_block_blink.setCount(count);
  non_block_blink.start();
}
