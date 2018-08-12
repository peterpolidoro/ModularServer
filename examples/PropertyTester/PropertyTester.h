// ----------------------------------------------------------------------------
// PropertyTester.h
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef PROPERTY_TESTER_H
#define PROPERTY_TESTER_H
#include <Functor.h>
#include <ModularServer.h>

#include "Constants.h"


class PropertyTester
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
  void getDoubledHandler();
  void getBoolHandler();
  void getLongArrayFixedHandler();
  void getLongArrayVariableHandler();
  void setLongArrayFixedHandler();
  void setLongArrayVariableHandler();
  void setLongArrayParameterHandler();
  void getStringAllHandler();
  void getStringSomeHandler();
  void getCountHandler();
  void getCountArrayHandler();
  void getDirectionHandler();
  void getDirectionArrayHandler();
  void checkModeHandler();
  void incrementModeHandler();
  void setNewDoubleRangeHandler();
  void setNewOddSubsetHandler();
  void setNewOddDefaultHandler();
  void preSetPropertyValueHandler();
  void postSetPropertyValueHandler();
  void preSetPropertyElementValueHandler(const size_t element_index);
  void postSetPropertyElementValueHandler(const size_t element_index);

};

#endif
