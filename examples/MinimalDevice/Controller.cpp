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
  modular_server_.setHardwareVersion(constants::hardware_major,constants::hardware_minor,constants::hardware_patch);

  // Add Server Streams
  modular_server_.addServerStream(Serial);

  // Set Storage
  modular_server_.setFieldStorage(fields_);
  modular_server_.setParameterStorage(parameters_);
  modular_server_.setMethodStorage(methods_);

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

ModularDevice::ModularServer & Controller::getModularServer()
{
  return modular_server_;
}

Controller controller;
