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


class Controller
{
public:
  Controller();
  void setup();
  void update();
  ModularDevice::ModularServer& getModularServer();

  void setStoredString(String str);
  String getStoredString();
private:
  ModularDevice::ModularServer modular_server_;

  ModularDevice::Field fields_[constants::FIELD_COUNT_MAX];
  ModularDevice::Parameter parameters_[constants::PARAMETER_COUNT_MAX];
  ModularDevice::Method methods_[constants::METHOD_COUNT_MAX];

  String stored_string_;
};

extern Controller controller;

#endif
