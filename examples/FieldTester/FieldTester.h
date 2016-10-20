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

  // Functors
  void getDoubledFunctor();
  void getBoolFunctor();
  void getLongArrayFixedFunctor();
  void getLongArrayVariableFunctor();
  void setLongArrayFixedFunctor();
  void setLongArrayVariableFunctor();
  void setLongArrayParameterFunctor();
  void getStringAllFunctor();
  void getStringSomeFunctor();
  void getCountFunctor();
  void getCountArrayFunctor();
  void getDirectionFunctor();
  void getDirectionArrayFunctor();
  void checkModeFunctor();
  void incrementModeFunctor();
  void preSetFieldValueFunctor();
  void postSetFieldValueFunctor();
  void preSetFieldElementValueFunctor(const size_t element_index);
  void postSetFieldElementValueFunctor(const size_t element_index);

};

#endif
