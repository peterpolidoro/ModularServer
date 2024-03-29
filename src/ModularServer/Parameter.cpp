// ----------------------------------------------------------------------------
// Parameter.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "Parameter.h"


namespace modular_server
{
Functor1wRet<const ConstantString &,ArduinoJson::JsonVariant> Parameter::get_value_functor_;

// public
Parameter::Parameter()
{
  setup(constants::empty_constant_string);
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
    array_element_type_ = JsonStream::STRING_TYPE;
  }
  else
  {
    array_element_type_ = JsonStream::STRING_TYPE;
  }
}

void Parameter::setTypeObject()
{
  if (type_ != JsonStream::ARRAY_TYPE)
  {
    type_ = JsonStream::OBJECT_TYPE;
  }
  else
  {
    array_element_type_ = JsonStream::OBJECT_TYPE;
  }
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

void Parameter::setUnits(const ConstantString & units)
{
  units_ptr_ = &units;
}

void Parameter::setRange(double min,
  double max)
{
  min_.d = min;
  max_.d = max;
  setTypeDouble();
  range_is_set_ = true;
}

void Parameter::setRange(float min,
  float max)
{
  min_.d = (double)min;
  max_.d = (double)max;
  setTypeDouble();
  range_is_set_ = true;
}

void Parameter::setRange(constants::NumberType min,
  constants::NumberType max)
{
  min_ = min;
  max_ = max;
  range_is_set_ = true;

  if ((array_length_range_is_set_) && (type_ == JsonStream::LONG_TYPE))
  {
    size_t max_value_count = abs(max.l - min.l) + 1;
    if (array_length_min_ > max_value_count)
    {
      array_length_min_ = max_value_count;
    }
    if (array_length_max_ > max_value_count)
    {
      array_length_max_ = max_value_count;
    }
  }
}

void Parameter::removeRange()
{
  range_is_set_ = false;
}

void Parameter::setArrayLengthRange(size_t array_length_min,
  size_t array_length_max)
{
  setTypeArray();
  array_length_min_ = array_length_min;
  array_length_max_ = array_length_max;
  array_length_range_is_set_ = true;

  if (range_is_set_ && (type_ == JsonStream::LONG_TYPE))
  {
    size_t max_value_count = abs(max_.l - min_.l) + 1;
    if (array_length_min_ > max_value_count)
    {
      array_length_min_ = max_value_count;
    }
    if (array_length_max_ > max_value_count)
    {
      array_length_max_ = max_value_count;
    }
  }

  if (subset_is_set_)
  {
    size_t max_value_count = subset_.max_size();
    if (array_length_min_ > max_value_count)
    {
      array_length_min_ = max_value_count;
    }
    if (array_length_max_ > max_value_count)
    {
      array_length_max_ = max_value_count;
    }
  }
}

void Parameter::removeArrayLengthRange()
{
  array_length_range_is_set_ = false;
}

void Parameter::setSubset(constants::SubsetMemberType * subset,
  size_t max_size,
  size_t size)
{
  subset_.setStorage(subset,max_size,size);
  subset_is_set_ = true;

  if (array_length_range_is_set_)
  {
    size_t max_value_count = max_size;
    if (array_length_min_ > max_value_count)
    {
      array_length_min_ = max_value_count;
    }
    if (array_length_max_ > max_value_count)
    {
      array_length_max_ = max_value_count;
    }
  }
}

void Parameter::setSubset(Vector<constants::SubsetMemberType> & subset)
{
  subset_ = subset;
  subset_is_set_ = true;

  if (array_length_range_is_set_)
  {
    size_t max_value_count = subset.max_size();
    if (array_length_min_ > max_value_count)
    {
      array_length_min_ = max_value_count;
    }
    if (array_length_max_ > max_value_count)
    {
      array_length_max_ = max_value_count;
    }
  }
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

size_t Parameter::getSubsetSize()
{
  return subset_.size();
}

size_t Parameter::getSubsetMaxSize()
{
  return subset_.max_size();
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
    value = NULL;
    return false;
  }
  value = get_value_functor_(getName());
  return true;
}

bool Parameter::getValue(ArduinoJson::JsonArray & value)
{
  if (getType() != JsonStream::ARRAY_TYPE)
  {
    return false;
  }
  value = get_value_functor_(getName());
  return true;
}

bool Parameter::getValue(ArduinoJson::JsonObject & value)
{
  if (getType() != JsonStream::OBJECT_TYPE)
  {
    return false;
  }
  value = get_value_functor_(getName());
  return true;
}

bool Parameter::getValue(const ConstantString * & value)
{
  if ((getType() != JsonStream::STRING_TYPE) && (getType() != JsonStream::ANY_TYPE))
  {
    value = NULL;
    return false;
  }
  const char * string_value = get_value_functor_(getName());
  int subset_value_index = findSubsetValueIndex(string_value);
  if (subset_value_index < 0)
  {
    value = NULL;
    return false;
  }
  value = subset_[subset_value_index].cs_ptr;
  return true;
}

Parameter Parameter::getElementParameter()
{
  Parameter element_parameter;
  element_parameter.setType(getArrayElementType());
  element_parameter.setUnits(getUnits());
  if (rangeIsSet())
  {
    element_parameter.setRange(getRangeMin(),getRangeMax());
  }
  if (subsetIsSet())
  {
    element_parameter.setSubset(getSubset());
  }
  return element_parameter;
}

// private
Parameter::Parameter(const ConstantString & name)
{
  setup(name);
}

void Parameter::setup(const ConstantString & name)
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

bool Parameter::valueInRange(double value)
{
  bool in_range = true;
  if (rangeIsSet())
  {
    double min = getRangeMin().d - constants::epsilon;
    double max = getRangeMax().d + constants::epsilon;
    if ((value < min) || (value > max))
    {
      in_range = false;
    }
  }
  return in_range;
}

bool Parameter::valueInRange(float value)
{
  bool in_range = true;
  if (rangeIsSet())
  {
    double min = getRangeMin().d - constants::epsilon;
    double max = getRangeMax().d + constants::epsilon;
    if (((double)value < min) || ((double)value > max))
    {
      in_range = false;
    }
  }
  return in_range;
}

const constants::NumberType & Parameter::getRangeMin()
{
  return min_;
}

const constants::NumberType & Parameter::getRangeMax()
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

bool Parameter::arrayLengthInRange(size_t array_length)
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

int Parameter::findSubsetValueIndex(long value)
{
  int value_index = -1;
  if (subsetIsSet())
  {
    for (size_t i=0; i<subset_.size(); ++i)
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
    for (size_t i=0; i<subset_.size(); ++i)
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

int Parameter::findSubsetValueIndex(const ConstantString * value)
{
  int value_index = -1;
  if (subsetIsSet())
  {
    for (size_t i=0; i<subset_.size(); ++i)
    {
      if (value == subset_[i].cs_ptr)
      {
        value_index = i;
        break;
      }
    }
  }
  return value_index;
}

bool Parameter::valueInSubset(long value)
{
  bool in_subset = true;
  if (subsetIsSet())
  {
    int subset_value_index = findSubsetValueIndex(value);
    in_subset = (subset_value_index >= 0);
  }
  return in_subset;
}

bool Parameter::valueInSubset(const char * value)
{
  bool in_subset = true;
  if (subsetIsSet())
  {
    int subset_value_index = findSubsetValueIndex(value);
    in_subset = (subset_value_index >= 0);
  }
  return in_subset;
}

bool Parameter::valueInSubset(const ConstantString * value)
{
  bool in_subset = true;
  if (subsetIsSet())
  {
    int subset_value_index = findSubsetValueIndex(value);
    in_subset = (subset_value_index >= 0);
  }
  return in_subset;
}

Vector<constants::SubsetMemberType> & Parameter::getSubset()
{
  return subset_;
}

void Parameter::writeApi(Response & response,
  bool write_name_only,
  bool write_method_type,
  bool is_property,
  bool write_firmware,
  bool write_instance_details)
{
  if (response.error())
  {
    return;
  }

  const ConstantString & name = getName();
  if (write_name_only)
  {
    response.write(name);
    return;
  }

  response.beginObject();

  response.write(constants::name_constant_string,name);

  if (is_property && write_method_type)
  {
    response.write(constants::method_type_constant_string,constants::property_constant_string);
  }

  if (write_firmware)
  {
    const ConstantString & firmware_name = getFirmwareName();
    response.write(constants::firmware_constant_string,firmware_name);
  }

  JsonStream::JsonTypes type = getType();
  if (write_instance_details)
  {
    switch (type)
    {
      case JsonStream::LONG_TYPE:
      {
        response.write(constants::type_constant_string,JsonStream::LONG_TYPE);
        if (subsetIsSet())
        {
          response.writeKey(constants::subset_constant_string);
          response.write(getSubset(),JsonStream::LONG_TYPE);
        }
        if (rangeIsSet())
        {
          long min = getRangeMin().l;
          long max = getRangeMax().l;
          response.write(constants::min_constant_string,min);
          response.write(constants::max_constant_string,max);
        }
        break;
      }
      case JsonStream::DOUBLE_TYPE:
      {
        response.write(constants::type_constant_string,JsonStream::DOUBLE_TYPE);
        if (rangeIsSet())
        {
          double min = getRangeMin().d;
          double max = getRangeMax().d;
          response.write(constants::min_constant_string,min);
          response.write(constants::max_constant_string,max);
        }
        break;
      }
      case JsonStream::BOOL_TYPE:
      {
        response.write(constants::type_constant_string,JsonStream::BOOL_TYPE);
        break;
      }
      case JsonStream::NULL_TYPE:
      {
        break;
      }
      case JsonStream::STRING_TYPE:
      {
        response.write(constants::type_constant_string,JsonStream::STRING_TYPE);
        if (subsetIsSet())
        {
          response.writeKey(constants::subset_constant_string);
          response.write(getSubset(),JsonStream::STRING_TYPE);
        }
        break;
      }
      case JsonStream::OBJECT_TYPE:
      {
        response.write(constants::type_constant_string,JsonStream::OBJECT_TYPE);
        break;
      }
      case JsonStream::ARRAY_TYPE:
      {
        response.write(constants::type_constant_string,JsonStream::ARRAY_TYPE);
        JsonStream::JsonTypes array_element_type = getArrayElementType();
        switch (array_element_type)
        {
          case JsonStream::LONG_TYPE:
          {
            response.write(constants::array_element_type_constant_string,JsonStream::LONG_TYPE);
            if (subsetIsSet())
            {
              response.writeKey(constants::array_element_subset_constant_string);
              response.write(getSubset(),JsonStream::LONG_TYPE);
            }
            if (rangeIsSet())
            {
              long min = getRangeMin().l;
              long max = getRangeMax().l;
              response.write(constants::array_element_min_constant_string,min);
              response.write(constants::array_element_max_constant_string,max);
            }
            break;
          }
          case JsonStream::DOUBLE_TYPE:
          {
            response.write(constants::array_element_type_constant_string,JsonStream::DOUBLE_TYPE);
            if (rangeIsSet())
            {
              double min = getRangeMin().d;
              double max = getRangeMax().d;
              response.write(constants::array_element_min_constant_string,min);
              response.write(constants::array_element_max_constant_string,max);
            }
            break;
          }
          case JsonStream::BOOL_TYPE:
          {
            response.write(constants::array_element_type_constant_string,JsonStream::BOOL_TYPE);
            break;
          }
          case JsonStream::NULL_TYPE:
          {
            break;
          }
          case JsonStream::STRING_TYPE:
          {
            response.write(constants::array_element_type_constant_string,JsonStream::STRING_TYPE);
            if (subsetIsSet())
            {
              response.writeKey(constants::array_element_subset_constant_string);
              response.write(getSubset(),JsonStream::STRING_TYPE);
            }
            break;
          }
          case JsonStream::OBJECT_TYPE:
          {
            response.write(constants::array_element_type_constant_string,JsonStream::OBJECT_TYPE);
            break;
          }
          case JsonStream::ARRAY_TYPE:
          {
            response.write(constants::array_element_type_constant_string,JsonStream::ARRAY_TYPE);
            break;
          }
          case JsonStream::ANY_TYPE:
          {
            response.write(constants::array_element_type_constant_string,JsonStream::ANY_TYPE);
            break;
          }
        }
        if (arrayLengthRangeIsSet() && !is_property)
        {
          size_t array_length_min = getArrayLengthMin();
          size_t array_length_max = getArrayLengthMax();
          response.write(constants::array_length_min_constant_string,array_length_min);
          response.write(constants::array_length_max_constant_string,array_length_max);
        }
        break;
      }
      case JsonStream::ANY_TYPE:
      {
        response.write(constants::type_constant_string,JsonStream::ANY_TYPE);
        break;
      }
    }
  }
  else
  {
    response.write(constants::type_constant_string,type);
    if (type == JsonStream::ARRAY_TYPE)
    {
      JsonStream::JsonTypes array_element_type = getArrayElementType();
      response.write(constants::array_element_type_constant_string,array_element_type);
    }
  }

  if (write_instance_details)
  {
    const ConstantString & units = getUnits();
    if (units.length() != 0)
    {
      response.write(constants::units_constant_string,units);
    }
  }

  if (!is_property)
  {
    response.endObject();
  }
}

}
