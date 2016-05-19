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
        const double (&default_value)[N]) :
    parameter_(name),
    saved_variable_(default_value,N)
  {
    parameter_.setTypeDouble();
    parameter_.setArrayLengthRange(N,N);
  }
  template <size_t N>
  Field(const ConstantString &name,
        const bool (&default_value)[N]) :
    parameter_(name),
    saved_variable_(default_value,N)
  {
    parameter_.setTypeBool();
    parameter_.setArrayLengthRange(N,N);
  }
  template <size_t N>
  Field(const ConstantString &name,
        const char (&default_value)[N]) :
    parameter_(name),
    saved_variable_(default_value,N)
  {
    parameter_.setTypeString();
    parameter_.setArrayLengthRange(1,N);
  }

  void setRange(const long min, const long max);
  void setRange(const double min, const double max);

private:
  Parameter parameter_;
  SavedVariable saved_variable_;

  // Saved Variable Methods
  template<typename T>
  bool getDefaultValue(T &value);
  template<size_t N>
  bool getDefaultValue(long (&value)[N])
  {
    return saved_variable_.getDefaultValue(value);
  };
  template<size_t N>
  bool getDefaultValue(double (&value)[N])
  {
    return saved_variable_.getDefaultValue(value);
  };
  template<size_t N>
  bool getDefaultValue(bool (&value)[N])
  {
    return saved_variable_.getDefaultValue(value);
  };
  template<typename T>
  bool getDefaultElementValue(T &value, const size_t element_index);
  template<typename T>
  bool setValue(const T &value);
  template<size_t N>
  bool setValue(const long (&value)[N])
  {
    if (getArrayLength() != N)
    {
      return false;
    }
    bool success;
    for (size_t i=0;i<N;++i)
    {
      success = setElementValue(value[i],i);
      if (!success)
      {
        return false;
      }
    }
    return true;
  };
  template<size_t N>
  bool setValue(const double (&value)[N])
  {
    if (getArrayLength() != N)
    {
      return false;
    }
    bool success;
    for (size_t i=0;i<N;++i)
    {
      success = setElementValue(value[i],i);
      if (!success)
      {
        return false;
      }
    }
    return true;
  };
  template<size_t N>
  bool setValue(const bool (&value)[N])
  {
    if (getArrayLength() != N)
    {
      return false;
    }
    return saved_variable_.setValue(value);
  };
  template<typename T>
  bool setElementValue(const T &value, const size_t element_index);
  template<typename T>
  bool getValue(T &value);
  template<size_t N>
  bool getValue(long (&value)[N])
  {
    if (getArrayLength() != N)
    {
      return false;
    }
    return saved_variable_.getValue(value);
  };
  template<size_t N>
  bool getValue(double (&value)[N])
  {
    if (getArrayLength() != N)
    {
      return false;
    }
    return saved_variable_.getValue(value);
  };
  template<size_t N>
  bool getValue(bool (&value)[N])
  {
    if (getArrayLength() != N)
    {
      return false;
    }
    return saved_variable_.getValue(value);
  };
  template<typename T>
  bool getElementValue(T &value, const size_t element_index);
  void setDefaultValue();
  bool isDefaultValue();
  size_t getArrayLength();
  Parameter& getParameter();
  friend class Server;
};
}

#endif
