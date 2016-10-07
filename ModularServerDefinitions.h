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

template <typename T>
bool ModularServer::setFieldValue(const ConstantString & field_name,
                                  const T & value)
{
  return server_.setFieldValue(field_name,value);
}

template <typename T>
bool ModularServer::setFieldValue(const ConstantString & field_name,
                                  const T * value,
                                  const size_t N)
{
  return server_.setFieldValue(field_name,value,N);
}

template <typename T>
bool ModularServer::setFieldElementValue(const ConstantString & field_name,
                                         const size_t element_index,
                                         const T & value)
{
  return server_.setFieldElementValue(field_name,element_index,value);
}

template <typename T>
bool ModularServer::getFieldValue(const ConstantString & field_name,
                                  T & value)
{
  return server_.getFieldValue(field_name,value);
}

template <typename T>
bool ModularServer::getFieldValue(const ConstantString & field_name,
                                  T * value,
                                  const size_t N)
{
  return server_.getFieldValue(field_name,value,N);
}

template <typename T>
bool ModularServer::getFieldElementValue(const ConstantString & field_name,
                                         const size_t element_index,
                                         T & value)
{
  return server_.getFieldElementValue(field_name,element_index,value);
}

template <typename T>
bool ModularServer::getFieldDefaultValue(const ConstantString & field_name,
                                         T & value)
{
  return server_.getFieldDefaultValue(field_name,value);
}

template <typename T>
bool ModularServer::getFieldDefaultValue(const ConstantString & field_name,
                                         T * value,
                                         const size_t N)
{
  return server_.getFieldDefaultValue(field_name,value,N);
}

template <typename T>
bool ModularServer::getFieldDefaultElementValue(const ConstantString & field_name,
                                                const size_t element_index,
                                                T & value)
{
  return server_.getFieldDefaultElementValue(field_name,element_index,value);
}

template <typename T>
Field & ModularServer::setFieldDefaultValue(const ConstantString & field_name,
                                            const T & default_value)
{
  return server_.setFieldDefaultValue(field_name,default_value);
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
