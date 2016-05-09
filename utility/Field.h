// ----------------------------------------------------------------------------
// Field.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_DEVICE_FIELD_H_
#define _MODULAR_DEVICE_FIELD_H_
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "SavedVariable.h"
#include "JsonStream.h"
#include "Parameter.h"
#include "Constants.h"


namespace ModularDevice
{
class Field
{
public:
  Field();
  template <typename T>
  Field(const ConstantString &name,
        const T &default_value);
  template <typename T>
  Field(const ConstantString &name,
        const T default_value[],
        const unsigned int array_length);

  // Parameter Methods
  void setRange(const long min, const long max);
  void removeRange();

  // Saved Variable Methods
  template<typename T>
  void getDefaultValue(T &value);
  template<typename T>
  void getDefaultElementValue(T &value, const unsigned int element_index);
  template<typename T>
  void setValue(const T &value);
  template<typename T>
  void setElementValue(const T &value, const unsigned int element_index);
  template<typename T>
  void getValue(T &value);
  template<typename T>
  void getElementValue(T &value, const unsigned int element_index);
  void setDefaultValue();
  bool isDefaultValue();
private:
  Parameter parameter_;
  SavedVariable saved_variable_;
  Parameter& getParameter();
  friend class Server;
};
}

#endif
