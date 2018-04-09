// ----------------------------------------------------------------------------
// BoardLedController.h
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef BOARD_LED_CONTROLLER_H
#define BOARD_LED_CONTROLLER_H
#include <Functor.h>
#include <ModularServer.h>

#include "Constants.h"
#include "Blinker.h"


class BoardLedController
{
public:
  void setup();
  void startServer();
  void update();

private:
  modular_server::ModularServer modular_server_;

  modular_server::Pin pins_[constants::PIN_COUNT_MAX];

  modular_server::Property properties_[constants::PROPERTY_COUNT_MAX];
  modular_server::Parameter parameters_[constants::PARAMETER_COUNT_MAX];
  modular_server::Function functions_[constants::FUNCTION_COUNT_MAX];
  modular_server::Callback callbacks_[constants::CALLBACK_COUNT_MAX];

  Blinker blinker_;

  // Handlers
  void setLedOnHandler();
  void setLedOffHandler();
  void getLedPinNumberHandler();
  void blinkLedHandler();

};

#endif
