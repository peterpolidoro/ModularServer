// ----------------------------------------------------------------------------
// MinimalDevice.h
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
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
  void startServer();
  void update();

private:
  modular_server::ModularServer modular_server_;

  modular_server::Pin pins_[constants::PIN_COUNT_MAX];

  modular_server::Property properties_[constants::PROPERTY_COUNT_MAX];
  modular_server::Parameter parameters_[constants::PARAMETER_COUNT_MAX];
  modular_server::Function functions_[constants::FUNCTION_COUNT_MAX];
  modular_server::Callback callbacks_[constants::CALLBACK_COUNT_MAX];

  // Handlers
};

#endif
