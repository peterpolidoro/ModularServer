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
template<size_t MAX_SIZE>
void Server::setMethodStorageArray(Method (&methods)[MAX_SIZE])
{
  server_.setMethodStorageArray(methods);
}

template<size_t MAX_SIZE>
void Server::setParameterStorageArray(Parameter (&parameters)[MAX_SIZE])
{
  server_.setParameterStorageArray(parameters);
}

template<size_t MAX_SIZE>
void Server::setSavedVariableStorageArray(SavedVariable (&saved_variables)[MAX_SIZE])
{
  server_.setSavedVariableStorageArray(saved_variables);
}

template<typename T>
SavedVariable& ModularServer::createSavedVariable(const ConstantString &saved_variable_name,
                                                  const T &default_value)
{
  return server_.createSavedVariable(saved_variable_name,default_value);
}

template<typename T>
SavedVariable& ModularServer::createSavedVariable(const ConstantString &saved_variable_name,
                                                  const T default_value[],
                                                  const unsigned int array_length)
{
  return server_.createSavedVariable(saved_variable_name,default_value,array_length);
}

template<typename T>
void ModularServer::setSavedVariableValue(const ConstantString &saved_variable_name,
                                          const T &value)
{
  server_.setSavedVariableValue(saved_variable_name,value);
}

template<typename T>
void ModularServer::setSavedVariableValue(const ConstantString &saved_variable_name,
                                          const T value[],
                                          const unsigned int array_index)
{
  server_.setSavedVariableValue(saved_variable_name,value,array_index);
}

template<typename T>
void ModularServer::getSavedVariableValue(const ConstantString &saved_variable_name,
                                          T &value)
{
  server_.getSavedVariableValue(saved_variable_name,value);
}

template<typename T>
void ModularServer::getSavedVariableValue(const ConstantString &saved_variable_name,
                                          T value[],
                                          const unsigned int array_index)
{
  server_.getSavedVariableValue(saved_variable_name,value,array_index);
}

template<typename K>
void ModularServer::writeKeyToResponse(K key)
{
  server_.writeKeyToResponse(key);
}

template<typename T>
void ModularServer::writeToResponse(T value)
{
  server_.writeToResponse(value);
}

template<typename K, typename T>
void ModularServer::writeToResponse(K key, T value)
{
  server_.writeToResponse(key,value);
}

template<typename K>
void ModularServer::writeNullToResponse(K key)
{
  server_.writeNullToResponse(key);
}

template<typename T>
void ModularServer::sendErrorResponse(T error)
{
  server_.sendErrorResponse(error);
}

template<typename T>
void ModularServer::writeResultToResponse(T value)
{
  server_.writeResultToResponse(value);
}

}

#endif
