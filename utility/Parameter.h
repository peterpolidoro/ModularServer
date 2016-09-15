// ----------------------------------------------------------------------------
// Parameter.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_DEVICE_PARAMETER_H_
#define _MODULAR_DEVICE_PARAMETER_H_
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "ConstantVariable.h"
#include "JsonStream.h"
#include "Vector.h"
#include "Constants.h"


namespace ModularDevice
{
class Parameter
{
public:
  Parameter();
  Parameter(const ConstantString &name);
  void setName(const ConstantString &name);
  void setUnits(const ConstantString &name);
  void setTypeLong();
  void setTypeDouble();
  void setTypeBool();
  void setTypeString();
  void setTypeObject();
  void setTypeArray();
  void setTypeValue();
  template <typename T>
  void setRange(const T min, const T max)
  {
    min_.l = (long)min;
    max_.l = (long)max;
    setTypeLong();
    range_is_set_ = true;
  }
  void setRange(const double min, const double max);
  void setRange(const float min, const float max);
  void removeRange();
  void setArrayLengthRange(const size_t array_length_min,
                           const size_t array_length_max);
  void removeArrayLengthRange();
  template <size_t N>
  void setSubset(const constants::SubsetMemberType (&subset)[N])
  {
    subset_.setStorage(subset,N);
    subset_is_set_ = true;
  }
  void removeSubset();
private:
  const ConstantString *name_ptr_;
  const ConstantString *units_ptr_;
  JsonStream::JsonTypes type_;
  JsonStream::JsonTypes array_element_type_;
  constants::NumberType min_;
  constants::NumberType max_;
  bool range_is_set_;
  size_t array_length_min_;
  size_t array_length_max_;
  bool array_length_range_is_set_;
  Vector<const constants::SubsetMemberType> subset_;
  bool subset_is_set_;
  bool compareName(const char *name_to_compare);
  bool compareName(const ConstantString &name_to_compare);
  const ConstantString& getName();
  const ConstantString& getUnits();
  JsonStream::JsonTypes getType();
  JsonStream::JsonTypes getArrayElementType();
  bool rangeIsSet();
  template <typename T>
  bool valueInRange(const T value)
  {
    bool in_range = true;
    if (rangeIsSet())
    {
      long min = getMin().l;
      long max = getMax().l;
      if (((long)value < min) || ((long)value > max))
      {
        in_range = false;
      }
    }
    return in_range;
  }
  bool valueInRange(const double value);
  bool valueInRange(const float value);
  constants::NumberType getMin();
  constants::NumberType getMax();
  size_t getArrayLengthMin();
  size_t getArrayLengthMax();
  bool arrayLengthRangeIsSet();
  bool arrayLengthInRange(const size_t array_length);
  bool subsetIsSet();
  bool valueInSubset(const long value);
  bool valueInSubset(const char *value);
  Vector<const constants::SubsetMemberType>& getSubset();
  friend class Field;
  friend class Method;
  friend class Server;
};
}
#endif
