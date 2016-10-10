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
          size_t METHODS_MAX_SIZE>
void ModularServer::addFirmware(const FirmwareInfo & firmware_info,
                                Field (&fields)[FIELDS_MAX_SIZE],
                                Parameter (&parameters)[PARAMETERS_MAX_SIZE],
                                Method (&methods)[METHODS_MAX_SIZE])
{
  server_.addFirmware(firmware_info,
                      fields,
                      parameters,
                      methods);
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

// Response
template <typename K>
void ModularServer::writeKeyToResponse(K key)
{
  server_.writeKeyToResponse(key);
}

template <typename T>
void ModularServer::writeToResponse(T value)
{
  server_.writeToResponse(value);
}

template <typename T, size_t N>
void ModularServer::writeToResponse(T (&value)[N])
{
  server_.writeToResponse(value);
}

template <typename K, typename T>
void ModularServer::writeToResponse(K key, T value)
{
  server_.writeToResponse(key,value);
}

template <typename K, typename T, size_t N>
void ModularServer::writeToResponse(K key, T (&value)[N])
{
  server_.writeToResponse(key,value);
}

template <typename T>
void ModularServer::writeArrayToResponse(T * value, size_t N)
{
  server_.writeArrayToResponse(value);
}

template <typename K, typename T>
void ModularServer::writeArrayToResponse(K key, T * value, size_t N)
{
  server_.writeArrayToResponse(key,value,N);
}

template <typename K>
void ModularServer::writeNullToResponse(K key)
{
  server_.writeNullToResponse(key);
}

template <typename T>
void ModularServer::sendErrorResponse(T error)
{
  server_.sendErrorResponse(error);
}

template <typename T>
void ModularServer::writeResultToResponse(T value)
{
  server_.writeResultToResponse(value);
}

template <typename T, size_t N>
void ModularServer::writeResultToResponse(T (&value)[N])
{
  server_.writeResultToResponse(value);
}

template <typename T>
void ModularServer::writeResultToResponse(T * value, size_t N)
{
  server_.writeResultToResponse(value,N);
}
}

#endif
