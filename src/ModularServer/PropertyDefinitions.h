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
    long default_value;
    getDefaultValue(default_value);
    if (!parameter_.valueInSubset(default_value))
    {
      setDefaultToSubsetElement(0);
    }
    long value;
    getValue(value);
    if (!parameter_.valueInSubset(value))
    {
      setValueToDefault();
    }
  }
  else if ((getType() == JsonStream::STRING_TYPE) &&
           !stringSavedAsCharArray())
  {
    const ConstantString * default_value;
    getDefaultValue(default_value);
    if (!parameter_.valueInSubset(default_value))
    {
      setDefaultToSubsetElement(0);
    }
    const ConstantString * value;
    getValue(value);
    if (!parameter_.valueInSubset(value))
    {
      setValueToDefault();
    }
  }
  else if ((getType() == JsonStream::ARRAY_TYPE) &&
           (getArrayElementType() == JsonStream::LONG_TYPE))
  {
    setArrayLengthRange(array_length_min_,array_length_max_);
    size_t array_length = getArrayLength();
    for (size_t i=0; i<array_length; ++i)
    {
      long value;
      getElementValue(i,value);
      if (!parameter_.valueInSubset(value))
      {
        setElementValueToDefault(i);
      }
    }
  }
  else if ((getType() == JsonStream::ARRAY_TYPE) &&
           (getArrayElementType() == JsonStream::STRING_TYPE) &&
           !stringSavedAsCharArray())
  {
    setArrayLengthRange(array_length_min_,array_length_max_);
    size_t array_length = getArrayLength();
    for (size_t i=0; i<array_length; ++i)
    {
      const ConstantString * value;
      getElementValue(i,value);
      if (!parameter_.valueInSubset(value))
      {
        setElementValueToDefault(i);
      }
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

template <size_t N>
bool Property::setDefaultValue(const long (&default_value)[N])
{
  bool success = false;
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::LONG_TYPE))
  {
    success = saved_variable_.setDefaultValue(default_value);
    if (success)
    {
      parameter_.setArrayLengthRange(N,N);
      setArrayLengthRange(N,N);
    }
  }
  return success;
}

template <size_t N>
bool Property::setDefaultValue(const double (&default_value)[N])
{
  bool success = false;
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::DOUBLE_TYPE))
  {
    success = saved_variable_.setDefaultValue(default_value);
    if (success)
    {
      parameter_.setArrayLengthRange(N,N);
      setArrayLengthRange(N,N);
    }
  }
  return success;
}

template <size_t N>
bool Property::setDefaultValue(const bool (&default_value)[N])
{
  bool success = false;
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::BOOL_TYPE))
  {
    success = saved_variable_.setDefaultValue(default_value);
    if (success)
    {
      parameter_.setArrayLengthRange(N,N);
      setArrayLengthRange(N,N);
    }
  }
  return success;
}

template <size_t N>
bool Property::setDefaultValue(const char (&default_value)[N])
{
  bool success = false;
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::STRING_TYPE) &&
      string_saved_as_char_array_)
  {
    success = saved_variable_.setDefaultValue(default_value);
    if (success)
    {
      parameter_.setArrayLengthRange(N,N);
      setArrayLengthRange(N,N);
    }
  }
  size_t array_length = getArrayLength();
  char string[array_length];
  getValue(string,array_length);
  size_t string_length = strlen(string);
  if (string_length >= array_length)
  {
    setValueToDefault();
  }
  else
  {
    for (size_t i=0; i<string_length; ++i)
    {
      char c = string[i];
      // check if all characters are printable ascii
      if ((c < 32) || (c > 126))
      {
        setValueToDefault();
        break;
      }
    }
  }
  return success;
}

template <size_t N>
bool Property::setDefaultValue(const ConstantString * const (&default_value)[N])
{
  bool success = false;
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::STRING_TYPE) &&
      !string_saved_as_char_array_)
  {
    success = saved_variable_.setDefaultValue(default_value);
    if (success)
    {
      parameter_.setArrayLengthRange(N,N);
      setArrayLengthRange(N,N);
    }
  }
  return success;
}

// private
template <size_t N>
Property::Property(const ConstantString & name,
                   const long (&default_value)[N]) :
  parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeLong();
  parameter_.setArrayLengthRange(N,N);
  setArrayLengthRange(N,N);
  setup();
}

template <size_t N>
Property::Property(const ConstantString & name,
                   const double (&default_value)[N]) :
  parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeDouble();
  parameter_.setArrayLengthRange(N,N);
  setArrayLengthRange(N,N);
  setup();
}

template <size_t N>
Property::Property(const ConstantString & name,
                   const bool (&default_value)[N]) :
  parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeBool();
  parameter_.setArrayLengthRange(N,N);
  setArrayLengthRange(N,N);
  setup();
}

template <size_t N>
Property::Property(const ConstantString & name,
                   const char (&default_value)[N]) :
  parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeString();
  string_saved_as_char_array_ = true;
  size_t array_length = getArrayLength();
  char string[array_length];
  getValue(string,array_length);
  size_t string_length = strlen(string);
  if (string_length >= array_length)
  {
    setValueToDefault();
  }
  else
  {
    for (size_t i=0; i<string_length; ++i)
    {
      char c = string[i];
      // check if all characters are printable ascii
      if ((c < 32) || (c > 126))
      {
        setValueToDefault();
        break;
      }
    }
  }
  setup();
}

template <size_t N>
Property::Property(const ConstantString & name,
                   const ConstantString * const (&default_value)[N]) :
  parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeString();
  parameter_.setArrayLengthRange(N,N);
  string_saved_as_char_array_ = false;
  setup();
}

}
#endif
