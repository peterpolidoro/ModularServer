// ----------------------------------------------------------------------------
// Property.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Property.h"


namespace modular_server
{

namespace property
{
// Parameters
CONSTANT_STRING(function_parameter_name,"function");
CONSTANT_STRING(value_parameter_name,"value");

// Functions
CONSTANT_STRING(get_value_function_name,"getValue");
CONSTANT_STRING(set_value_function_name,"setValue");
CONSTANT_STRING(get_default_value_function_name,"getDefaultValue");
CONSTANT_STRING(set_value_to_default_function_name,"setValueToDefault");

// Array Parameters
CONSTANT_STRING(element_index_parameter_name,"element_index");
CONSTANT_STRING(element_value_parameter_name,"element_value");
CONSTANT_STRING(array_length_parameter_name,"array_length");

// Array Functions
CONSTANT_STRING(get_element_value_function_name,"getElementValue");
CONSTANT_STRING(set_element_value_function_name,"setElementValue");
CONSTANT_STRING(get_default_element_value_function_name,"getDefaultElementValue");
CONSTANT_STRING(set_element_value_to_default_function_name,"setElementValueToDefault");
CONSTANT_STRING(set_all_element_values_function_name,"setAllElementValues");
CONSTANT_STRING(get_array_length_function_name,"getArrayLength");
CONSTANT_STRING(set_array_length_function_name,"setArrayLength");
}

Parameter Property::property_parameters_[property::PARAMETER_COUNT_MAX];
Function Property::property_functions_[property::FUNCTION_COUNT_MAX];
Parameter Property::property_array_parameters_[property::ARRAY_PARAMETER_COUNT_MAX];
Function Property::property_array_functions_[property::ARRAY_FUNCTION_COUNT_MAX];
ConcatenatedArray<Parameter,property::FUNCTION_PARAMETER_TYPE_COUNT> Property::parameters_;
ConcatenatedArray<Function,property::FUNCTION_PARAMETER_TYPE_COUNT> Property::functions_;
Response * Property::response_ptr_;
Functor1wRet<const ConstantString &, ArduinoJson::JsonVariant> Property::get_parameter_value_functor_;

Parameter & Property::createParameter(const ConstantString & parameter_name)
{
  int parameter_index = findParameterIndex(parameter_name);
  if (parameter_index < 0)
  {
    parameters_.push_back(Parameter(parameter_name));
    parameters_.back().setFirmwareName(constants::firmware_name);
    return parameters_.back();
  }
  return parameters_[0]; // bad reference
}

Parameter & Property::parameter(const ConstantString & parameter_name)
{
  int parameter_index = findParameterIndex(parameter_name);
  if ((parameter_index >= 0) && (parameter_index < (int)parameters_.size()))
  {
    return parameters_[parameter_index];
  }
  return parameters_[0]; // bad reference
}

Parameter & Property::copyParameter(Parameter parameter, const ConstantString & parameter_name)
{
  parameters_.push_back(parameter);
  parameters_.back().setName(parameter_name);
  return parameters_.back();
}

Function & Property::createFunction(const ConstantString & function_name)
{
  int function_index = findFunctionIndex(function_name);
  if (function_index < 0)
  {
    functions_.push_back(Function(function_name));
    functions_.back().setFirmwareName(constants::firmware_name);
    return functions_.back();
  }
  return functions_[0]; // bad reference
}

Function & Property::function(const ConstantString & function_name)
{
  int function_index = findFunctionIndex(function_name);
  if ((function_index >= 0) && (function_index < (int)functions_.size()))
  {
    return functions_[function_index];
  }
  return functions_[0]; // bad reference
}

// public
Property::Property()
{
  response_ptr_ = NULL;
  functors_enabled_ = true;
}

template <>
bool Property::getValue<long>(long & value)
{
  if (getType() != JsonStream::LONG_TYPE)
  {
    return false;
  }
  return saved_variable_.getValue(value);
}

template <>
bool Property::getValue<double>(double & value)
{
  if (getType() != JsonStream::DOUBLE_TYPE)
  {
    return false;
  }
  return saved_variable_.getValue(value);
}

template <>
bool Property::getValue<bool>(bool & value)
{
  if (getType() != JsonStream::BOOL_TYPE)
  {
    return false;
  }
  return saved_variable_.getValue(value);
}

template <>
bool Property::getValue<const ConstantString *>(const ConstantString * & value)
{
  if ((getType() != JsonStream::STRING_TYPE) ||
      (stringSavedAsCharArray()))
  {
    value = NULL;
    return false;
  }
  return saved_variable_.getValue(value);
}

template <>
bool Property::getElementValue<long>(const size_t element_index, long & value)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::LONG_TYPE))
  {
    return false;
  }
  return saved_variable_.getElementValue(element_index,value);
}

template <>
bool Property::getElementValue<double>(const size_t element_index, double & value)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::DOUBLE_TYPE))
  {
    return false;
  }
  return saved_variable_.getElementValue(element_index,value);
}

template <>
bool Property::getElementValue<bool>(const size_t element_index, bool & value)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::BOOL_TYPE))
  {
    return false;
  }
  return saved_variable_.getElementValue(element_index,value);
}

template <>
bool Property::getElementValue<char>(const size_t element_index, char & value)
{
  if (getType() != JsonStream::STRING_TYPE)
  {
    return false;
  }
  if (!stringSavedAsCharArray())
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
bool Property::getElementValue<const ConstantString *>(const size_t element_index, const ConstantString * & value)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::STRING_TYPE) ||
      (stringSavedAsCharArray()))
  {
    return false;
  }
  return saved_variable_.getElementValue(element_index,value);
}

