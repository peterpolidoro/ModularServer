// ----------------------------------------------------------------------------
// MinimalDevice.cpp
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "MinimalDevice.h"


void MinimalDevice::setup()
{
  // Pin Setup

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
                              methods_);

  // Fields

  // Parameters

  // Methods

  // Begin Streams
  Serial.begin(constants::baudrate);

  // Start Modular Device Server
  modular_server_.startServer();
}

void MinimalDevice::update()
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
// field.getValue type must match the field default type
// field.setValue type must match the field default type
// field.getElementValue type must match the field array element default type
// field.setElementValue type must match the field array element default type
