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
  modular_server.setName(constants::device_name);
  modular_server.setModelNumber(constants::model_number);
  modular_server.setFirmwareVersion(constants::firmware_major,constants::firmware_minor,constants::firmware_patch);

  // Add Server Streams

  // Saved Variables

  // Parameters

  // Methods

  // Setup Streams
  Serial.begin(constants::baudrate);

  // Start Modular Device Server
  modular_server.startServer();
}

void Controller::update()
{
  modular_server.handleServerRequests();
}

Controller controller;
