// ----------------------------------------------------------------------------
// ModularServerDefinitions.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef MODULAR_SERVER_DEFINITIONS_H
#define MODULAR_SERVER_DEFINITIONS_H


namespace ModularDevice
{
template <size_t MAX_SIZE>
void ModularServer::setMethodStorage(Method (&methods)[MAX_SIZE])
{
  server_.setMethodStorage(methods);
}

template <size_t MAX_SIZE>
void ModularServer::setParameterStorage(Parameter (&parameters)[MAX_SIZE])
{
  server_.setParameterStorage(parameters);
}

template <size_t MAX_SIZE>
void ModularServer::setFieldStorage(Field (&fields)[MAX_SIZE])
{
  server_.setFieldStorage(fields);
}

template <typename T>
Field& ModularServer::createField(const ConstantString &field_name,
                                  const T &default_value)
{
  return server_.createField(field_name,default_value);
}

template <typename T>
Field& ModularServer::createField(const ConstantString &field_name,
                                  const T default_value[],
                                  const unsigned int array_length)
{
  return server_.createField(field_name,default_value,array_length);
}

template <typename T>
void ModularServer::setFieldValue(const ConstantString &field_name,
                                  const T &value)
{
  server_.setFieldValue(field_name,value);
}

template <typename T>
void ModularServer::setFieldElementValue(const ConstantString &field_name,
                                         const T &value,
                                         const unsigned int element_index)
{
  server_.setFieldElementValue(field_name,value,element_index);
}

template <typename T>
void ModularServer::getFieldValue(const ConstantString &field_name,
                                  T &value)
{
  server_.getFieldValue(field_name,value);
}

template <typename T>
void ModularServer::getFieldElementValue(const ConstantString &field_name,
                                         T &value,
                                         const unsigned int element_index)
{
  server_.getFieldElementValue(field_name,value,element_index);
}

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
void ModularServer::writeToResponse(T (&values)[N])
{
  server_.writeToResponse(values);
}

template <typename K, typename T>
void ModularServer::writeToResponse(K key, T value)
{
  server_.writeToResponse(key,value);
}

template <typename K, typename T, size_t N>
void ModularServer::writeToResponse(K key, T (&values)[N])
{
  server_.writeToResponse(key,values);
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

}

#endif
