// ----------------------------------------------------------------------------
// ServerDefinitions.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_SERVER_DEFINITIONS_H_
#define _MODULAR_SERVER_SERVER_DEFINITIONS_H_


namespace modular_server
{
// public

// Streams

// Device ID

// Hardware Info

// Firmware
template <size_t PROPERTIES_MAX_SIZE,
          size_t PARAMETERS_MAX_SIZE,
          size_t METHODS_MAX_SIZE,
          size_t CALLBACKS_MAX_SIZE>
void Server::addFirmware(const constants::FirmwareInfo & firmware_info,
                         Property (&properties)[PROPERTIES_MAX_SIZE],
                         Parameter (&parameters)[PARAMETERS_MAX_SIZE],
                         Method (&methods)[METHODS_MAX_SIZE],
                         Callback (&callbacks)[CALLBACKS_MAX_SIZE])
{
  firmware_info_array_.push_back(&firmware_info);
  constants::SubsetMemberType firmware_name;
  firmware_name.cs_ptr = firmware_info.name_ptr;
  firmware_name_array_.push_back(firmware_name);
  parameter(constants::firmware_constant_string).addValueToSubset(firmware_name_array_.back());
  properties_.addArray(properties);
  parameters_.addArray(parameters);
  methods_.addArray(methods);
  callbacks_.addArray(callbacks);
}

// Properties
template <typename T>
Property & Server::createProperty(const ConstantString & property_name,
                           const T & default_value)
{
  int property_index = findPropertyIndex(property_name);
  if (property_index < 0)
  {
    properties_.push_back(Property(property_name,
                            default_value));
    const ConstantString * firmware_name_ptr = firmware_info_array_.back()->name_ptr;
    properties_.back().parameter().setFirmwareName(*firmware_name_ptr);
    return properties_.back();
  }
}

template <typename T, size_t N>
Property & Server::createProperty(const ConstantString & property_name,
                           const T (&default_value)[N])
{
  int property_index = findPropertyIndex(property_name);
  if (property_index < 0)
  {
    properties_.push_back(Property(property_name,
                            default_value));
    const ConstantString * firmware_name_ptr = firmware_info_array_.back()->name_ptr;
    properties_.back().parameter().setFirmwareName(*firmware_name_ptr);
    return properties_.back();
  }
}

// Parameters

// Methods

// Callbacks

// Response

// private
template <typename T>
int Server::findPropertyIndex(T const & property_name)
{
  int property_index = -1;
  for (size_t i=0; i<properties_.size(); ++i)
  {
    if (properties_[i].parameter().compareName(property_name))
    {
      property_index = i;
      break;
    }
  }
  return property_index;
}

template <typename T>
int Server::findParameterIndex(T const & parameter_name)
{
  int parameter_index = -1;
  for (size_t i=0; i<parameters_.size(); ++i)
  {
    if (parameters_[i].compareName(parameter_name))
    {
      parameter_index = i;
      break;
    }
  }
  return parameter_index;
}

template <typename T>
int Server::findMethodParameterIndex(int method_index, T const & parameter_name)
{
  int parameter_index = -1;
  if ((method_index >= 0) && (method_index < (int)methods_.size()))
  {
    Array<Parameter *,constants::METHOD_PARAMETER_COUNT_MAX> * parameter_ptrs_ptr = NULL;
    parameter_ptrs_ptr = &methods_[method_index].parameter_ptrs_;
    for (size_t i=0; i<parameter_ptrs_ptr->size(); ++i)
    {
      if ((*parameter_ptrs_ptr)[i]->compareName(parameter_name))
      {
        parameter_index = i;
        break;
      }
    }
  }
  return parameter_index;
}

template <typename T>
int Server::findMethodIndex(T const & method_name)
{
  int method_index = -1;
  for (size_t i=0; i<methods_.size(); ++i)
  {
    if (methods_[i].compareName(method_name))
    {
      method_index = i;
      break;
    }
  }
  return method_index;
}

template <typename T>
int Server::findCallbackIndex(T const & callback_name)
{
  int callback_index = -1;
  for (size_t i=0; i<callbacks_.size(); ++i)
  {
    if (callbacks_[i].compareName(callback_name))
    {
      callback_index = i;
      break;
    }
  }
  return callback_index;
}

}

#endif