template <>
bool Property::getDefaultValue<long>(long & value)
{
  if (getType() != JsonStream::LONG_TYPE)
  {
    return false;
  }
  return saved_variable_.getDefaultValue(value);
}

template <>
bool Property::getDefaultValue<double>(double & value)
{
  if (getType() != JsonStream::DOUBLE_TYPE)
  {
    return false;
  }
  return saved_variable_.getDefaultValue(value);
}

template <>
bool Property::getDefaultValue<bool>(bool & value)
{
  if (getType() != JsonStream::BOOL_TYPE)
  {
    return false;
  }
  return saved_variable_.getDefaultValue(value);
}

template <>
bool Property::getDefaultValue<const ConstantString *>(const ConstantString * & value)
{
  if ((getType() != JsonStream::STRING_TYPE) ||
      stringSavedAsCharArray())
  {
    value = NULL;
    return false;
  }
  return saved_variable_.getDefaultValue(value);
}

template <>
bool Property::getDefaultElementValue<long>(const size_t element_index, long & value)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::LONG_TYPE))
  {
    return false;
  }
  return saved_variable_.getDefaultElementValue(element_index,value);
}

template <>
bool Property::getDefaultElementValue<double>(const size_t element_index, double & value)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::DOUBLE_TYPE))
  {
    return false;
  }
  return saved_variable_.getDefaultElementValue(element_index,value);
}

template <>
bool Property::getDefaultElementValue<bool>(const size_t element_index, bool & value)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::BOOL_TYPE))
  {
    return false;
  }
  return saved_variable_.getDefaultElementValue(element_index,value);
}

template <>
bool Property::getDefaultElementValue<char>(const size_t element_index, char & value)
{
  if (getType() != JsonStream::STRING_TYPE)
  {
    return false;
  }
  if (!stringSavedAsCharArray())
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
bool Property::getDefaultElementValue<const ConstantString *>(const size_t element_index, const ConstantString * & value)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::STRING_TYPE) ||
      (stringSavedAsCharArray()))
  {
    return false;
  }
  return saved_variable_.getDefaultElementValue(element_index,value);
}

template <>
bool Property::setElementValue<long>(const size_t element_index, const long & value)
{
  bool success = false;
  preSetElementValueFunctor(element_index);
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::LONG_TYPE))
  {
    if (parameter_.valueInRange(value) && parameter_.valueInSubset(value))
    {
      success = saved_variable_.setElementValue(element_index,value);
    }
  }
  postSetElementValueFunctor(element_index);
  return success;
}

template <>
bool Property::setElementValue<double>(const size_t element_index, const double & value)
{
  bool success = false;
  preSetElementValueFunctor(element_index);
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::DOUBLE_TYPE))
  {
    if (parameter_.valueInRange(value))
    {
      success = saved_variable_.setElementValue(element_index,value);
    }
  }
  postSetElementValueFunctor(element_index);
  return success;
}

template <>
bool Property::setElementValue<bool>(const size_t element_index, const bool & value)
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
bool Property::setElementValue<char>(const size_t element_index, const char & value)
{
  bool success = false;
  preSetElementValueFunctor(element_index);
  if ((getType() == JsonStream::STRING_TYPE) &&
      (stringSavedAsCharArray()))
  {
    success = saved_variable_.setElementValue(element_index,value);
  }
  postSetElementValueFunctor(element_index);
  return success;
}

template <>
bool Property::setElementValue<const ConstantString * const>(const size_t element_index, const ConstantString * const & value)
{
  bool success = false;
  preSetElementValueFunctor(element_index);
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::STRING_TYPE) &&
      (!stringSavedAsCharArray()))
  {
    if (parameter_.valueInSubset(value))
    {
      success = saved_variable_.setElementValue(element_index,value);
    }
  }
  postSetElementValueFunctor(element_index);
  return success;
}

template <>
bool Property::setElementValue<const char *>(const size_t element_index, const char * const & value)
{
  bool success = false;
  preSetElementValueFunctor(element_index);
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::STRING_TYPE) &&
      (!stringSavedAsCharArray()))
  {
    int subset_value_index = findSubsetValueIndex(value);
    if (subset_value_index >= 0)
    {
      Vector<constants::SubsetMemberType> & subset = getSubset();
      const ConstantString * const subset_value = subset[subset_value_index].cs_ptr;
      success = saved_variable_.setElementValue(element_index,subset_value);
    }
  }
  postSetElementValueFunctor(element_index);
  return success;
}

template <>
bool Property::setValue<long>(const long & value)
{
  bool success = false;
  preSetValueFunctor();
  if (getType() == JsonStream::LONG_TYPE)
  {
    if (parameter_.valueInRange(value) && parameter_.valueInSubset(value))
    {
      success = saved_variable_.setValue(value);
    }
  }
  postSetValueFunctor();
  return success;
}

template <>
bool Property::setValue<double>(const double & value)
{
  bool success = false;
  preSetValueFunctor();
  if (getType() == JsonStream::DOUBLE_TYPE)
  {
    if (parameter_.valueInRange(value))
    {
      success = saved_variable_.setValue(value);
    }
  }
  postSetValueFunctor();
  return success;
}

template <>
bool Property::setValue<bool>(const bool & value)
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
bool Property::setValue<const ConstantString *>(const ConstantString * const & value)
{
  bool success = false;
  preSetValueFunctor();
  if ((getType() == JsonStream::STRING_TYPE) &&
      !stringSavedAsCharArray())
  {
    if (parameter_.valueInSubset(value))
    {
      success = saved_variable_.setValue(value);
    }
  }
  postSetValueFunctor();
  return success;
}

