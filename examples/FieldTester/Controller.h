// ----------------------------------------------------------------------------
// Controller.h
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Functor.h"
#include "ModularServer.h"

#include "Constants.h"


class Controller
{
public:
  Controller();
  void setup();
  void update();

private:
  ModularDevice::ModularServer modular_server_;

  ModularDevice::Field fields_[constants::FIELD_COUNT_MAX];
  ModularDevice::Parameter parameters_[constants::PARAMETER_COUNT_MAX];
  ModularDevice::Method methods_[constants::METHOD_COUNT_MAX];

  // Callbacks
  void getDoubledCallback();
  void getBoolCallback();
  void getLongArrayFixedCallback();
  void getLongArrayVariableCallback();
  void setLongArrayFixedCallback();
  void setLongArrayVariableCallback();
  void setLongArrayParameterCallback();
  void getStringAllCallback();
  void getStringSomeCallback();
  void getCountCallback();
  void getCountArrayCallback();
  void getDirectionCallback();
  void getDirectionArrayCallback();
  void checkModeCallback();
  void incrementModeCallback();
  void preSetFieldValueCallback();
  void postSetFieldValueCallback();
  void preSetFieldElementValueCallback(const size_t element_index);
  void postSetFieldElementValueCallback(const size_t element_index);

};

#endif
