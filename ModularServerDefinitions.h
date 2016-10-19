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
template <size_t FIELDS_MAX_SIZE,
          size_t PARAMETERS_MAX_SIZE,
          size_t METHODS_MAX_SIZE,
          size_t INTERRUPTS_MAX_SIZE>
void ModularServer::addFirmware(const FirmwareInfo & firmware_info,
                                Field (&fields)[FIELDS_MAX_SIZE],
                                Parameter (&parameters)[PARAMETERS_MAX_SIZE],
                                Method (&methods)[METHODS_MAX_SIZE],
                                Interrupt (&interrupts)[INTERRUPTS_MAX_SIZE])
{
  server_.addFirmware(firmware_info,
                      fields,
                      parameters,
                      methods,
                      interrupts);
}

// Fields
template <typename T>
Field & ModularServer::createField(const ConstantString & field_name,
                                   const T & default_value)
{
  return server_.createField(field_name,default_value);
}

template <typename T, size_t N>
Field & ModularServer::createField(const ConstantString & field_name,
                                   const T (&default_value)[N])
{
  return server_.createField(field_name,default_value);
}

// Parameters

// Methods

// Interrupts

// Response

// Server

}

#endif
