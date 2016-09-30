// ----------------------------------------------------------------------------
// Controller.h
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Functor.h"
#include "ModularServer.h"

#include "Constants.h"


class Controller
{
public:
  Controller();
  void setup();
  void update();

private:
  Modular::ModularServer modular_server_;

  Modular::Field fields_[constants::FIELD_COUNT_MAX];
  Modular::Parameter parameters_[constants::PARAMETER_COUNT_MAX];
  Modular::Method methods_[constants::METHOD_COUNT_MAX];

  // Callbacks
  void echoCallback();
  void lengthCallback();
  void startsWithCallback();
  void repeatCallback();
  void charsAtCallback();
  void startingCharsCallback();
  void setStoredStringCallback();
  void getStoredStringCallback();

};

#endif
