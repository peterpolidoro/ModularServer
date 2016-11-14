// ----------------------------------------------------------------------------
// ModularServerDefinitions.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef MODULAR_SERVER_DEFINITIONS_H
#define MODULAR_SERVER_DEFINITIONS_H


namespace modular_server
{
// Streams

// Device ID

// Hardware Info

// Firmware
template <size_t PROPERTIES_MAX_SIZE,
          size_t PARAMETERS_MAX_SIZE,
          size_t METHODS_MAX_SIZE,
          size_t CALLBACKS_MAX_SIZE>
void ModularServer::addFirmware(const FirmwareInfo & firmware_info,
                                Property (&properties)[PROPERTIES_MAX_SIZE],
                                Parameter (&parameters)[PARAMETERS_MAX_SIZE],
                                Method (&methods)[METHODS_MAX_SIZE],
                                Callback (&callbacks)[CALLBACKS_MAX_SIZE])
{
  server_.addFirmware(firmware_info,
                      properties,
                      parameters,
                      methods,
                      callbacks);
}

// Properties
template <typename T>
Property & ModularServer::createProperty(const ConstantString & property_name,
                                   const T & default_value)
{
  return server_.createProperty(property_name,default_value);
}

template <typename T, size_t N>
Property & ModularServer::createProperty(const ConstantString & property_name,
                                   const T (&default_value)[N])
{
  return server_.createProperty(property_name,default_value);
}

// Parameters

// Methods

// Callbacks

// Response

// Server

}

#endif
