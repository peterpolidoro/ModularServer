// ----------------------------------------------------------------------------
// BoardLedController.cpp
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "BoardLedController.h"


void BoardLedController::setup()
{
  // Server Setup
  modular_server_.setup();

  // Add Server Streams
  modular_server_.addServerStream(Serial);

  // Set Device ID
  modular_server_.setDeviceName(constants::device_name);
  modular_server_.setFormFactor(constants::form_factor);

  // Add Hardware
  modular_server_.addHardware(constants::hardware_info,
                              pins_);

  // Pins
  modular_server::Pin & led_pin = modular_server_.createPin(constants::led_pin_name,constants::led_pin_number);
  led_pin.setModeDigitalOutput();

  blinker_.setup(led_pin);

  // Add Firmware
  modular_server_.addFirmware(constants::firmware_info,
                              properties_,
                              parameters_,
                              functions_,
                              callbacks_);

  // Properties

  // Parameters
  modular_server::Parameter & duration_on_parameter = modular_server_.createParameter(constants::duration_on_parameter_name);
  duration_on_parameter.setUnits(constants::seconds_unit);
  duration_on_parameter.setRange(constants::duration_min,constants::duration_max);

  modular_server::Parameter & duration_off_parameter = modular_server_.copyParameter(duration_on_parameter,constants::duration_off_parameter_name);

  modular_server::Parameter & count_parameter = modular_server_.createParameter(constants::count_parameter_name);
  count_parameter.setRange(constants::count_min,constants::count_max);

  // Functions
  modular_server::Function & led_on_function = modular_server_.createFunction(constants::led_on_function_name);
  led_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&BoardLedController::setLedOnHandler));

  modular_server::Function & led_off_function = modular_server_.createFunction(constants::led_off_function_name);
  led_off_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&BoardLedController::setLedOffHandler));

  modular_server::Function & get_led_pin_number_function = modular_server_.createFunction(constants::get_led_pin_number_function_name);
  get_led_pin_number_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&BoardLedController::getLedPinNumberHandler));
  get_led_pin_number_function.setResultTypeLong();

  modular_server::Function & blink_led_function = modular_server_.createFunction(constants::blink_led_function_name);
  blink_led_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&BoardLedController::blinkLedHandler));
  blink_led_function.addParameter(duration_on_parameter);
  blink_led_function.addParameter(duration_off_parameter);
  blink_led_function.addParameter(count_parameter);

  // Functions

  // Callbacks

  // Begin Streams
  Serial.begin(constants::baud);
}

void BoardLedController::startServer()
{
  // Start Modular Device Server
  modular_server_.startServer();
}

void BoardLedController::update()
{
  modular_server_.handleServerRequests();
  blinker_.update();
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

void BoardLedController::setLedOnHandler()
{
  blinker_.stop();
  modular_server_.pin(constants::led_pin_name).setValue(HIGH);
}

void BoardLedController::setLedOffHandler()
{
  blinker_.stop();
  modular_server_.pin(constants::led_pin_name).setValue(LOW);
}

void BoardLedController::getLedPinNumberHandler()
{
  size_t led_pin_number = modular_server_.pin(constants::led_pin_name).getPinNumber();
  modular_server_.response().returnResult(led_pin_number);
}

void BoardLedController::blinkLedHandler()
{
  float duration_on;
  modular_server_.parameter(constants::duration_on_parameter_name).getValue(duration_on);
  double duration_off;
  modular_server_.parameter(constants::duration_off_parameter_name).getValue(duration_off);
  int count;
  modular_server_.parameter(constants::count_parameter_name).getValue(count);
  blinker_.stop();
  blinker_.setDurationOn(duration_on);
  blinker_.setDurationOff(duration_off);
  blinker_.setCount(count);
  blinker_.start();
}
