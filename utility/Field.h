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
  template <size_t N>
  Field(const ConstantString &name,
        const long (&default_value)[N]):
    parameter_(name),
    saved_variable_(default_value,N)
  {
    parameter_.setTypeLong();
    parameter_.setArrayLengthRange(N,N);
  };
  template <size_t N>
  Field(const ConstantString &name,
        const bool (&default_value)[N]) :
    parameter_(name),
    saved_variable_(default_value,N)
  {
    parameter_.setTypeBool();
    parameter_.setArrayLengthRange(N,N);
  }

  // Parameter Methods
  void setRange(const long min, const long max);

  // Saved Variable Methods
  template<typename T>
  void getDefaultValue(T &value);
  template<size_t N>
  void getDefaultValue(long (&value)[N])
  {
    saved_variable_.getDefaultValue(value);
  };
  template<size_t N>
  void getDefaultValue(bool (&value)[N])
  {
    saved_variable_.getDefaultValue(value);
  };
  template<typename T>
  void getDefaultElementValue(T &value, const unsigned int element_index);
  template<typename T>
  void setValue(const T &value);
  template<size_t N>
  void setValue(const long (&value)[N])
  {
    saved_variable_.setValue(value);
  };
  template<size_t N>
  void setValue(const bool (&value)[N])
  {
    saved_variable_.setValue(value);
  };
  template<typename T>
  void setElementValue(const T &value, const unsigned int element_index);
  template<typename T>
  void getValue(T &value);
  template<size_t N>
  void getValue(long (&value)[N])
  {
    saved_variable_.getValue(value);
  };
  template<size_t N>
  void getValue(bool (&value)[N])
  {
    saved_variable_.getValue(value);
  };
  template<typename T>
  void getElementValue(T &value, const unsigned int element_index);
  void setDefaultValue();
  bool isDefaultValue();
  unsigned int getArrayLength();
private:
  Parameter parameter_;
  SavedVariable saved_variable_;
  Parameter& getParameter();
  friend class Server;
};
}

#endif
