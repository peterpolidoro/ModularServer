// ----------------------------------------------------------------------------
// Field.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Field.h"


namespace ModularDevice
{
Field::Field()
{
  set_value_callback_ = NULL;
  set_element_value_callback_ = NULL;
}

template <>
Field::Field<long>(const ConstantString & name,
                   const long & default_value) :
parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeLong();
  set_value_callback_ = NULL;
  set_element_value_callback_ = NULL;
}

template <>
Field::Field<double>(const ConstantString & name,
                     const double & default_value) :
parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeDouble();
  set_value_callback_ = NULL;
  set_element_value_callback_ = NULL;
}

template <>
Field::Field<bool>(const ConstantString & name,
                   const bool & default_value) :
parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeBool();
  set_value_callback_ = NULL;
  set_element_value_callback_ = NULL;
}

template <>
Field::Field(const ConstantString & name,
             const ConstantString * const & default_value) :
  parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeString();
  string_saved_as_char_array_ = false;
  set_value_callback_ = NULL;
  set_element_value_callback_ = NULL;
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

void Field::attachSetValueCallback(SetValueCallback callback)
{
  set_value_callback_ = callback;
}

void Field::attachSetElementValueCallback(SetElementValueCallback callback)
{
  set_element_value_callback_ = callback;
}

// Private
// Saved Variable Methods
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

template <>
bool Field::getDefaultValue<const ConstantString *>(const ConstantString * & value)
{
  if ((getType() != JsonStream::STRING_TYPE) ||
      isStringSavedAsCharArray())
  {
    value = NULL;
    return false;
  }
  return saved_variable_.getDefaultValue(value);
}

template <>
bool Field::getDefaultElementValue<long>(long & value, const size_t element_index)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::LONG_TYPE))
  {
    return false;
  }
  return saved_variable_.getDefaultElementValue(value,element_index);
}

template <>
bool Field::getDefaultElementValue<double>(double & value, const size_t element_index)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::DOUBLE_TYPE))
  {
    return false;
  }
  return saved_variable_.getDefaultElementValue(value,element_index);
}

template <>
bool Field::getDefaultElementValue<bool>(bool & value, const size_t element_index)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::BOOL_TYPE))
  {
    return false;
  }
  return saved_variable_.getDefaultElementValue(value,element_index);
}

template <>
bool Field::getDefaultElementValue<char>(char & value, const size_t element_index)
{
  if ((getType() != JsonStream::STRING_TYPE) ||
      (!isStringSavedAsCharArray()))
  {
    return false;
  }
  return saved_variable_.getDefaultElementValue(value,element_index);
}

template <>
bool Field::setValue<long>(const long & value)
{
  if (getType() != JsonStream::LONG_TYPE)
  {
    return false;
  }
  if (parameter_.rangeIsSet())
  {
    long min = parameter_.getMin().l;
    long max = parameter_.getMax().l;
    if ((value < min) || (value > max))
    {
      return false;
    }
  }
  return saved_variable_.setValue(value);
}

template <>
bool Field::setValue<double>(const double & value)
{
  if (getType() != JsonStream::DOUBLE_TYPE)
  {
    return false;
  }
  if (parameter_.rangeIsSet())
  {
    double min = parameter_.getMin().d;
    double max = parameter_.getMax().d;
    if ((value < min) || (value > max))
    {
      return false;
    }
  }
  return saved_variable_.setValue(value);
}

template <>
bool Field::setValue<bool>(const bool & value)
{
  if (getType() != JsonStream::BOOL_TYPE)
  {
    return false;
  }
  return saved_variable_.setValue(value);
}

template <>
bool Field::setValue<const ConstantString * const>(const ConstantString * const & value)
{
  if ((getType() != JsonStream::STRING_TYPE) ||
      isStringSavedAsCharArray())
  {
    return false;
  }
  return saved_variable_.setValue(value);
}

template <>
bool Field::setElementValue<long>(const long & value, const size_t element_index)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::LONG_TYPE))
  {
    return false;
  }
  if (parameter_.rangeIsSet())
  {
    long min = parameter_.getMin().l;
    long max = parameter_.getMax().l;
    if ((value < min) || (value > max))
    {
      return false;
    }
  }
  return saved_variable_.setElementValue(value,element_index);
}

template <>
bool Field::setElementValue<double>(const double & value, const size_t element_index)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::DOUBLE_TYPE))
  {
    return false;
  }
  if (parameter_.rangeIsSet())
  {
    double min = parameter_.getMin().d;
    double max = parameter_.getMax().d;
    if ((value < min) || (value > max))
    {
      return false;
    }
  }
  return saved_variable_.setElementValue(value,element_index);
}

template <>
bool Field::setElementValue<bool>(const bool & value, const size_t element_index)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::BOOL_TYPE))
  {
    return false;
  }
  return saved_variable_.setElementValue(value,element_index);
}

template <>
bool Field::setElementValue<char>(const char & value, const size_t element_index)
{
  if ((getType() != JsonStream::STRING_TYPE) ||
      (!isStringSavedAsCharArray()))
  {
    return false;
  }
  return saved_variable_.setElementValue(value,element_index);
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

template <>
bool Field::getValue<const ConstantString *>(const ConstantString * & value)
{
  if ((getType() != JsonStream::STRING_TYPE) ||
      isStringSavedAsCharArray())
  {
    value = NULL;
    return false;
  }
  return saved_variable_.getValue(value);
}

template <>
bool Field::getElementValue<long>(long & value, const size_t element_index)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::LONG_TYPE))
  {
    return false;
  }
  return saved_variable_.getElementValue(value,element_index);
}

template <>
bool Field::getElementValue<double>(double & value, const size_t element_index)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::DOUBLE_TYPE))
  {
    return false;
  }
  return saved_variable_.getElementValue(value,element_index);
}

template <>
bool Field::getElementValue<bool>(bool & value, const size_t element_index)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::BOOL_TYPE))
  {
    return false;
  }
  return saved_variable_.getElementValue(value,element_index);
}

template <>
bool Field::getElementValue<char>(char & value, const size_t element_index)
{
  if ((getType() != JsonStream::STRING_TYPE) ||
      (!isStringSavedAsCharArray()))
  {
    return false;
  }
  return saved_variable_.getElementValue(value,element_index);
}

void Field::setDefaultValue()
{
  saved_variable_.setDefaultValue();
  setValueCallback();
}

bool Field::isDefaultValue()
{
  return saved_variable_.isDefaultValue();
}

size_t Field::getArrayLength()
{
  return saved_variable_.getArrayLength();
}

Parameter & Field::getParameter()
{
  return parameter_;
}

const ConstantString & Field::getName()
{
  return parameter_.getName();
}

JsonStream::JsonTypes Field::getType()
{
  return parameter_.getType();
}

JsonStream::JsonTypes Field::getArrayElementType()
{
  return parameter_.getArrayElementType();
}

bool Field::isStringSavedAsCharArray()
{
  return string_saved_as_char_array_;
}

void Field::setValueCallback()
{
  if (set_value_callback_ != NULL)
  {
    (*set_value_callback_)();
  }
}

void Field::setElementValueCallback(const size_t element_index)
{
  if (set_element_value_callback_ != NULL)
  {
    (*set_element_value_callback_)(element_index);
  }
}

}
