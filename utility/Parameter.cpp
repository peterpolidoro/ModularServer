// ----------------------------------------------------------------------------
// Parameter.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Parameter.h"


namespace modular_server
{
Functor1wRet<const ConstantString &, ArduinoJson::JsonVariant> Parameter::get_value_functor_;

// public
Parameter::Parameter()
{
  setName(constants::empty_constant_string);
  setUnits(constants::empty_constant_string);
  type_ = JsonStream::LONG_TYPE;
  array_element_type_ = JsonStream::LONG_TYPE;
  range_is_set_ = false;
  array_length_range_is_set_ = false;
  subset_is_set_ = false;
}

void Parameter::setUnits(const ConstantString & units)
{
  units_ptr_ = &units;
}

void Parameter::setTypeLong()
{
  if (type_ != JsonStream::ARRAY_TYPE)
  {
    type_ = JsonStream::LONG_TYPE;
  }
  else
  {
    array_element_type_ = JsonStream::LONG_TYPE;
  }
}

void Parameter::setTypeDouble()
{
  if (type_ != JsonStream::ARRAY_TYPE)
  {
    type_ = JsonStream::DOUBLE_TYPE;
  }
  else
  {
    array_element_type_ = JsonStream::DOUBLE_TYPE;
  }
}

void Parameter::setTypeBool()
{
  if (type_ != JsonStream::ARRAY_TYPE)
  {
    type_ = JsonStream::BOOL_TYPE;
  }
  else
  {
    array_element_type_ = JsonStream::BOOL_TYPE;
  }
}

void Parameter::setTypeString()
{
  if (type_ != JsonStream::ARRAY_TYPE)
  {
    type_ = JsonStream::STRING_TYPE;
  }
  else
  {
    array_element_type_ = JsonStream::STRING_TYPE;
  }
}

void Parameter::setTypeObject()
{
  type_ = JsonStream::OBJECT_TYPE;
}

void Parameter::setTypeArray()
{
  if (type_ != JsonStream::ARRAY_TYPE)
  {
    array_element_type_ = type_;
    type_ = JsonStream::ARRAY_TYPE;
  }
}

void Parameter::setTypeAny()
{
  if (type_ != JsonStream::ARRAY_TYPE)
  {
    type_ = JsonStream::ANY_TYPE;
  }
  else
  {
    array_element_type_ = JsonStream::ANY_TYPE;
  }
}

void Parameter::setType(JsonStream::JsonTypes type)
{
  if (type_ != JsonStream::ARRAY_TYPE)
  {
    type_ = type;
  }
  else
  {
    array_element_type_ = type;
  }
}

void Parameter::setRange(const double min, const double max)
{
  min_.d = min;
  max_.d = max;
  setTypeDouble();
  range_is_set_ = true;
}

void Parameter::setRange(const float min, const float max)
{
  min_.d = (double)min;
  max_.d = (double)max;
  setTypeDouble();
  range_is_set_ = true;
}

void Parameter::setRange(const constants::NumberType min, const constants::NumberType max)
{
  min_ = min;
  max_ = max;
  range_is_set_ = true;
}

void Parameter::removeRange()
{
  range_is_set_ = false;
}

void Parameter::setArrayLengthRange(const size_t array_length_min,
                                    const size_t array_length_max)
{
  setTypeArray();
  array_length_min_ = array_length_min;
  array_length_max_ = array_length_max;
  array_length_range_is_set_ = true;
}

void Parameter::removeArrayLengthRange()
{
  array_length_range_is_set_ = false;
}

void Parameter::setSubset(constants::SubsetMemberType * subset, size_t max_size, size_t size)
{
  subset_.setStorage(subset,max_size,size);
  subset_is_set_ = true;
}

void Parameter::addValueToSubset(constants::SubsetMemberType & value)
{
  if (subset_is_set_)
  {
    subset_.push_back(value);
  }
}

void Parameter::removeSubset()
{
  subset_is_set_ = false;
}

template <>
bool Parameter::getValue<long>(long & value)
{
  if ((getType() == JsonStream::LONG_TYPE) ||
      (getType() == JsonStream::DOUBLE_TYPE) ||
      (getType() == JsonStream::BOOL_TYPE) ||
      (getType() == JsonStream::ANY_TYPE))
  {
    long v = get_value_functor_(getName());
    value = v;
    return true;
  }
  return false;
}

template <>
bool Parameter::getValue<double>(double & value)
{
  if ((getType() == JsonStream::LONG_TYPE) ||
      (getType() == JsonStream::DOUBLE_TYPE) ||
      (getType() == JsonStream::BOOL_TYPE) ||
      (getType() == JsonStream::ANY_TYPE))
  {
    double v = get_value_functor_(getName());
    value = v;
    return true;
  }
  return false;
}

template <>
bool Parameter::getValue<float>(float & value)
{
  if ((getType() == JsonStream::LONG_TYPE) ||
      (getType() == JsonStream::DOUBLE_TYPE) ||
      (getType() == JsonStream::BOOL_TYPE) ||
      (getType() == JsonStream::ANY_TYPE))
  {
    double v = get_value_functor_(getName());
    value = v;
    return true;
  }
  return false;
}

template <>
bool Parameter::getValue<bool>(bool & value)
{
  if ((getType() == JsonStream::LONG_TYPE) ||
      (getType() == JsonStream::DOUBLE_TYPE) ||
      (getType() == JsonStream::BOOL_TYPE) ||
      (getType() == JsonStream::ANY_TYPE))
  {
    bool v = get_value_functor_(getName());
    value = v;
    return true;
  }
  return false;
}

bool Parameter::getValue(const char * & value)
{
  if ((getType() != JsonStream::STRING_TYPE) && (getType() != JsonStream::ANY_TYPE))
  {
    return false;
  }
  value = get_value_functor_(getName());
  return true;
}

bool Parameter::getValue(ArduinoJson::JsonArray * & value)
{
  if (getType() != JsonStream::ARRAY_TYPE)
  {
    return false;
  }
  ArduinoJson::JsonArray & array = get_value_functor_(getName());
  value = &array;
  return true;
}

bool Parameter::getValue(ArduinoJson::JsonObject * & value)
{
  if (getType() != JsonStream::OBJECT_TYPE)
  {
    return false;
  }
  ArduinoJson::JsonObject & object = get_value_functor_(getName());
  value = &object;
  return true;
}

// private
Parameter::Parameter(const ConstantString & name)
{
  setName(name);
  setUnits(constants::empty_constant_string);
  type_ = JsonStream::LONG_TYPE;
  array_element_type_ = JsonStream::LONG_TYPE;
  range_is_set_ = false;
  array_length_range_is_set_ = false;
  subset_is_set_ = false;
}

const ConstantString & Parameter::getUnits()
{
  return *units_ptr_;
}

JsonStream::JsonTypes Parameter::getType()
{
  return type_;
}

JsonStream::JsonTypes Parameter::getArrayElementType()
{
  return array_element_type_;
}

bool Parameter::rangeIsSet()
{
  return range_is_set_;
}

bool Parameter::valueInRange(const double value)
{
  bool in_range = true;
  if (rangeIsSet())
  {
    double min = getMin().d;
    double max = getMax().d;
    if ((value < min) || (value > max))
    {
      in_range = false;
    }
  }
  return in_range;
}

bool Parameter::valueInRange(const float value)
{
  bool in_range = true;
  if (rangeIsSet())
  {
    double min = getMin().d;
    double max = getMax().d;
    if (((double)value < min) || ((double)value > max))
    {
      in_range = false;
    }
  }
  return in_range;
}

constants::NumberType Parameter::getMin()
{
  return min_;
}

constants::NumberType Parameter::getMax()
{
  return max_;
}

size_t Parameter::getArrayLengthMin()
{
  return array_length_min_;
}

size_t Parameter::getArrayLengthMax()
{
  return array_length_max_;
}

bool Parameter::arrayLengthRangeIsSet()
{
  return array_length_range_is_set_;
}

bool Parameter::arrayLengthInRange(const size_t array_length)
{
  bool in_range = true;
  if (arrayLengthRangeIsSet())
  {
    size_t array_length_min = getArrayLengthMin();
    size_t array_length_max = getArrayLengthMax();
    if ((array_length < array_length_min) || (array_length > array_length_max))
    {
      in_range = false;
    }
  }
  return in_range;
}

bool Parameter::subsetIsSet()
{
  return subset_is_set_;
}

int Parameter::findSubsetValueIndex(const long value)
{
  int value_index = -1;
  if (subsetIsSet())
  {
    for(size_t i=0; i<subset_.size(); ++i)
    {
      if (value == subset_[i].l)
      {
        value_index = i;
        break;
      }
    }
  }
  return value_index;
}

int Parameter::findSubsetValueIndex(const char * value)
{
  int value_index = -1;
  if (subsetIsSet())
  {
    for(size_t i=0; i<subset_.size(); ++i)
    {
      if (value == *subset_[i].cs_ptr)
      {
        value_index = i;
        break;
      }
    }
  }
  return value_index;
}

bool Parameter::valueInSubset(const long value)
{
  bool in_subset = true;
  if (subsetIsSet())
  {
    int subset_value_index = findSubsetValueIndex(value);
    in_subset = ((subset_value_index >= 0) ? true : false);
  }
  return in_subset;
}

bool Parameter::valueInSubset(const char * value)
{
  bool in_subset = true;
  if (subsetIsSet())
  {
    int subset_value_index = findSubsetValueIndex(value);
    in_subset = ((subset_value_index >= 0) ? true : false);
  }
  return in_subset;
}

Vector<constants::SubsetMemberType> & Parameter::getSubset()
{
  return subset_;
}

}
