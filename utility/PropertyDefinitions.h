// ----------------------------------------------------------------------------
// PropertyDefinitions.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_PROPERTY_DEFINITIONS_H_
#define _MODULAR_SERVER_PROPERTY_DEFINITIONS_H_


namespace modular_server
{
// public
template <size_t MAX_SIZE>
void Property::setSubset(constants::SubsetMemberType (&subset)[MAX_SIZE], size_t size)
{
  parameter_.setSubset(subset);
  if (getType() == JsonStream::LONG_TYPE)
  {
    long value;
    getValue(value);
    if (!valueInSubset(value))
    {
      setValueToDefault();
    }
  }
  else if ((getType() == JsonStream::STRING_TYPE) &&
           !stringSavedAsCharArray())
  {
    const ConstantString * value;
    getValue(value);
    if (!valueInSubset(value))
    {
      setValueToDefault();
    }
  }
}

template <size_t N>
bool Property::getValue(long (&value)[N])
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::LONG_TYPE) ||
      (getArrayLength() != N))
  {
    return false;
  }
  return saved_variable_.getValue(value);
}

template <size_t N>
bool Property::getValue(double (&value)[N])
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::DOUBLE_TYPE) ||
      (getArrayLength() != N))
  {
    return false;
  }
  return saved_variable_.getValue(value);
}

template <size_t N>
bool Property::getValue(bool (&value)[N])
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::BOOL_TYPE) ||
      (getArrayLength() != N))
  {
    return false;
  }
  return saved_variable_.getValue(value);
}

template <size_t N>
bool Property::getValue(const ConstantString * (&value)[N])
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::STRING_TYPE) ||
      (stringSavedAsCharArray()) ||
      (getArrayLength() != N))
  {
    return false;
  }
  return saved_variable_.getValue(value);
}

template <typename T>
bool Property::getValue(T * value, const size_t N)
{
  JsonStream::JsonTypes type = getType();
  size_t array_length = getArrayLength();
  size_t array_length_min = min(array_length,N);
  bool success;
  for (size_t i=0; i<array_length_min; ++i)
  {
    T v;
    success = getElementValue(i,v);
    if (!success)
    {
      // terminate string
      if (type == JsonStream::STRING_TYPE)
      {
        value[i] = '\0';
      }
      return false;
    }
    value[i] = v;
  }
  // terminate string
  if ((type == JsonStream::STRING_TYPE) &&
      (array_length_min >= 1))
  {
    value[array_length_min-1] = '\0';
  }
  return true;
}

template <size_t N>
bool Property::getDefaultValue(long (&value)[N])
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::LONG_TYPE) ||
      (getArrayLength() != N))
  {
    return false;
  }
  return saved_variable_.getDefaultValue(value);
}

template <size_t N>
bool Property::getDefaultValue(double (&value)[N])
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::DOUBLE_TYPE) ||
      (getArrayLength() != N))
  {
    return false;
  }
  return saved_variable_.getDefaultValue(value);
}

template <size_t N>
bool Property::getDefaultValue(bool (&value)[N])
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::BOOL_TYPE) ||
      (getArrayLength() != N))
  {
    return false;
  }
  return saved_variable_.getDefaultValue(value);
}

template <size_t N>
bool Property::getDefaultValue(const ConstantString * (&value)[N])
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::STRING_TYPE) ||
      (stringSavedAsCharArray()) ||
      (getArrayLength() != N))
  {
    return false;
  }
  return saved_variable_.getDefaultValue(value);
}

template <typename T>
bool Property::getDefaultValue(T * value, const size_t N)
{
  size_t array_length = getArrayLength();
  size_t array_length_min = min(array_length,N);
  bool success;
  for (size_t i=0; i<array_length_min; ++i)
  {
    T v;
    success = getDefaultElementValue(i,v);
    if (!success)
    {
      return false;
    }
    value[i] = v;
  }
  return true;
}

template <size_t N>
bool Property::setValue(const long (&value)[N])
{
  bool success = false;
  preSetValueFunctor();
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::LONG_TYPE) &&
      (getArrayLength() == N))
  {
    for (size_t i=0;i<N;++i)
    {
      success = setElementValue<long>(i,value[i]);
      if (!success)
      {
        break;
      }
    }
  }
  postSetValueFunctor();
  return success;
}

template <size_t N>
bool Property::setValue(const double (&value)[N])
{
  bool success = false;
  preSetValueFunctor();
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::DOUBLE_TYPE) &&
      (getArrayLength() &= N))
  {
    for (size_t i=0;i<N;++i)
    {
      success = setElementValue<double>(i,value[i]);
      if (!success)
      {
        break;
      }
    }
  }
  postSetValueFunctor();
  return success;
}

