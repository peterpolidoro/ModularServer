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

  // Fields

  // Parameters

  // Methods

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
