// ----------------------------------------------------------------------------
// Field.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Field.h"


namespace modular_server
{
// public
Field::Field()
{
}

void Field::setUnits(const ConstantString & name)
{
  parameter_.setUnits(name);
}

void Field::setRange(const long min, const long max)
{
  parameter_.setRange(min,max);
}

void Field::setRange(const double min, const double max)
{
  parameter_.setRange(min,max);
}

void Field::setSubset(const constants::SubsetMemberType * subset, size_t max_size, size_t size)
{
  parameter_.setSubset(subset,max_size,size);
}

void Field::addValueToSubset(const constants::SubsetMemberType & value)
{
  parameter_.addValueToSubset(value);
}

void Field::attachPreSetValueFunctor(const Functor0 & functor)
{
  pre_set_value_functor_ = functor;
}

void Field::attachPreSetElementValueFunctor(const Functor1<const size_t> & functor)
{
  pre_set_element_value_functor_ = functor;
}

void Field::attachPostSetValueFunctor(const Functor0 & functor)
{
  post_set_value_functor_ = functor;
}

void Field::attachPostSetElementValueFunctor(const Functor1<const size_t> & functor)
{
  post_set_element_value_functor_ = functor;
}

template <>
bool Field::getValue<long>(long & value)
{
  if (getType() != JsonStream::LONG_TYPE)
  {
    return false;
  }
  return saved_variable_.getValue(value);
}

template <>
bool Field::getValue<double>(double & value)
{
  if (getType() != JsonStream::DOUBLE_TYPE)
  {
    return false;
  }
  return saved_variable_.getValue(value);
}

template <>
bool Field::getValue<bool>(bool & value)
{
  if (getType() != JsonStream::BOOL_TYPE)
  {
    return false;
  }
  return saved_variable_.getValue(value);
}

bool Field::getValue(const ConstantString * & value)
{
  if ((getType() != JsonStream::STRING_TYPE) ||
      stringIsSavedAsCharArray())
  {
    value = NULL;
    return false;
  }
  return saved_variable_.getValue(value);
}

template <>
bool Field::getElementValue<long>(const size_t element_index, long & value)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::LONG_TYPE))
  {
    return false;
  }
  return saved_variable_.getElementValue(element_index,value);
}

template <>
bool Field::getElementValue<double>(const size_t element_index, double & value)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::DOUBLE_TYPE))
  {
    return false;
  }
  return saved_variable_.getElementValue(element_index,value);
}

template <>
bool Field::getElementValue<bool>(const size_t element_index, bool & value)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::BOOL_TYPE))
  {
    return false;
  }
  return saved_variable_.getElementValue(element_index,value);
}

template <>
bool Field::getElementValue<char>(const size_t element_index, char & value)
{
  if (getType() != JsonStream::STRING_TYPE)
  {
    return false;
  }
  if (!stringIsSavedAsCharArray())
  {
    const ConstantString * cs_ptr;
    getValue(cs_ptr);
    size_t str_len = cs_ptr->length();
    if (element_index <= str_len)
    {
      char str[str_len+1];
      cs_ptr->copy(str);
      value = str[element_index];
      return true;
    }
    else
    {
      return false;
    }
  }
  return saved_variable_.getElementValue(element_index,value);
}

template <>
bool Field::getDefaultValue<long>(long & value)
{
  if (getType() != JsonStream::LONG_TYPE)
  {
    return false;
  }
  return saved_variable_.getDefaultValue(value);
}

template <>
bool Field::getDefaultValue<double>(double & value)
{
  if (getType() != JsonStream::DOUBLE_TYPE)
  {
    return false;
  }
  return saved_variable_.getDefaultValue(value);
}

template <>
bool Field::getDefaultValue<bool>(bool & value)
{
  if (getType() != JsonStream::BOOL_TYPE)
  {
    return false;
  }
  return saved_variable_.getDefaultValue(value);
}

bool Field::getDefaultValue(const ConstantString * & value)
{
  if ((getType() != JsonStream::STRING_TYPE) ||
      stringIsSavedAsCharArray())
  {
    value = NULL;
    return false;
  }
  return saved_variable_.getDefaultValue(value);
}

template <>
bool Field::getDefaultElementValue<long>(const size_t element_index, long & value)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::LONG_TYPE))
  {
    return false;
  }
  return saved_variable_.getDefaultElementValue(element_index,value);
}

template <>
bool Field::getDefaultElementValue<double>(const size_t element_index, double & value)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::DOUBLE_TYPE))
  {
    return false;
  }
  return saved_variable_.getDefaultElementValue(element_index,value);
}

template <>
bool Field::getDefaultElementValue<bool>(const size_t element_index, bool & value)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::BOOL_TYPE))
  {
    return false;
  }
  return saved_variable_.getDefaultElementValue(element_index,value);
}

