// ----------------------------------------------------------------------------
// Field.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_FIELD_H_
#define _MODULAR_SERVER_FIELD_H_
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "SavedVariable.h"
#include "JsonStream.h"
#include "Vector.h"
#include "Functor.h"

#include "Parameter.h"
#include "Constants.h"

#include "Streaming.h"


namespace modular_server
{
class Field
{
public:
  Field();
  template <typename T>
  Field(const ConstantString & name,
        const T & default_value);
  template <size_t N>
  Field(const ConstantString & name,
        const long (&default_value)[N]):
    parameter_(name),
    saved_variable_(default_value,N)
  {
    parameter_.setTypeLong();
    parameter_.setArrayLengthRange(1,N);
  };
  template <size_t N>
  Field(const ConstantString & name,
        const double (&default_value)[N]) :
    parameter_(name),
    saved_variable_(default_value,N)
  {
    parameter_.setTypeDouble();
    parameter_.setArrayLengthRange(1,N);
  }
  template <size_t N>
  Field(const ConstantString & name,
        const bool (&default_value)[N]) :
    parameter_(name),
    saved_variable_(default_value,N)
  {
    parameter_.setTypeBool();
    parameter_.setArrayLengthRange(1,N);
  }
  template <size_t N>
  Field(const ConstantString & name,
        const char (&default_value)[N]) :
    parameter_(name),
    saved_variable_(default_value,N)
  {
    parameter_.setTypeString();
    string_saved_as_char_array_ = true;
  }
  void setUnits(const ConstantString & name);
  void setRange(const long min, const long max);
  void setRange(const double min, const double max);
  template <size_t N>
  void setSubset(const constants::SubsetMemberType (&subset)[N])
  {
    parameter_.setSubset(subset);
  }
  void attachPreSetValueCallback(const Functor0 & callback);
  void attachPreSetElementValueCallback(const Functor1<const size_t> & callback);
  void attachPostSetValueCallback(const Functor0 & callback);
  void attachPostSetElementValueCallback(const Functor1<const size_t> & callback);
private:
  Parameter parameter_;
  SavedVariable saved_variable_;
  Functor0 pre_set_value_callback_;
  Functor1<const size_t> pre_set_element_value_callback_;
  Functor0 post_set_value_callback_;
  Functor1<const size_t> post_set_element_value_callback_;
  bool string_saved_as_char_array_;

  // Saved Variable Methods
  template <typename T>
  bool getDefaultValue(T & value);
  template <typename T>
  bool setDefaultValue(T & default_value)
  {
    return saved_variable_.setDefaultValue(default_value);
  };
  template <size_t N>
  bool getDefaultValue(long (&value)[N])
  {
    if ((getType() != JsonStream::ARRAY_TYPE) ||
        (getArrayElementType() != JsonStream::LONG_TYPE) ||
        (getArrayLength() != N))
    {
      return false;
    }
    return saved_variable_.getDefaultValue(value);
  };
  template <size_t N>
  bool getDefaultValue(double (&value)[N])
  {
    if ((getType() != JsonStream::ARRAY_TYPE) ||
        (getArrayElementType() != JsonStream::DOUBLE_TYPE) ||
        (getArrayLength() != N))
    {
      return false;
    }
    return saved_variable_.getDefaultValue(value);
  };
  template <size_t N>
  bool getDefaultValue(bool (&value)[N])
  {
    if ((getType() != JsonStream::ARRAY_TYPE) ||
        (getArrayElementType() != JsonStream::BOOL_TYPE) ||
        (getArrayLength() != N))
    {
      return false;
    }
    return saved_variable_.getDefaultValue(value);
  };
  template <typename T>
  bool getDefaultElementValue(T & value, const size_t element_index);
  template <typename T>
  bool setValue(const T & value);
  template <size_t N>
  bool setValue(const long (&value)[N])
  {
    bool success = false;
    preSetValueCallback();
    if ((getType() == JsonStream::ARRAY_TYPE) &&
        (getArrayElementType() == JsonStream::LONG_TYPE) &&
        (getArrayLength() == N))
    {
      for (size_t i=0;i<N;++i)
      {
        success = setElementValue(value[i],i);
        if (!success)
        {
          break;
        }
      }
    }
    postSetValueCallback();
    return success;
  };
  template <size_t N>
  bool setValue(const double (&value)[N])
  {
    bool success = false;
    preSetValueCallback();
    if ((getType() == JsonStream::ARRAY_TYPE) &&
        (getArrayElementType() == JsonStream::DOUBLE_TYPE) &&
        (getArrayLength() &= N))
    {
      for (size_t i=0;i<N;++i)
      {
        success = setElementValue(value[i],i);
        if (!success)
        {
          break;
        }
      }
    }
    postSetValueCallback();
    return success;
  };
  template <size_t N>
  bool setValue(const bool (&value)[N])
  {
    bool success = false;
    preSetValueCallback();
    if ((getType() == JsonStream::ARRAY_TYPE) &&
        (getArrayElementType() == JsonStream::BOOL_TYPE) &&
        (getArrayLength() == N))
    {
      for (size_t i=0;i<N;++i)
      {
        success = setElementValue(value[i],i);
        if (!success)
        {
          break;
        }
      }
    }
    postSetValueCallback();
    return success;
  };
  template <typename T>
  bool setElementValue(const T & value, const size_t element_index);
  template <typename T>
  bool getValue(T & value);
  template <size_t N>
  bool getValue(long (&value)[N])
  {
    if ((getType() != JsonStream::ARRAY_TYPE) ||
        (getArrayElementType() != JsonStream::LONG_TYPE) ||
        (getArrayLength() != N))
    {
      return false;
    }
    return saved_variable_.getValue(value);
  };
  template <size_t N>
  bool getValue(double (&value)[N])
  {
    if ((getType() != JsonStream::ARRAY_TYPE) ||
        (getArrayElementType() != JsonStream::DOUBLE_TYPE) ||
        (getArrayLength() != N))
    {
      return false;
    }
    return saved_variable_.getValue(value);
  };
  template <size_t N>
  bool getValue(bool (&value)[N])
  {
    if ((getType() != JsonStream::ARRAY_TYPE) ||
        (getArrayElementType() != JsonStream::BOOL_TYPE) ||
        (getArrayLength() != N))
    {
      return false;
    }
    return saved_variable_.getValue(value);
  };
  template <typename T>
  bool getElementValue(T & value, const size_t element_index);
  void setValueToDefault();
  bool valueIsDefault();
  size_t getArrayLength();
  Parameter & getParameter();
  const ConstantString & getName();
  JsonStream::JsonTypes getType();
  JsonStream::JsonTypes getArrayElementType();
  bool stringIsSavedAsCharArray();
  int findSubsetValueIndex(const long value);
  int findSubsetValueIndex(const char * value);
  Vector<const constants::SubsetMemberType> & getSubset();
  void preSetValueCallback();
  void preSetElementValueCallback(const size_t element_index);
  void postSetValueCallback();
  void postSetElementValueCallback(const size_t element_index);
  friend class Server;
};
}

#endif