template <>
bool Property::setValue<ConstantString *>(ConstantString * const & value)
{
  bool success = false;
  preSetValueFunctor();
  if ((getType() == JsonStream::STRING_TYPE) &&
      !stringSavedAsCharArray())
  {
    if (parameter_.valueInSubset(value))
    {
      success = saved_variable_.setValue(value);
    }
  }
  postSetValueFunctor();
  return success;
}

bool Property::setValue(ArduinoJson::JsonArray & value)
{
  bool success = false;
  JsonStream::JsonTypes property_type = getType();
  if (property_type == JsonStream::ARRAY_TYPE)
  {
    size_t array_length = getArrayLength();
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
        if (!stringSavedAsCharArray())
        {
          for (size_t i=0; i<array_length_min; ++i)
          {
            const char * v = value[i];
            success = setElementValue(i,v);
            if (!success)
            {
              break;
            }
          }
        }
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
      case JsonStream::ANY_TYPE:
      {
        break;
      }
    }
    postSetValueFunctor();
  }
  return success;
}

template <>
bool Property::setAllElementValues<const char *>(const char * const & value)
{
  bool success = false;
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::STRING_TYPE) ||
      (stringSavedAsCharArray()))
  {
    return success;
  }
  size_t array_length = getArrayLength();
  preSetValueFunctor();
  for (size_t i=0; i<array_length; ++i)
  {
    success = setElementValue(i,value);
    if (!success)
    {
      break;
    }
  }
  postSetValueFunctor();
  return success;
}

template <>
bool Property::setDefaultValue<long>(const long & default_value)
{
  bool success = false;
  if (getType() == JsonStream::LONG_TYPE)
  {
    success = saved_variable_.setDefaultValue(default_value);
  }
  return success;
}

template <>
bool Property::setDefaultValue<double>(const double & default_value)
{
  bool success = false;
  if (getType() == JsonStream::DOUBLE_TYPE)
  {
    success = saved_variable_.setDefaultValue(default_value);
  }
  return success;
}

template <>
bool Property::setDefaultValue<bool>(const bool & default_value)
{
  bool success = false;
  if (getType() == JsonStream::BOOL_TYPE)
  {
    success = saved_variable_.setDefaultValue(default_value);
  }
  return success;
}

template <>
bool Property::setDefaultValue<const ConstantString *>(const ConstantString * const & default_value)
{
  bool success = false;
  if ((getType() == JsonStream::STRING_TYPE) &&
      !stringSavedAsCharArray())
  {
    success = saved_variable_.setDefaultValue(default_value);
  }
  return success;
}

template <>
bool Property::setDefaultValue<ConstantString *>(ConstantString * const & default_value)
{
  bool success = false;
  if ((getType() == JsonStream::STRING_TYPE) &&
      !stringSavedAsCharArray())
  {
    success = saved_variable_.setDefaultValue(default_value);
  }
  return success;
}

bool Property::setDefaultToRangeMin()
{
  if (rangeIsSet())
  {
    if (getType() == JsonStream::LONG_TYPE)
    {
      const long & range_min = parameter_.getRangeMin().l;
      return setDefaultValue(range_min);
    }
    else if (getType() == JsonStream::DOUBLE_TYPE)
    {
      const double & range_min = parameter_.getRangeMin().d;
      return setDefaultValue(range_min);
    }
  }
  return false;
}

bool Property::setDefaultToRangeMax()
{
  if (rangeIsSet())
  {
    if (getType() == JsonStream::LONG_TYPE)
    {
      const long & range_max = parameter_.getRangeMax().l;
      return setDefaultValue(range_max);
    }
    else if (getType() == JsonStream::DOUBLE_TYPE)
    {
      const double & range_max = parameter_.getRangeMax().d;
      return setDefaultValue(range_max);
    }
  }
  return false;
}

bool Property::setDefaultToSubsetElement(const size_t element_index)
{
  if (subsetIsSet() &&
      (parameter_.getSubset().size() > 0) &&
      (element_index < parameter_.getSubset().size()))
  {
    if (getType() == JsonStream::LONG_TYPE)
    {
      const long & first_subset_value = parameter_.getSubset()[element_index].l;
      setDefaultValue(first_subset_value);
    }
    else if ((getType() == JsonStream::STRING_TYPE) &&
             !stringSavedAsCharArray())
    {
      const ConstantString * & first_subset_value = parameter_.getSubset()[element_index].cs_ptr;
      setDefaultValue(first_subset_value);
    }
  }
  return false;
}

void Property::setValueToDefault()
{
  preSetValueFunctor();
  if (getType() != JsonStream::ARRAY_TYPE)
  {
    saved_variable_.setValueToDefault();
  }
  else
  {
    size_t array_length = getArrayLength();
    for (size_t i=0; i<array_length; ++i)
    {
      setElementValueToDefault(i);
    }
  }
  postSetValueFunctor();
}

void Property::setElementValueToDefault(const size_t element_index)
{
  preSetElementValueFunctor(element_index);
  saved_variable_.setElementValueToDefault(element_index);
  postSetElementValueFunctor(element_index);
}

bool Property::valueIsDefault()
{
  return saved_variable_.valueIsDefault();
}

size_t Property::getArrayLength()
{
  if ((getType() == JsonStream::STRING_TYPE) &&
      (!stringSavedAsCharArray()))
  {
    const ConstantString * cs_ptr;
    getValue(cs_ptr);
    return cs_ptr->length() + 1;
  }
  return saved_variable_.getArrayLength();
}

