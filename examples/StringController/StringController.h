// ----------------------------------------------------------------------------
// StringController.h
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef STRING_CONTROLLER_H
#define STRING_CONTROLLER_H
#include "Functor.h"
#include "ModularServer.h"

#include "Constants.h"


class StringController
{
public:
  void setup();
  void update();

private:
  modular_server::ModularServer modular_server_;

  modular_server::Field fields_[constants::FIELD_COUNT_MAX];
  modular_server::Parameter parameters_[constants::PARAMETER_COUNT_MAX];
  modular_server::Method methods_[constants::METHOD_COUNT_MAX];
  modular_server::Interrupt interrupts_[constants::INTERRUPT_COUNT_MAX];

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
