// ----------------------------------------------------------------------------
// ParameterDefinitions.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_PARAMETER_DEFINITIONS_H_
#define _MODULAR_SERVER_PARAMETER_DEFINITIONS_H_


namespace modular_server
{
// public
template <typename T>
void Parameter::setRange(const T min, const T max)
{
  min_.l = (long)min;
  max_.l = (long)max;
  setTypeLong();
  range_is_set_ = true;
}

template <size_t N>
void Parameter::setSubset(const constants::SubsetMemberType (&subset)[N])
{
  subset_.setStorage(subset,N);
  subset_is_set_ = true;
}

template <typename T>
bool Parameter::getValue(T & value)
{
  if (getType() == JsonStream::LONG_TYPE)
  {
    long v = get_value_callback_(getName());
    value = v;
    return true;
  }
  else if (getType() == JsonStream::DOUBLE_TYPE)
  {
    double v = get_value_callback_(getName());
    value = v;
    return true;
  }
  else if (getType() == JsonStream::BOOL_TYPE)
  {
    bool v = get_value_callback_(getName());
    value = v;
    return true;
  }
  return false;
}

// private
template <typename T>
bool Parameter::valueInRange(const T value)
{
  bool in_range = true;
  if (rangeIsSet())
  {
    long min = getMin().l;
    long max = getMax().l;
    if (((long)value < min) || ((long)value > max))
    {
      in_range = false;
    }
  }
  return in_range;
}

}
#endif