template <>
bool Field::getDefaultElementValue<char>(const size_t element_index, char & value)
{
  if (getType() != JsonStream::STRING_TYPE)
  {
    return false;
  }
  if (!stringIsSavedAsCharArray())
  {
    const ConstantString * cs_ptr;
    getDefaultValue(cs_ptr);
    size_t str_len = cs_ptr->length();
    if (element_index <= str_len)
    {
      char str[str_len+1];
      cs_ptr->copy(str);
      value = str[element_index];
      return true;
    }
    else
    {
      return false;
    }
  }
  return saved_variable_.getDefaultElementValue(element_index,value);
}

template <>
bool Field::setElementValue<long>(const size_t element_index, const long & value)
{
  bool success = false;
  preSetElementValueFunctor(element_index);
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::LONG_TYPE))
  {
    if (parameter_.rangeIsSet())
    {
      long min = parameter_.getMin().l;
      long max = parameter_.getMax().l;
      if ((value >= min) && (value <= max))
      {
        success = saved_variable_.setElementValue(element_index,value);
      }
    }
    else
    {
      success = saved_variable_.setElementValue(element_index,value);
    }
  }
  postSetElementValueFunctor(element_index);
  return success;
}

template <>
bool Field::setElementValue<double>(const size_t element_index, const double & value)
{
  bool success = false;
  preSetElementValueFunctor(element_index);
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::DOUBLE_TYPE))
  {
    if (parameter_.rangeIsSet())
    {
      double min = parameter_.getMin().d;
      double max = parameter_.getMax().d;
      if ((value >= min) && (value <= max))
      {
        success = saved_variable_.setElementValue(element_index,value);
      }
    }
    else
    {
      success = saved_variable_.setElementValue(element_index,value);
    }
  }
  postSetElementValueFunctor(element_index);
  return success;
}

template <>
bool Field::setElementValue<bool>(const size_t element_index, const bool & value)
{
  bool success = false;
  preSetElementValueFunctor(element_index);
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::BOOL_TYPE))
  {
    success = saved_variable_.setElementValue(element_index,value);
  }
  postSetElementValueFunctor(element_index);
  return success;
}

template <>
bool Field::setElementValue<char>(const size_t element_index, const char & value)
{
  bool success = false;
  preSetElementValueFunctor(element_index);
  if ((getType() == JsonStream::STRING_TYPE) &&
      (stringIsSavedAsCharArray()))
  {
    success = saved_variable_.setElementValue(element_index,value);
  }
  postSetElementValueFunctor(element_index);
  return success;
}

template <>
bool Field::setValue<long>(const long & value)
{
  bool success = false;
  preSetValueFunctor();
  if (getType() == JsonStream::LONG_TYPE)
  {
    if (parameter_.rangeIsSet())
    {
      long min = parameter_.getMin().l;
      long max = parameter_.getMax().l;
      if ((value >= min) && (value <= max))
      {
        success = saved_variable_.setValue(value);
      }
    }
    else
    {
      success = saved_variable_.setValue(value);
    }
  }
  postSetValueFunctor();
  return success;
}

template <>
bool Field::setValue<double>(const double & value)
{
  bool success = false;
  preSetValueFunctor();
  if (getType() == JsonStream::DOUBLE_TYPE)
  {
    if (parameter_.rangeIsSet())
    {
      double min = parameter_.getMin().d;
      double max = parameter_.getMax().d;
      if ((value >= min) && (value <= max))
      {
        success = saved_variable_.setValue(value);
      }
    }
    else
    {
      success = saved_variable_.setValue(value);
    }
  }
  postSetValueFunctor();
  return success;
}

template <>
bool Field::setValue<bool>(const bool & value)
{
  bool success = false;
  preSetValueFunctor();
  if (getType() == JsonStream::BOOL_TYPE)
  {
    success = saved_variable_.setValue(value);
  }
  postSetValueFunctor();
  return success;
}

template <>
bool Field::setValue<const ConstantString *>(const ConstantString * const & value)
{
  bool success = false;
  preSetValueFunctor();
  if ((getType() == JsonStream::STRING_TYPE) &&
      !stringIsSavedAsCharArray())
  {
    success = saved_variable_.setValue(value);
  }
  postSetValueFunctor();
  return success;
}

template <>
bool Field::setValue<ConstantString *>(ConstantString * const & value)
{
  bool success = false;
  preSetValueFunctor();
  if ((getType() == JsonStream::STRING_TYPE) &&
      !stringIsSavedAsCharArray())
  {
    success = saved_variable_.setValue(value);
  }
  postSetValueFunctor();
  return success;
}

