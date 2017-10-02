// ----------------------------------------------------------------------------
// CallbackTester.h
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef CALLBACK_TESTER_H
#define CALLBACK_TESTER_H
#include <Functor.h>
#include <ModularServer.h>

#include "Constants.h"
#include "NonBlockBlink.h"


class CallbackTester
{
public:
  void setup();
  void update();

private:
  modular_server::ModularServer modular_server_;

  modular_server::Interrupt interrupts_[constants::INTERRUPT_COUNT_MAX];

  modular_server::Property properties_[constants::PROPERTY_COUNT_MAX];
  modular_server::Parameter parameters_[constants::PARAMETER_COUNT_MAX];
  modular_server::Function functions_[constants::FUNCTION_COUNT_MAX];
  modular_server::Callback callbacks_[constants::CALLBACK_COUNT_MAX];

  // Handlers
  void setLedOnHandler(modular_server::Interrupt * interrupt_ptr);
  void setLedOffHandler(modular_server::Interrupt * interrupt_ptr);
  void blinkLedHandler(modular_server::Interrupt * interrupt_ptr);
};

#endif
