// ----------------------------------------------------------------------------
// FieldTester.h
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef FIELD_TESTER_H
#define FIELD_TESTER_H
#include "Functor.h"
#include "ModularServer.h"

#include "Constants.h"


class FieldTester
{
public:
  void setup();
  void update();

private:
  modular_server::ModularServer modular_server_;

  modular_server::Field fields_[constants::FIELD_COUNT_MAX];
  modular_server::Parameter parameters_[constants::PARAMETER_COUNT_MAX];
  modular_server::Method methods_[constants::METHOD_COUNT_MAX];
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
  void preSetFieldValueHandler();
  void postSetFieldValueHandler();
  void preSetFieldElementValueHandler(const size_t element_index);
  void postSetFieldElementValueHandler(const size_t element_index);

};

#endif