template <size_t N>
bool Property::setValue(const bool (&value)[N])
{
  bool success = false;
  preSetValueFunctor();
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::BOOL_TYPE) &&
      (getArrayLength() == N))
  {
    for (size_t i=0;i<N;++i)
    {
      success = setElementValue<bool>(i,value[i]);
      if (!success)
      {
        break;
      }
    }
  }
  postSetValueFunctor();
  return success;
}

template <size_t N>
bool Property::setValue(const ConstantString * (&value)[N])
{
  bool success = false;
  preSetValueFunctor();
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::STRING_TYPE) &&
      (!stringSavedAsCharArray()) &&
      (getArrayLength() == N))
  {
    for (size_t i=0;i<N;++i)
    {
      success = setElementValue(i,value[i]);
      if (!success)
      {
        break;
      }
    }
  }
  postSetValueFunctor();
  return success;
}

template <typename T>
bool Property::setValue(T * value, const size_t N)
{
  bool success = false;
  JsonStream::JsonTypes type = getType();
  if ((type == JsonStream::STRING_TYPE) &&
      !stringSavedAsCharArray())
  {
    int subset_value_index = findSubsetValueIndex((const char *)value);
    if (subset_value_index >= 0)
    {
      Vector<constants::SubsetMemberType> & subset = getSubset();
      const ConstantString * const subset_value = subset[subset_value_index].cs_ptr;
      success = setValue(subset_value);
    }
    return success;
  }
  size_t array_length = getArrayLength();
  size_t array_length_min = min(array_length,N);
  preSetValueFunctor();
  for (size_t i=0; i<array_length_min; ++i)
  {
    T v = value[i];
    success = setElementValue(i,v);
    if (!success && (type == JsonStream::STRING_TYPE))
    {
      // terminate string
      setElementValue<char>(i,'\0');
      break;
    }
  }
  if (success &&
      (type == JsonStream::STRING_TYPE) &&
      (array_length_min >= 1))
  {
    // terminate string just in case
    setElementValue<char>(array_length_min-1,'\0');
  }
  postSetValueFunctor();
  return success;
}

template <typename T>
bool Property::setAllElementValues(const T & value)
{
  bool success = false;
  JsonStream::JsonTypes type = getType();
  if ((type == JsonStream::STRING_TYPE) &&
      !stringSavedAsCharArray())
  {
    return success;
  }
  size_t array_length = getArrayLength();
  preSetValueFunctor();
  for (size_t i=0; i<array_length; ++i)
  {
    success = setElementValue(i,value);
    if (!success && (type == JsonStream::STRING_TYPE))
    {
      // terminate string
      setElementValue<char>(i,'\0');
      break;
    }
  }
  if (success &&
      (type == JsonStream::STRING_TYPE) &&
      (array_length >= 1))
  {
    // terminate string just in case
    setElementValue<char>(array_length-1,'\0');
  }
  postSetValueFunctor();
  return success;
}

template <typename T>
bool Property::setDefaultValue(T & default_value)
{
  return saved_variable_.setDefaultValue(default_value);
};

// private
template <size_t N>
Property::Property(const ConstantString & name,
                   const long (&default_value)[N]):
  parameter_(name),
  saved_variable_(default_value,N)
{
  parameter_.setTypeLong();
  parameter_.setArrayLengthRange(N,N);
}

template <size_t N>
Property::Property(const ConstantString & name,
                   const double (&default_value)[N]) :
  parameter_(name),
  saved_variable_(default_value,N)
{
  parameter_.setTypeDouble();
  parameter_.setArrayLengthRange(N,N);
}

template <size_t N>
Property::Property(const ConstantString & name,
                   const bool (&default_value)[N]) :
  parameter_(name),
  saved_variable_(default_value,N)
{
  parameter_.setTypeBool();
  parameter_.setArrayLengthRange(N,N);
}

template <size_t N>
Property::Property(const ConstantString & name,
                   const char (&default_value)[N]) :
  parameter_(name),
  saved_variable_(default_value,N)
{
  parameter_.setTypeString();
  string_saved_as_char_array_ = true;
}

template <size_t N>
Property::Property(const ConstantString & name,
                   const ConstantString * const (&default_value)[N]) :
  parameter_(name),
  saved_variable_(default_value,N)
{
  parameter_.setTypeString();
  parameter_.setArrayLengthRange(N,N);
  string_saved_as_char_array_ = false;
}

}
#endif
