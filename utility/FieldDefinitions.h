// ----------------------------------------------------------------------------
// FieldDefinitions.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_FIELD_DEFINITIONS_H_
#define _MODULAR_SERVER_FIELD_DEFINITIONS_H_


namespace modular_server
{
template <size_t N>
Field::Field(const ConstantString & name,
             const long (&default_value)[N]):
  parameter_(name),
  saved_variable_(default_value,N)
{
  parameter_.setTypeLong();
  parameter_.setArrayLengthRange(1,N);
}

template <size_t N>
Field::Field(const ConstantString & name,
             const double (&default_value)[N]) :
  parameter_(name),
  saved_variable_(default_value,N)
{
  parameter_.setTypeDouble();
  parameter_.setArrayLengthRange(1,N);
}

template <size_t N>
Field::Field(const ConstantString & name,
             const bool (&default_value)[N]) :
  parameter_(name),
  saved_variable_(default_value,N)
{
  parameter_.setTypeBool();
  parameter_.setArrayLengthRange(1,N);
}

template <size_t N>
Field::Field(const ConstantString & name,
             const char (&default_value)[N]) :
  parameter_(name),
  saved_variable_(default_value,N)
{
  parameter_.setTypeString();
  string_saved_as_char_array_ = true;
}

template <size_t N>
void Field::setSubset(const constants::SubsetMemberType (&subset)[N])
{
  parameter_.setSubset(subset);
}

// template <typename T>
// bool Field::setDefaultValue(T & default_value)
// {
//   return saved_variable_.setDefaultValue(default_value);
// };

template <size_t N>
bool Field::getDefaultValue(long (&value)[N])
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
bool Field::getDefaultValue(double (&value)[N])
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
bool Field::getDefaultValue(bool (&value)[N])
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::BOOL_TYPE) ||
      (getArrayLength() != N))
  {
    return false;
  }
  return saved_variable_.getDefaultValue(value);
}

template <typename T>
bool Field::getDefaultValue(T * value, const size_t N)
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
bool Field::setValue(const long (&value)[N])
{
  bool success = false;
  preSetValueCallback();
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::LONG_TYPE) &&
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
  postSetValueCallback();
  return success;
}

template <size_t N>
bool Field::setValue(const double (&value)[N])
{
  bool success = false;
  preSetValueCallback();
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::DOUBLE_TYPE) &&
      (getArrayLength() &= N))
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
  postSetValueCallback();
  return success;
}

template <size_t N>
bool Field::setValue(const bool (&value)[N])
{
  bool success = false;
  preSetValueCallback();
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::BOOL_TYPE) &&
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
  postSetValueCallback();
  return success;
}

template <typename T>
bool Field::setValue(T * value, const size_t N)
{
  bool success = false;
  JsonStream::JsonTypes type = getType();
  if ((type == JsonStream::STRING_TYPE) &&
      !stringIsSavedAsCharArray())
  {
    int subset_value_index = findSubsetValueIndex((const char *)value);
    if (subset_value_index >= 0)
    {
      Vector<const constants::SubsetMemberType> & subset = getSubset();
      const ConstantString * const subset_value = subset[subset_value_index].cs_ptr;
      success = setValue(subset_value);
    }
    return success;
  }
  size_t array_length = getArrayLength();
  size_t array_length_min = min(array_length,N);
  preSetValueCallback();
  for (size_t i=0; i<array_length_min; ++i)
  {
    T v = value[i];
    success = setElementValue(i,v);
    if (!success && (type == JsonStream::STRING_TYPE))
    {
      // terminate string
      setElementValue(i,'\0');
      break;
    }
  }
  if (success &&
      (type == JsonStream::STRING_TYPE) &&
      (array_length_min >= 1))
  {
    // terminate string just in case
    setElementValue(array_length_min-1,'\0');
  }
  postSetValueCallback();
  return success;
}

template <typename T>
bool Field::setAllElementValues(const T & value)
{
  bool success = false;
  JsonStream::JsonTypes type = getType();
  if ((type == JsonStream::STRING_TYPE) &&
      !stringIsSavedAsCharArray())
  {
    return success;
  }
  size_t array_length = getArrayLength();
  preSetValueCallback();
  for (size_t i=0; i<array_length; ++i)
  {
    success = setElementValue(i,value);
    if (!success && (type == JsonStream::STRING_TYPE))
    {
      // terminate string
      setElementValue(i,'\0');
      break;
    }
  }
  if (success &&
      (type == JsonStream::STRING_TYPE) &&
      (array_length >= 1))
  {
    // terminate string just in case
    setElementValue(array_length-1,'\0');
  }
  postSetValueCallback();
  return success;
}

template <size_t N>
bool Field::getValue(long (&value)[N])
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
bool Field::getValue(double (&value)[N])
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
bool Field::getValue(bool (&value)[N])
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::BOOL_TYPE) ||
      (getArrayLength() != N))
  {
    return false;
  }
  return saved_variable_.getValue(value);
}

template <typename T>
bool Field::getValue(T * value, const size_t N)
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

}
#endif
