// ----------------------------------------------------------------------------
// CallbackTester.cpp
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "CallbackTester.h"


void CallbackTester::setup()
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

  modular_server::Pin & bnc_a_pin = modular_server_.createPin(constants::bnc_a_pin_name,
                                                              constants::bnc_a_pin_number);

  modular_server::Pin & bnc_b_pin = modular_server_.createPin(constants::bnc_b_pin_name,
                                                              constants::bnc_b_pin_number);

  modular_server::Pin & bnc_c_pin = modular_server_.createPin(constants::bnc_c_pin_name,
                                                              constants::bnc_c_pin_number);

  modular_server::Pin & bnc_d_pin = modular_server_.createPin(constants::bnc_d_pin_name,
                                                              constants::bnc_d_pin_number);

  modular_server::Pin & bnc_e_pin = modular_server_.createPin(constants::bnc_e_pin_name,
                                                              constants::bnc_e_pin_number);

  modular_server::Pin & bnc_f_pin = modular_server_.createPin(constants::bnc_f_pin_name,
                                                              constants::bnc_f_pin_number);

  // Add Firmware
  modular_server_.addFirmware(constants::firmware_info,
                              properties_,
                              parameters_,
                              functions_,
                              callbacks_);

  // Properties
  modular_server::Property & duration_on_property = modular_server_.createProperty(constants::duration_on_property_name,constants::duration_on_default);
  duration_on_property.setUnits(constants::seconds_unit);
  duration_on_property.setRange(constants::duration_min,constants::duration_max);

  modular_server::Property & duration_off_property = modular_server_.createProperty(constants::duration_off_property_name,constants::duration_off_default);
  duration_off_property.setUnits(constants::seconds_unit);
  duration_off_property.setRange(constants::duration_min,constants::duration_max);

  modular_server::Property & count_property = modular_server_.createProperty(constants::count_property_name,constants::count_default);
  count_property.setRange(constants::count_min,constants::count_max);

  // Parameters

  // Functions

  // Callbacks
  modular_server::Callback & set_led_on_callback = modular_server_.createCallback(constants::set_led_on_callback_name);
  set_led_on_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&CallbackTester::setLedOnHandler));
  set_led_on_callback.attachTo(bnc_a_pin,modular_server::constants::pin_mode_interrupt_falling);
  set_led_on_callback.attachTo(bnc_c_pin,modular_server::constants::pin_mode_interrupt_falling);

  modular_server::Callback & set_led_off_callback = modular_server_.createCallback(constants::set_led_off_callback_name);
  set_led_off_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&CallbackTester::setLedOffHandler));
  set_led_off_callback.attachTo(bnc_b_pin,modular_server::constants::pin_mode_interrupt_falling);
  set_led_off_callback.attachTo(bnc_d_pin,modular_server::constants::pin_mode_interrupt_falling);
  set_led_off_callback.attachTo(bnc_f_pin,modular_server::constants::pin_mode_interrupt_falling);

  modular_server::Callback & blink_led_callback = modular_server_.createCallback(constants::blink_led_callback_name);
  blink_led_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&CallbackTester::blinkLedHandler));
  blink_led_callback.addProperty(duration_on_property);
  blink_led_callback.addProperty(duration_off_property);
  blink_led_callback.addProperty(count_property);
  blink_led_callback.attachTo(bnc_e_pin,modular_server::constants::pin_mode_interrupt_falling);

  // Begin Streams
  Serial.begin(constants::baud);
}

void CallbackTester::startServer()
{
  // Start Modular Device Server
  modular_server_.startServer();
}

void CallbackTester::update()
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

void CallbackTester::setLedOnHandler(modular_server::Pin * pin_ptr)
{
  blinker_.stop();
  modular_server_.pin(constants::led_pin_name).setValue(HIGH);
}

void CallbackTester::setLedOffHandler(modular_server::Pin * pin_ptr)
{
  blinker_.stop();
  modular_server_.pin(constants::led_pin_name).setValue(LOW);
}

void CallbackTester::blinkLedHandler(modular_server::Pin * pin_ptr)
{
  double duration_on;
  modular_server_.property(constants::duration_on_property_name).getValue(duration_on);
  double duration_off;
  modular_server_.property(constants::duration_off_property_name).getValue(duration_off);
  long count;
  modular_server_.property(constants::count_property_name).getValue(count);
  blinker_.stop();
  blinker_.setDurationOn(duration_on);
  blinker_.setDurationOff(duration_off);
  blinker_.setCount(count);
  blinker_.start();
}
