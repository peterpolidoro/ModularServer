// ----------------------------------------------------------------------------
// ParameterDefinitions.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_PARAMETER_DEFINITIONS_H_
#define _MODULAR_SERVER_PARAMETER_DEFINITIONS_H_


namespace modular_server
{
// public
template <typename T>
void Parameter::setRange(T min,
  T max)
{
  min_.l = (long)min;
  max_.l = (long)max;
  setTypeLong();
  range_is_set_ = true;

  if (array_length_range_is_set_)
  {
    size_t max_value_count = abs((long)max - (long)min) + 1;
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

template <size_t MAX_SIZE>
void Parameter::setSubset(constants::SubsetMemberType (&subset)[MAX_SIZE],
  size_t size)
{
  subset_.setStorage(subset,size);
  subset_is_set_ = true;

  if (array_length_range_is_set_)
  {
    size_t max_value_count = MAX_SIZE;
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

template <typename T>
bool Parameter::getValue(T & value)
{
  if (getType() == JsonStream::LONG_TYPE)
  {
    long v = get_value_functor_(getName());
    value = v;
    return true;
  }
  else if (getType() == JsonStream::DOUBLE_TYPE)
  {
    double v = get_value_functor_(getName());
    value = v;
    return true;
  }
  else if (getType() == JsonStream::BOOL_TYPE)
  {
    bool v = get_value_functor_(getName());
    value = v;
    return true;
  }
  return false;
}

// private
template <typename T>
bool Parameter::valueInRange(T value)
{
  bool in_range = true;
  if (rangeIsSet())
  {
    long min = getRangeMin().l;
    long max = getRangeMax().l;
    if (((long)value < min) || ((long)value > max))
    {
      in_range = false;
    }
  }
  return in_range;
}

}
#endif
