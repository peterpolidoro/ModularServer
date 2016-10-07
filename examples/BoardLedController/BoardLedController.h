// ----------------------------------------------------------------------------
// BoardLedController.h
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef BOARD_LED_CONTROLLER_H
#define BOARD_LED_CONTROLLER_H
#include "Functor.h"
#include "ModularServer.h"

#include "Constants.h"
#include "NonBlockBlink.h"


class BoardLedController
{
public:
  void setup();
  void update();

private:
  modular_server::ModularServer modular_server_;

  modular_server::Field fields_[constants::FIELD_COUNT_MAX];
  modular_server::Parameter parameters_[constants::PARAMETER_COUNT_MAX];
  modular_server::Method methods_[constants::METHOD_COUNT_MAX];

  // Callbacks
  void setLedOnCallback();
  void setLedOffCallback();
  void getLedPinCallback();
  void blinkLedCallback();

};

#endif
