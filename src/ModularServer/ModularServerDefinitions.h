// ----------------------------------------------------------------------------
// ModularServerDefinitions.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef MODULAR_SERVER_DEFINITIONS_H
#define MODULAR_SERVER_DEFINITIONS_H


namespace modular_server
{
// Streams

// Device ID

// Hardware
template <size_t PINS_MAX_SIZE>
void ModularServer::addHardware(const HardwareInfo & hardware_info,
  Pin (&pins)[PINS_MAX_SIZE])
{
  server_.addHardware(hardware_info,
    pins);
}

// Pins

// Firmware
template <size_t PROPERTIES_MAX_SIZE,
  size_t PARAMETERS_MAX_SIZE,
  size_t FUNCTIONS_MAX_SIZE,
  size_t CALLBACKS_MAX_SIZE>
void ModularServer::addFirmware(const FirmwareInfo & firmware_info,
  Property (&properties)[PROPERTIES_MAX_SIZE],
  Parameter (&parameters)[PARAMETERS_MAX_SIZE],
  Function (&functions)[FUNCTIONS_MAX_SIZE],
  Callback (&callbacks)[CALLBACKS_MAX_SIZE])
{
  server_.addFirmware(firmware_info,
    properties,
    parameters,
    functions,
    callbacks);
}

// Properties
template <typename T>
Property & ModularServer::createProperty(const ConstantString & property_name,
  const T & default_value)
{
  return server_.createProperty(property_name,default_value);
}

template <typename T,
  size_t N>
Property & ModularServer::createProperty(const ConstantString & property_name,
  const T (&default_value)[N])
{
  return server_.createProperty(property_name,default_value);
}

template <typename T>
void ModularServer::setPropertiesToDefaults(T & firmware_name_array)
{
  server_.setPropertiesToDefaults(firmware_name_array);
}

// Parameters

// Functions

// Callbacks

// Response

// Server

}

#endif
