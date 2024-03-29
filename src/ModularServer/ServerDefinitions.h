// ----------------------------------------------------------------------------
// ServerDefinitions.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_SERVER_DEFINITIONS_H_
#define _MODULAR_SERVER_SERVER_DEFINITIONS_H_


namespace modular_server
{
// public

// Streams

// Device ID

// Hardware
template <size_t PINS_MAX_SIZE>
void Server::addHardware(const constants::HardwareInfo & hardware_info,
  Pin (&pins)[PINS_MAX_SIZE])
{
  hardware_info_array_.push_back(&hardware_info);
  pins_.addArray(pins);
}

// Pins

// Firmware
template <size_t PROPERTIES_MAX_SIZE,
  size_t PARAMETERS_MAX_SIZE,
  size_t FUNCTIONS_MAX_SIZE,
  size_t CALLBACKS_MAX_SIZE>
void Server::addFirmware(const constants::FirmwareInfo & firmware_info,
  Property (&properties)[PROPERTIES_MAX_SIZE],
  Parameter (&parameters)[PARAMETERS_MAX_SIZE],
  Function (&functions)[FUNCTIONS_MAX_SIZE],
  Callback (&callbacks)[CALLBACKS_MAX_SIZE])
{
  firmware_info_array_.push_back(&firmware_info);
  constants::SubsetMemberType firmware_name;
  firmware_name.cs_ptr = firmware_info.name_ptr;
  firmware_name_array_.push_back(firmware_name);
  Parameter & firmware_parameter = parameter(constants::firmware_constant_string);
  firmware_parameter.addValueToSubset(firmware_name_array_.back());
  // array length ranges is 1 less than subset size because one value is ALL
  firmware_parameter.setArrayLengthRange(1,(firmware_parameter.getSubsetSize() - 1));
  properties_.addArray(properties);
  parameters_.addArray(parameters);
  functions_.addArray(functions);
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
  return properties_[0]; // bad reference
}

template <typename T,
  size_t N>
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
  return properties_[0]; // bad reference
}

template <typename T>
void Server::setPropertiesToDefaults(T & firmware_name_array)
{
  for (size_t i=0; i<properties_.size(); ++i)
  {
    Property & property = properties_[i];
    if (property.parameter().firmwareNameInArray(firmware_name_array))
    {
      property.setValueToDefault();
    }
  }
}

// Parameters

// Functions

// Callbacks

// Response

// private
template <typename T>
int Server::findPinIndex(T const & pin_name)
{
  int pin_index = -1;
  for (size_t i=0; i<pins_.size(); ++i)
  {
    if (pins_[i].compareName(pin_name))
    {
      pin_index = i;
      break;
    }
  }
  return pin_index;
}

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
int Server::findFunctionParameterIndex(Function & function,
  T const & parameter_name)
{
  int parameter_index = -1;
  Array<Parameter *,constants::FUNCTION_PARAMETER_COUNT_MAX> * parameter_ptrs_ptr = NULL;
  parameter_ptrs_ptr = &function.parameter_ptrs_;
  for (size_t i=0; i<parameter_ptrs_ptr->size(); ++i)
  {
    if ((*parameter_ptrs_ptr)[i]->compareName(parameter_name))
    {
      parameter_index = i;
      break;
    }
  }
  return parameter_index;
}

template <typename T>
int Server::findFunctionIndex(T const & function_name)
{
  int function_index = -1;
  for (size_t i=0; i<functions_.size(); ++i)
  {
    if (functions_[i].compareName(function_name))
    {
      function_index = i;
      break;
    }
  }
  return function_index;
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