void Property::setArrayLength(const size_t array_length)
{
  if (getType() == JsonStream::ARRAY_TYPE)
  {
    preSetValueFunctor();
    size_t new_array_length = array_length;
    if (new_array_length < array_length_min_)
    {
      new_array_length = array_length_min_;
    }
    else if (new_array_length > array_length_max_)
    {
      new_array_length = array_length_max_;
    }
    saved_variable_.setArrayLength(new_array_length);
    new_array_length = saved_variable_.getArrayLength();
    parameter_.setArrayLengthRange(new_array_length,new_array_length);
    for (size_t index=0; index<new_array_length; ++index)
    {
      preSetElementValueFunctor(index);
      postSetElementValueFunctor(index);
    }
    postSetValueFunctor();
  }
}

void Property::setArrayLengthRange(const size_t array_length_min,
                                   const size_t array_length_max)
{
  if (getType() == JsonStream::ARRAY_TYPE)
  {
    size_t max = array_length_max;
    if (max < 1)
    {
      max = 1;
    }
    else if (max > saved_variable_.getArrayLengthMax())
    {
      max = saved_variable_.getArrayLengthMax();
    }

    size_t min = array_length_min;
    if (min < 1)
    {
      min = 1;
    }
    else if (min > max)
    {
      min = max;
    }

    array_length_min_ = min;
    array_length_max_ = max;

    size_t array_length = getArrayLength();
    if (array_length < min)
    {
      setArrayLength(min);
    }
    else if (array_length > max)
    {
      setArrayLength(max);
    }
  }
}

