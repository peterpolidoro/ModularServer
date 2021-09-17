// ----------------------------------------------------------------------------
// MinimalDevice.cpp
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "MinimalDevice.h"


void MinimalDevice::setup()
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

  // Begin Streams
  Serial.begin(constants::baud);
}

void MinimalDevice::startServer()
{
  // Start Modular Device Server
  modular_server_.startServer();
}

void MinimalDevice::update()
{
  modular_server_.handleServerRequests();
}

