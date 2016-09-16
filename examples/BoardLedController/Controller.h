// ----------------------------------------------------------------------------
// Controller.h
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "ModularServer.h"
#include "Constants.h"
#include "Callbacks.h"
#include "NonBlockBlink.h"


class Controller
{
public:
  Controller();
  void setup();
  void update();
  ModularDevice::ModularServer & getModularServer();

private:
  ModularDevice::ModularServer modular_server_;

  ModularDevice::Field fields_[constants::FIELD_COUNT_MAX];
  ModularDevice::Parameter parameters_[constants::PARAMETER_COUNT_MAX];
  ModularDevice::Method methods_[constants::METHOD_COUNT_MAX];
};

extern Controller controller;

#endif
