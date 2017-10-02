// ----------------------------------------------------------------------------
// MinimalDevice.h
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef MINIMAL_DEVICE_H
#define MINIMAL_DEVICE_H
#include <Functor.h>
#include <ModularServer.h>

#include "Constants.h"


class MinimalDevice
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
};

#endif