size_t Property::getStringLength()
{
  size_t length = 0;
  JsonStream::JsonTypes property_type = getType();
  if (property_type == JsonStream::STRING_TYPE)
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

void Property::setUnits(const ConstantString & name)
{
  parameter_.setUnits(name);
}

void Property::setRange(const long min, const long max)
{
  parameter_.setRange(min,max);
  if (getType() == JsonStream::LONG_TYPE)
  {
    long default_value;
    getDefaultValue(default_value);
    if (!parameter_.valueInRange(default_value))
    {
      // must use reference so default value will have valid pointer
      const long & range_min = parameter_.getRangeMin().l;
      if (default_value < range_min)
      {
        setDefaultToRangeMin();
      }
      else
      {
        setDefaultToRangeMax();
      }
    }
    long value;
    getValue(value);
    if (!parameter_.valueInRange(value))
    {
      setValueToDefault();
    }
  }
  else if ((getType() == JsonStream::ARRAY_TYPE) &&
           (getArrayElementType() == JsonStream::LONG_TYPE))
  {
    size_t array_length = getArrayLength();
    for (size_t i=0; i<array_length; ++i)
    {
      long value;
      getElementValue(i,value);
      if (!parameter_.valueInRange(value))
      {
        setElementValueToDefault(i);
      }
    }
  }
}

void Property::setRange(const double min, const double max)
{
  parameter_.setRange(min,max);
  if (getType() == JsonStream::DOUBLE_TYPE)
  {
    double default_value;
    getDefaultValue(default_value);
    if (!parameter_.valueInRange(default_value))
    {
      // must use reference so default value will have valid pointer
      const double & range_min = parameter_.getRangeMin().d;
      if (default_value < range_min)
      {
        setDefaultToRangeMin();
      }
      else
      {
        setDefaultToRangeMax();
      }
    }
    double value;
    getValue(value);
    if (!parameter_.valueInRange(value))
    {
      setValueToDefault();
    }
  }
  else if ((getType() == JsonStream::ARRAY_TYPE) &&
           (getArrayElementType() == JsonStream::DOUBLE_TYPE))
  {
    size_t array_length = getArrayLength();
    for (size_t i=0; i<array_length; ++i)
    {
      double value;
      getElementValue(i,value);
      if (!parameter_.valueInRange(value))
      {
        setElementValueToDefault(i);
      }
    }
  }
}

void Property::setSubset(constants::SubsetMemberType * subset, size_t max_size, size_t size)
{
  parameter_.setSubset(subset,max_size,size);
  if (getType() == JsonStream::LONG_TYPE)
  {
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

void Property::addValueToSubset(constants::SubsetMemberType & value)
{
  parameter_.addValueToSubset(value);
}

void Property::attachPreSetValueFunctor(const Functor0 & functor)
{
  pre_set_value_functor_ = functor;
}

void Property::attachPreSetElementValueFunctor(const Functor1<const size_t> & functor)
{
  pre_set_element_value_functor_ = functor;
}

void Property::attachPostSetValueFunctor(const Functor0 & functor)
{
  post_set_value_functor_ = functor;
}

void Property::attachPostSetElementValueFunctor(const Functor1<const size_t> & functor)
{
  post_set_element_value_functor_ = functor;
}

void Property::disableFunctors()
{
  functors_enabled_ = false;
}

void Property::reenableFunctors()
{
  functors_enabled_ = true;
}

void Property::writeValue(Response & response,
                          bool write_key,
                          bool write_default,
                          int element_index)
{
  const ConstantString & property_name = getName();
  if (write_key)
  {
    response.writeKey(property_name);
  }
  JsonStream::JsonTypes property_type = getType();
  switch (property_type)
  {
    case JsonStream::LONG_TYPE:
    {
      if (element_index >= 0)
      {
        response.returnParameterInvalidError(constants::property_not_array_type_error_data);
        return;
      }
      long property_value;
      if (write_default)
      {
        getDefaultValue(property_value);
      }
      else
      {
        getValue(property_value);
      }
      response.write(property_value);
      break;
    }
    case JsonStream::DOUBLE_TYPE:
    {
      if (element_index >= 0)
      {
        response.returnParameterInvalidError(constants::property_not_array_type_error_data);
        return;
      }
      double property_value;
      if (write_default)
      {
        getDefaultValue(property_value);
      }
      else
      {
        getValue(property_value);
      }
      response.write(property_value);
      break;
    }
    case JsonStream::BOOL_TYPE:
    {
      if (element_index >= 0)
      {
        response.returnParameterInvalidError(constants::property_not_array_type_error_data);
        return;
      }
      bool property_value;
      if (write_default)
      {
        getDefaultValue(property_value);
      }
      else
      {
        getValue(property_value);
      }
      response.write(property_value);
      break;
    }
    case JsonStream::NULL_TYPE:
    {
      if (element_index >= 0)
      {
        response.returnParameterInvalidError(constants::property_not_array_type_error_data);
        return;
      }
      break;
    }
    case JsonStream::STRING_TYPE:
    {
      size_t array_length = getArrayLength();
      if (element_index >= 0)
      {
        if (element_index >= ((int)array_length-1))
        {
          response.returnParameterInvalidError(constants::property_element_index_out_of_bounds_error_data);
          return;
        }
        size_t array_length = 2;
        char char_array[array_length];
        char property_element_value;
        bool success = getElementValue(element_index,property_element_value);
        if (success)
        {
          char_array[0] = property_element_value;
          char_array[1] = '\0';
        }
        else
        {
          char_array[0] = '\0';
        }
        response.write(char_array);
        return;
      }
      char char_array[array_length];
      if (write_default)
      {
        getDefaultValue(char_array,array_length);
      }
      else
      {
        getValue(char_array,array_length);
      }
      response.write(char_array);
      break;
    }
    case JsonStream::ARRAY_TYPE:
    {
      const JsonStream::JsonTypes array_element_type = getArrayElementType();
      size_t array_length = getArrayLength();
      if (element_index >= (int)array_length)
      {
        response.returnParameterInvalidError(constants::property_element_index_out_of_bounds_error_data);
        return;
      }
      switch (array_element_type)
      {
        case JsonStream::LONG_TYPE:
        {
          if (element_index < 0)
          {
            long property_value[array_length];
            if (write_default)
            {
              getDefaultValue(property_value,array_length);
            }
            else
            {
              getValue(property_value,array_length);
            }
            response.writeArray(property_value,array_length);
          }
          else
          {
            long property_value;
            if (write_default)
            {
              getDefaultElementValue(element_index,property_value);
            }
            else
            {
              getElementValue(element_index,property_value);
            }
            response.write(property_value);
          }
          break;
        }
        case JsonStream::DOUBLE_TYPE:
        {
          if (element_index < 0)
          {
            double property_value[array_length];
            if (write_default)
            {
              getDefaultValue(property_value,array_length);
            }
            else
            {
              getValue(property_value,array_length);
            }
            response.writeArray(property_value,array_length);
          }
          else
          {
            double property_value;
            if (write_default)
            {
              getDefaultElementValue(element_index,property_value);
            }
            else
            {
              getElementValue(element_index,property_value);
            }
            response.write(property_value);
          }
          break;
        }
        case JsonStream::BOOL_TYPE:
        {
          if (element_index < 0)
          {
            bool property_value[array_length];
            if (write_default)
            {
              getDefaultValue(property_value,array_length);
            }
            else
            {
              getValue(property_value,array_length);
            }
            response.writeArray(property_value,array_length);
          }
          else
          {
            bool property_value;
            if (write_default)
            {
              getDefaultElementValue(element_index,property_value);
            }
            else
            {
              getElementValue(element_index,property_value);
            }
            response.write(property_value);
          }
          break;
        }
        case JsonStream::NULL_TYPE:
        {
          break;
        }
        case JsonStream::STRING_TYPE:
        {
          if (element_index < 0)
          {
            const ConstantString * property_value[array_length];
            if (write_default)
            {
              getDefaultValue(property_value,array_length);
            }
            else
            {
              getValue(property_value,array_length);
            }
            response.writeArray(property_value,array_length);
          }
          else
          {
            const ConstantString * property_value;
            if (write_default)
            {
              getDefaultElementValue(element_index,property_value);
            }
            else
            {
              getElementValue(element_index,property_value);
            }
            response.write(property_value);
          }
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
        case JsonStream::ANY_TYPE:
        {
          break;
        }
      }
      break;
    }
    case JsonStream::OBJECT_TYPE:
    {
      break;
    }
    case JsonStream::ANY_TYPE:
    {
      break;
    }
  }
}

void Property::writeApi(Response & response,
                        bool write_name_only,
                        bool write_firmware,
                        bool verbose,
                        bool write_instance_details)
{
  const ConstantString & name = getName();
  if (write_name_only)
  {
    response.write(name);
    return;
  }

  updateFunctionsAndParameters();

  parameter().writeApi(response,false,true,write_firmware,write_instance_details);

  if (write_instance_details)
  {
    if (getType() == JsonStream::ARRAY_TYPE)
    {
      response.write(constants::array_length_min_constant_string,getArrayLengthMin());
      response.write(constants::array_length_max_constant_string,getArrayLengthMax());
    }

    response.writeKey(constants::value_constant_string);
    writeValue(response,false,false);

    response.writeKey(constants::default_value_constant_string);
    writeValue(response,false,true);

    if ((getType() == JsonStream::STRING_TYPE) &&
        (stringSavedAsCharArray()))
    {
      response.write(constants::string_length_constant_string,getStringLength());
      size_t string_length_max = getArrayLength() - 1;
      response.write(constants::string_length_max_constant_string,string_length_max);
    }
  }

  response.writeKey(constants::functions_constant_string);
  response.beginArray();
  for (size_t i=0; i<Property::functions_.size(); ++i)
  {
    Function & function = Property::functions_[i];
    function.writeApi(response,!verbose,false,false);
  }
  response.endArray();

  response.writeKey(constants::parameters_constant_string);
  response.beginArray();
  for (size_t i=0; i<Property::parameters_.size(); ++i)
  {
    Parameter & parameter = Property::parameters_[i];
    parameter.writeApi(response,!verbose,false,false,write_instance_details);
  }
  response.endArray();

  response.endObject();
}

// private
template <>
Property::Property<long>(const ConstantString & name,
                         const long & default_value) :
  parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeLong();
}

template <>
Property::Property<double>(const ConstantString & name,
                           const double & default_value) :
  parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeDouble();
}

template <>
Property::Property<bool>(const ConstantString & name,
                         const bool & default_value) :
  parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeBool();
}

template <>
Property::Property<const ConstantString *>(const ConstantString & name,
                                           const ConstantString * const & default_value) :
  parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeString();
  string_saved_as_char_array_ = false;
}

