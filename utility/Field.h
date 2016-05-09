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

  // Saved Variable Methods
  template<typename T>
  void getDefaultValue(T &value);
  template<typename T>
  void getDefaultElementValue(T &value, unsigned int element_index);
  template<typename T>
  void setValue(const T &value);
  template<typename T>
  void setElementValue(const T &value, const unsigned int element_index);
  template<typename T>
  void getValue(T &value);
  template<typename T>
  void getElementValue(T &value, unsigned int element_index);
  void setDefaultValue();
  bool isDefaultValue();

  // Parameter Methods
  template <typename T>
  void setRange(const T min, const T max);
  {
    min_.l = (long)min;
    max_.l = (long)max;
    setTypeLong();
    range_is_set_ = true;
  }
  void removeRange();
private:
  Parameter parameter_;
  SavedVariable saved_variable_;

  // Parameter Methods
  const ConstantString& getName();
  const ConstantString& getUnits();
  JsonStream::JsonTypes getType();
  JsonStream::JsonTypes getArrayElementType();
  bool rangeIsSet();
  constants::NumberType getMin();
  constants::NumberType getMax();

  friend class Server;
};
}

#endif
