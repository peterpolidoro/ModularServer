// ----------------------------------------------------------------------------
// ModularDeviceDefinitions.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef MODULAR_DEVICE_DEFINITIONS_H
#define MODULAR_DEVICE_DEFINITIONS_H


namespace ModularDevice
{
template<typename T>
SavedVariable& ModularDevice::createSavedVariable(const ConstantString &saved_variable_name,
                                                  const T &default_value)
{
  return server_.createSavedVariable(saved_variable_name,default_value);
}

template<typename T>
SavedVariable& ModularDevice::createSavedVariable(const ConstantString &saved_variable_name,
                                                  const T default_value[],
                                                  const unsigned int array_length)
{
  return server_.createSavedVariable(saved_variable_name,default_value,array_length);
}

template<typename T>
void ModularDevice::setSavedVariableValue(const ConstantString &saved_variable_name,
                                          const T &value)
{
  server_.setSavedVariableValue(saved_variable_name,value);
}

template<typename T>
void ModularDevice::setSavedVariableValue(const ConstantString &saved_variable_name,
                                          const T value[],
                                          const unsigned int array_index)
{
  server_.setSavedVariableValue(saved_variable_name,value,array_index);
}

template<typename T>
void ModularDevice::getSavedVariableValue(const ConstantString &saved_variable_name,
                                          T &value)
{
  server_.getSavedVariableValue(saved_variable_name,value);
}

template<typename T>
void ModularDevice::getSavedVariableValue(const ConstantString &saved_variable_name,
                                          T value[],
                                          const unsigned int array_index)
{
  server_.getSavedVariableValue(saved_variable_name,value,array_index);
}

template<typename K>
void ModularDevice::addKeyToResponse(K key)
{
  server_.addKeyToResponse(key);
}

template<typename T>
void ModularDevice::addToResponse(T value)
{
  server_.addToResponse(value);
}

template<typename K, typename T>
void ModularDevice::addToResponse(K key, T value)
{
  server_.addToResponse(key,value);
}

template<typename K>
void ModularDevice::addNullToResponse(K key)
{
  server_.addNullToResponse(key);
}

template<typename T>
void ModularDevice::sendErrorResponse(T error)
{
  server_.sendErrorResponse(error);
}

template<typename T>
void ModularDevice::addResultToResponse(T value)
{
  server_.addResultToResponse(value);
}

}

#endif