Parameter & Property::parameter()
{
  return parameter_;
}

bool Property::compareName(const char * name_to_compare)
{
  return parameter_.compareName(name_to_compare);
}

bool Property::compareName(const ConstantString & name_to_compare)
{
  return parameter_.compareName(name_to_compare);
}

const ConstantString & Property::getName()
{
  return parameter_.getName();
}

const ConstantString & Property::getFirmwareName()
{
  return parameter_.getFirmwareName();
}

bool Property::firmwareNameInArray(ArduinoJson::JsonArray & firmware_name_array)
{
  return parameter_.firmwareNameInArray(firmware_name_array);
}

JsonStream::JsonTypes Property::getType()
{
  return parameter_.getType();
}

JsonStream::JsonTypes Property::getArrayElementType()
{
  return parameter_.getArrayElementType();
}

bool Property::rangeIsSet()
{
  return parameter_.rangeIsSet();
}

bool Property::subsetIsSet()
{
  return parameter_.subsetIsSet();
}

size_t Property::getArrayLengthMin()
{
  return array_length_min_;
}

size_t Property::getArrayLengthMax()
{
  return array_length_max_;
}

bool Property::stringSavedAsCharArray()
{
  return string_saved_as_char_array_;
}

int Property::findSubsetValueIndex(const long value)
{
  return parameter_.findSubsetValueIndex(value);
}

int Property::findSubsetValueIndex(const char * value)
{
  return parameter_.findSubsetValueIndex(value);
}

int Property::findSubsetValueIndex(const ConstantString * value)
{
  return parameter_.findSubsetValueIndex(value);
}

bool Property::valueInSubset(const long value)
{
  return parameter_.valueInSubset(value);
}

bool Property::valueInSubset(const char * value)
{
  return parameter_.valueInSubset(value);
}

bool Property::valueInSubset(const ConstantString * value)
{
  return parameter_.valueInSubset(value);
}

Vector<constants::SubsetMemberType> & Property::getSubset()
{
  return parameter_.getSubset();
}

void Property::preSetValueFunctor()
{
  if (pre_set_value_functor_ && functors_enabled_)
  {
    pre_set_value_functor_();
  }
}

void Property::preSetElementValueFunctor(const size_t element_index)
{
  if (pre_set_element_value_functor_ && functors_enabled_)
  {
    pre_set_element_value_functor_(element_index);
  }
}

void Property::postSetValueFunctor()
{
  if (post_set_value_functor_ && functors_enabled_)
  {
    post_set_value_functor_();
  }
}

void Property::postSetElementValueFunctor(const size_t element_index)
{
  if (post_set_element_value_functor_ && functors_enabled_)
  {
    post_set_element_value_functor_(element_index);
  }
}

