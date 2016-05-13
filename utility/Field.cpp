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
}

template <>
Field::Field<long>(const ConstantString &name,
                   const long &default_value) :
  parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeLong();
}

template <>
Field::Field<bool>(const ConstantString &name,
                   const bool &default_value) :
  parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeBool();
}

// Parameter Methods
void Field::setRange(const long min, const long max)
{
  parameter_.setRange(min,max);
}

// Saved Variable Methods
template <>
void Field::getDefaultValue<long>(long &value)
{
  saved_variable_.getDefaultValue(value);
}

template <>
void Field::getDefaultValue<bool>(bool &value)
{
  saved_variable_.getDefaultValue(value);
}

template <>
void Field::getDefaultElementValue<long>(long &value, const unsigned int element_index)
{
  saved_variable_.getDefaultElementValue(value,element_index);
}

template <>
void Field::getDefaultElementValue<bool>(bool &value, const unsigned int element_index)
{
  saved_variable_.getDefaultElementValue(value,element_index);
}

template <>
void Field::setValue<long>(const long &value)
{
  saved_variable_.setValue(value);
}

template <>
void Field::setValue<bool>(const bool &value)
{
  saved_variable_.setValue(value);
}

template <>
void Field::setElementValue<long>(const long &value, const unsigned int element_index)
{
  saved_variable_.setElementValue(value,element_index);
}

template <>
void Field::setElementValue<bool>(const bool &value, const unsigned int element_index)
{
  saved_variable_.setElementValue(value,element_index);
}

template <>
void Field::getValue<long>(long &value)
{
  saved_variable_.getValue(value);
}

template <>
void Field::getValue<bool>(bool &value)
{
  saved_variable_.getValue(value);
}

template <>
void Field::getElementValue<long>(long &value, const unsigned int element_index)
{
  saved_variable_.getElementValue(value,element_index);
}

template <>
void Field::getElementValue<bool>(bool &value, const unsigned int element_index)
{
  saved_variable_.getElementValue(value,element_index);
}

void Field::setDefaultValue()
{
  saved_variable_.setDefaultValue();
}

bool Field::isDefaultValue()
{
  return saved_variable_.isDefaultValue();
}

unsigned int Field::getArrayLength()
{
  return saved_variable_.getArrayLength();
}

// Private
Parameter& Field::getParameter()
{
  return parameter_;
}
}
