// ----------------------------------------------------------------------------
// PinTester.h
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef PIN_TESTER_H
#define PIN_TESTER_H
#include <Functor.h>
#include <ModularServer.h>

#include "Constants.h"


class PinTester
{
public:
  void setup();
  void startServer();
  void update();

  void testPin();

private:
  modular_server::ModularServer modular_server_;

  modular_server::Pin pins_[constants::PIN_COUNT_MAX];

  modular_server::Property properties_[constants::PROPERTY_COUNT_MAX];
  modular_server::Parameter parameters_[constants::PARAMETER_COUNT_MAX];
  modular_server::Function functions_[constants::FUNCTION_COUNT_MAX];
  modular_server::Callback callbacks_[constants::CALLBACK_COUNT_MAX];

  // Handlers
  void testPinHandler(modular_server::Pin * pin_ptr);
};

#endif
