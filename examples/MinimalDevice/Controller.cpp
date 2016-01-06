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

  // Set Storage Arrays
  modular_server_.setSavedVariableStorageArray(saved_variables_);
  modular_server_.setParameterStorageArray(parameters_);
  modular_server_.setMethodStorageArray(methods_);

  // Saved Variables

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

ModularDevice::ModularServer& Controller::getModularServer()
{
  return modular_server_;
}

Controller controller;