void Property::updateFunctionsAndParameters()
{
  JsonStream::JsonTypes type = getType();

  // Parameters
  parameters_.clear();
  parameters_.addArray(property_parameters_);

  Parameter & value_parameter = copyParameter(parameter(),property::value_parameter_name);

  // Functions
  functions_.clear();
  functions_.addArray(property_functions_);

  Function & get_value_function = createFunction(property::get_value_function_name);
  get_value_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Property::getValueHandler));
  get_value_function.setResultType(type);

  Function & set_value_function = createFunction(property::set_value_function_name);
  set_value_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Property::setValueHandler));
  set_value_function.addParameter(value_parameter);
  set_value_function.setResultType(type);

  Function & get_default_value_function = createFunction(property::get_default_value_function_name);
  get_default_value_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Property::getDefaultValueHandler));
  get_default_value_function.setResultType(type);

  Function & set_value_to_default_function = createFunction(property::set_value_to_default_function_name);
  set_value_to_default_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Property::setValueToDefaultHandler));
  set_value_to_default_function.setResultType(type);

  if ((type == JsonStream::ARRAY_TYPE) || ((type == JsonStream::STRING_TYPE) && stringSavedAsCharArray()))
  {
    JsonStream::JsonTypes array_element_type = getArrayElementType();

    // Update Function Result Type
    get_value_function.setResultType(array_element_type);
    set_value_function.setResultType(array_element_type);
    get_default_value_function.setResultType(array_element_type);
    set_value_to_default_function.setResultType(array_element_type);

    // Array Parameters
    parameters_.addArray(property_array_parameters_);

    Parameter & element_index_parameter = createParameter(property::element_index_parameter_name);
    element_index_parameter.setTypeLong();
    size_t element_index_min = 0;
    size_t element_index_max;
    if (type == JsonStream::ARRAY_TYPE)
    {
      element_index_max = getArrayLength() - 1;
    }
    else
    {
      // leave room for string termination character
      element_index_max = getArrayLength() - 2;
    }
    element_index_parameter.setRange(element_index_min,element_index_max);

    Parameter & element_value_parameter = copyParameter(parameter().getElementParameter(),property::element_value_parameter_name);

    Parameter * array_length_parameter_ptr = NULL;
    if (type == JsonStream::ARRAY_TYPE)
    {
      array_length_parameter_ptr = &(createParameter(property::array_length_parameter_name));
      array_length_parameter_ptr->setTypeLong();
      array_length_parameter_ptr->setRange(array_length_min_,array_length_max_);
    }

    // Array Functions
    functions_.addArray(property_array_functions_);

    Function & get_element_value_function = createFunction(property::get_element_value_function_name);
    get_element_value_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Property::getElementValueHandler));
    get_element_value_function.addParameter(element_index_parameter);
    get_element_value_function.setResultType(array_element_type);

    Function & set_element_value_function = createFunction(property::set_element_value_function_name);
    set_element_value_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Property::setElementValueHandler));
    set_element_value_function.addParameter(element_index_parameter);
    set_element_value_function.addParameter(element_value_parameter);
    set_element_value_function.setResultType(type);

    Function & get_default_element_value_function = createFunction(property::get_default_element_value_function_name);
    get_default_element_value_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Property::getDefaultElementValueHandler));
    get_default_element_value_function.addParameter(element_index_parameter);
    get_default_element_value_function.setResultType(array_element_type);

    Function & set_element_value_to_default_function = createFunction(property::set_element_value_to_default_function_name);
    set_element_value_to_default_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Property::setElementValueToDefaultHandler));
    set_element_value_to_default_function.addParameter(element_index_parameter);
    set_element_value_to_default_function.setResultType(type);

    Function & set_all_element_values_function = createFunction(property::set_all_element_values_function_name);
    set_all_element_values_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Property::setAllElementValuesHandler));
    set_all_element_values_function.addParameter(element_value_parameter);
    set_all_element_values_function.setResultType(type);

    if (type == JsonStream::ARRAY_TYPE)
    {
      set_element_value_function.setResultType(array_element_type);
      set_element_value_to_default_function.setResultType(array_element_type);
      set_all_element_values_function.setResultType(array_element_type);

      Function & get_array_length_function = createFunction(property::get_array_length_function_name);
      get_array_length_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Property::getArrayLengthHandler));
      get_array_length_function.setResultTypeLong();

      Function & set_array_length_function = createFunction(property::set_array_length_function_name);
      set_array_length_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Property::setArrayLengthHandler));
      set_array_length_function.addParameter(*array_length_parameter_ptr);
      set_array_length_function.setResultTypeLong();
    }
  }
}

void Property::getValueHandler()
{
  response_ptr_->writeResultKey();
  writeValue(*response_ptr_,false,false,-1);
}

void Property::setValueHandler()
{
  JsonStream::JsonTypes type = getType();
  switch (type)
  {
    case JsonStream::LONG_TYPE:
    {
      long value = get_parameter_value_functor_(property::value_parameter_name);
      setValue(value);
      break;
    }
    case JsonStream::DOUBLE_TYPE:
    {
      double value = get_parameter_value_functor_(property::value_parameter_name);
      setValue(value);
      break;
    }
    case JsonStream::BOOL_TYPE:
    {
      bool value = get_parameter_value_functor_(property::value_parameter_name);
      setValue(value);
      break;
    }
    case JsonStream::NULL_TYPE:
    {
      break;
    }
    case JsonStream::STRING_TYPE:
    {
      const char * value = get_parameter_value_functor_(property::value_parameter_name);
      size_t array_length = strlen(value) + 1;
      setValue(value,array_length);
      break;
    }
    case JsonStream::OBJECT_TYPE:
    {
      break;
    }
    case JsonStream::ARRAY_TYPE:
    {
      ArduinoJson::JsonArray & value = get_parameter_value_functor_(property::value_parameter_name);
      setValue(value);
      break;
    }
    case JsonStream::ANY_TYPE:
    {
      break;
    }
  }
  response_ptr_->writeResultKey();
  writeValue(*response_ptr_,false,false,-1);
}

void Property::getDefaultValueHandler()
{
  response_ptr_->writeResultKey();
  writeValue(*response_ptr_,false,true,-1);
}

void Property::setValueToDefaultHandler()
{
  setValueToDefault();
  response_ptr_->writeResultKey();
  writeValue(*response_ptr_,false,false,-1);
}

void Property::getElementValueHandler()
{
  long element_index = get_parameter_value_functor_(property::element_index_parameter_name);
  response_ptr_->writeResultKey();
  writeValue(*response_ptr_,false,false,element_index);
}