bool Field::setValue(ArduinoJson::JsonArray & value)
{
  bool success = false;
  size_t array_length = getArrayLength();
  JsonStream::JsonTypes field_type = getType();
  if (field_type == JsonStream::ARRAY_TYPE)
  {
    size_t N = value.size();
    size_t array_length_min = min(array_length,N);
    JsonStream::JsonTypes array_element_type = getArrayElementType();
    preSetValueFunctor();
    switch (array_element_type)
    {
      case JsonStream::LONG_TYPE:
      {
        for (size_t i=0; i<array_length_min; ++i)
        {
          long v = value[i];
          success = setElementValue<long>(i,v);
          if (!success)
          {
            break;
          }
        }
        break;
      }
      case JsonStream::DOUBLE_TYPE:
      {
        for (size_t i=0; i<array_length_min; ++i)
        {
          double v = value[i];
          success = setElementValue<double>(i,v);
          if (!success)
          {
            break;
          }
        }
        break;
      }
      case JsonStream::BOOL_TYPE:
      {
        for (size_t i=0; i<array_length_min; ++i)
        {
          bool v = value[i];
          success = setElementValue<bool>(i,v);
          if (!success)
          {
            break;
          }
        }
        break;
      }
      case JsonStream::NULL_TYPE:
      {
        break;
      }
      case JsonStream::STRING_TYPE:
      {
        break;
      }
      case JsonStream::OBJECT_TYPE:
      {
        break;
      }
      case JsonStream::ARRAY_TYPE:
      {
        break;
      }
      case JsonStream::VALUE_TYPE:
      {
        break;
      }
    }
    postSetValueFunctor();
  }
  return success;
}

void Field::setValueToDefault()
{
  preSetValueFunctor();
  saved_variable_.setValueToDefault();
  postSetValueFunctor();
}

bool Field::valueIsDefault()
{
  return saved_variable_.valueIsDefault();
}

size_t Field::getArrayLength()
{
  if ((getType() == JsonStream::STRING_TYPE) &&
      (!stringIsSavedAsCharArray()))
  {
    const ConstantString * cs_ptr;
    getValue(cs_ptr);
    return cs_ptr->length() + 1;
  }
  return saved_variable_.getArrayLength();
}

size_t Field::getStringLength()
{
  size_t length = 0;
  JsonStream::JsonTypes field_type = getType();
  if (field_type == JsonStream::STRING_TYPE)
  {
    size_t array_length_max = getArrayLength();
    size_t array_length = 1;
    char value;
    while (array_length < array_length_max)
    {
      getElementValue(array_length-1,value);
      if (value == 0)
      {
        return array_length - 1;
      }
      ++array_length;
    }
    return array_length_max;
  }
  return length;
}

// private
template <>
Field::Field<long>(const ConstantString & name,
                   const long & default_value) :
parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeLong();
}

template <>
Field::Field<double>(const ConstantString & name,
                     const double & default_value) :
parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeDouble();
}

template <>
Field::Field<bool>(const ConstantString & name,
                   const bool & default_value) :
parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeBool();
}

template <>
Field::Field<const ConstantString *>(const ConstantString & name,
                                     const ConstantString * const & default_value) :
  parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeString();
  string_saved_as_char_array_ = false;
}

Parameter & Field::parameter()
{
  return parameter_;
}

bool Field::compareName(const char * name_to_compare)
{
  return parameter_.compareName(name_to_compare);
}

bool Field::compareName(const ConstantString & name_to_compare)
{
  return parameter_.compareName(name_to_compare);
}

const ConstantString & Field::getName()
{
  return parameter_.getName();
}

const ConstantString & Field::getFirmwareName()
{
  return parameter_.getFirmwareName();
}

bool Field::firmwareNameInArray(ArduinoJson::JsonArray & firmware_name_array)
{
  return parameter_.firmwareNameInArray(firmware_name_array);
}

JsonStream::JsonTypes Field::getType()
{
  return parameter_.getType();
}

JsonStream::JsonTypes Field::getArrayElementType()
{
  return parameter_.getArrayElementType();
}

bool Field::stringIsSavedAsCharArray()
{
  return string_saved_as_char_array_;
}

int Field::findSubsetValueIndex(const long value)
{
  return parameter_.findSubsetValueIndex(value);
}

int Field::findSubsetValueIndex(const char * value)
{
  return parameter_.findSubsetValueIndex(value);
}

Vector<const constants::SubsetMemberType> & Field::getSubset()
{
  return parameter_.getSubset();
}

void Field::preSetValueFunctor()
{
  if (pre_set_value_functor_)
  {
    pre_set_value_functor_();
  }
}

void Field::preSetElementValueFunctor(const size_t element_index)
{
  if (pre_set_element_value_functor_)
  {
    pre_set_element_value_functor_(element_index);
  }
}

void Field::postSetValueFunctor()
{
  if (post_set_value_functor_)
  {
    post_set_value_functor_();
  }
}

void Field::postSetElementValueFunctor(const size_t element_index)
{
  if (post_set_element_value_functor_)
  {
    post_set_element_value_functor_(element_index);
  }
}

}
