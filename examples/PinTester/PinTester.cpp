// ----------------------------------------------------------------------------
// PinTester.cpp
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "PinTester.h"


void PinTester::setup()
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
    pins_);

  // Pins
  modular_server::Pin & pin_a_pin = modular_server_.createPin(constants::pin_a_pin_name,
    constants::pin_a_pin_number);
  pin_a_pin.setModePwmFalling();

  // Add Firmware
  modular_server_.addFirmware(constants::firmware_info,
    properties_,
    parameters_,
    functions_,
    callbacks_);

  // Properties

  // Parameters

  // Functions

  // Callbacks
  modular_server::Callback & test_pin_callback = modular_server_.createCallback(constants::test_pin_callback_name);
  test_pin_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&PinTester::testPinHandler));

  // Begin Streams
  Serial.begin(constants::baud);
}

void PinTester::startServer()
{
  // Start Modular Device Server
  modular_server_.startServer();
}

void PinTester::update()
{
  modular_server_.handleServerRequests();
}

void PinTester::testPin()
{
  modular_server_.pin(constants::pin_a_pin_name).addPwm(constants::pwm_period_ms,
    constants::pwm_on_duration_ms,
    constants::pwm_count);
}

void PinTester::testPinHandler(modular_server::Pin * pin_ptr)
{
  testPin();
}