void Property::setElementValueHandler()
{
  long element_index = get_parameter_value_functor_(property::element_index_parameter_name);

  JsonStream::JsonTypes type = getType();
  switch (type)
  {
    case JsonStream::LONG_TYPE:
    {
      response_ptr_->returnParameterInvalidError(constants::property_not_array_type_error_data);
      break;
    }
    case JsonStream::DOUBLE_TYPE:
    {
      response_ptr_->returnParameterInvalidError(constants::property_not_array_type_error_data);
      break;
    }
    case JsonStream::BOOL_TYPE:
    {
      response_ptr_->returnParameterInvalidError(constants::property_not_array_type_error_data);
      break;
    }
    case JsonStream::NULL_TYPE:
    {
      break;
    }
    case JsonStream::STRING_TYPE:
    {
      if (!stringSavedAsCharArray())
      {
        response_ptr_->returnParameterInvalidError(constants::cannot_set_element_in_string_property_with_subset_error_data);
        break;
      }
      size_t array_length = getArrayLength();
      if ((size_t)element_index >= (array_length - 1))
      {
        response_ptr_->returnParameterInvalidError(constants::property_element_index_out_of_bounds_error_data);
        return;
      }
      const char * value = get_parameter_value_functor_(property::element_value_parameter_name);
      size_t string_length = strlen(value);
      if (string_length >= 1)
      {
        char v = value[0];
        setElementValue(element_index,v);
      }
      break;
    }
    case JsonStream::OBJECT_TYPE:
    {
      break;
    }
    case JsonStream::ARRAY_TYPE:
    {
      size_t array_length = getArrayLength();
      if ((size_t)element_index >= array_length)
      {
        response_ptr_->returnParameterInvalidError(constants::property_element_index_out_of_bounds_error_data);
        return;
      }
      JsonStream::JsonTypes array_element_type = getArrayElementType();
      switch (array_element_type)
      {
        case JsonStream::LONG_TYPE:
        {
          long value = get_parameter_value_functor_(property::element_value_parameter_name);
          setElementValue(element_index,value);
          break;
        }
        case JsonStream::DOUBLE_TYPE:
        {
          double value = get_parameter_value_functor_(property::element_value_parameter_name);
          setElementValue(element_index,value);
          break;
        }
        case JsonStream::BOOL_TYPE:
        {
          bool value = get_parameter_value_functor_(property::element_value_parameter_name);
          setElementValue(element_index,value);
          break;
        }
        case JsonStream::NULL_TYPE:
        {
          break;
        }
        case JsonStream::STRING_TYPE:
        {
          const char * value = get_parameter_value_functor_(property::element_value_parameter_name);
          setElementValue(element_index,value);
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
        case JsonStream::ANY_TYPE:
        {
          break;
        }
      }
      break;
    }
    case JsonStream::ANY_TYPE:
    {
      break;
    }
  }
  response_ptr_->writeResultKey();
  writeValue(*response_ptr_,false,false,-1);
}

void Property::getDefaultElementValueHandler()
{
  long element_index = get_parameter_value_functor_(property::element_index_parameter_name);
  response_ptr_->writeResultKey();
  writeValue(*response_ptr_,false,true,element_index);
}

void Property::setElementValueToDefaultHandler()
{
  long element_index = get_parameter_value_functor_(property::element_index_parameter_name);
  setElementValueToDefault(element_index);
  response_ptr_->writeResultKey();
  writeValue(*response_ptr_,false,false,-1);
}

void Property::setAllElementValuesHandler()
{
  JsonStream::JsonTypes type = getType();
  switch (type)
  {
    case JsonStream::LONG_TYPE:
    {
      break;
    }
    case JsonStream::DOUBLE_TYPE:
    {
      break;
    }
    case JsonStream::BOOL_TYPE:
    {
      break;
    }
    case JsonStream::NULL_TYPE:
    {
      break;
    }
    case JsonStream::STRING_TYPE:
    {
      if (!stringSavedAsCharArray())
      {
        response_ptr_->returnParameterInvalidError(constants::cannot_set_element_in_string_property_with_subset_error_data);
        break;
      }
      const char * value = get_parameter_value_functor_(property::element_value_parameter_name);
      size_t string_length = strlen(value);
      if (string_length >= 1)
      {
        char v = value[0];
        setAllElementValues(v);
      }
      break;
    }
    case JsonStream::OBJECT_TYPE:
    {
      break;
    }
    case JsonStream::ARRAY_TYPE:
    {
      JsonStream::JsonTypes array_element_type = getArrayElementType();
      switch (array_element_type)
      {
        case JsonStream::LONG_TYPE:
        {
          long value = get_parameter_value_functor_(property::element_value_parameter_name);
          setAllElementValues(value);
          break;
        }
        case JsonStream::DOUBLE_TYPE:
        {
          double value = get_parameter_value_functor_(property::element_value_parameter_name);
          setAllElementValues(value);
          break;
        }
        case JsonStream::BOOL_TYPE:
        {
          bool value = get_parameter_value_functor_(property::element_value_parameter_name);
          setAllElementValues(value);
          break;
        }
        case JsonStream::NULL_TYPE:
        {
          break;
        }
        case JsonStream::STRING_TYPE:
        {
          const char * value = get_parameter_value_functor_(property::element_value_parameter_name);
          setAllElementValues(value);
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
        case JsonStream::ANY_TYPE:
        {
          break;
        }
      }
      break;
    }
    case JsonStream::ANY_TYPE:
    {
      break;
    }
  }
  response_ptr_->writeResultKey();
  writeValue(*response_ptr_,false,false,-1);
}

void Property::getArrayLengthHandler()
{
  size_t array_length = getArrayLength();
  response_ptr_->returnResult(array_length);
}

void Property::setArrayLengthHandler()
{
  long array_length = get_parameter_value_functor_(property::array_length_parameter_name);
  setArrayLength(array_length);

  array_length = getArrayLength();
  response_ptr_->returnResult(array_length);
}

}
