// ----------------------------------------------------------------------------
// ServerDefinitions.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_DEVICE_SERVER_DEFINITIONS_H_
#define _MODULAR_DEVICE_SERVER_DEFINITIONS_H_


namespace ModularDevice
{
template<typename T>
SavedVariable& Server::createSavedVariable(const ConstantString &saved_variable_name,
                                           const T &default_value)
{
  int saved_variable_index = findSavedVariableIndex(saved_variable_name);
  if (saved_variable_index < 0)
  {
    saved_variable_array_.push_back(SavedVariable(saved_variable_name,
                                                  eeprom_index_,
                                                  default_value));
    unsigned char eeprom_init_value = 0;
    getSavedVariableValue(*eeprom_init_name_ptr_,eeprom_init_value);
    if (eeprom_init_value != constants::eeprom_initialized_value)
    {
      saved_variable_array_.back().setDefaultValue();
    }
    eeprom_index_ += saved_variable_array_.back().getSize();
    return saved_variable_array_.back();
  }
}

template<typename T>
SavedVariable& Server::createSavedVariable(const ConstantString &saved_variable_name,
                                           const T default_value[],
                                           const unsigned int array_length)
{
  int saved_variable_index = findSavedVariableIndex(saved_variable_name);
  if (saved_variable_index < 0)
  {
    saved_variable_array_.push_back(SavedVariable(saved_variable_name,
                                                  eeprom_index_,
                                                  default_value,
                                                  array_length));
    unsigned char eeprom_init_value;
    getSavedVariableValue(*eeprom_init_name_ptr_,eeprom_init_value);
    if (eeprom_init_value != constants::eeprom_initialized_value)
    {
      saved_variable_array_.back().setDefaultValue();
    }
    eeprom_index_ += saved_variable_array_.back().getSize();
    return saved_variable_array_.back();
  }
}

template<typename T>
void Server::setSavedVariableValue(const ConstantString &saved_variable_name,
                                   const T &value)
{
  int saved_variable_index = findSavedVariableIndex(saved_variable_name);
  if (saved_variable_index >= 0)
  {
    saved_variable_array_[saved_variable_index].setValue(value);
  }
}

template<typename T>
void Server::setSavedVariableValue(const ConstantString &saved_variable_name,
                                   const T value[],
                                   const unsigned int array_index)
{
  int saved_variable_index = findSavedVariableIndex(saved_variable_name);
  if (saved_variable_index >= 0)
  {
    saved_variable_array_[saved_variable_index].setValue(value,array_index);
  }
}

template<typename T>
void Server::getSavedVariableValue(const ConstantString &saved_variable_name,
                                   T &value)
{
  int saved_variable_index = findSavedVariableIndex(saved_variable_name);
  if (saved_variable_index >= 0)
  {
    saved_variable_array_[saved_variable_index].getValue(value);
  }
}

template<typename T>
void Server::getSavedVariableValue(const ConstantString &saved_variable_name,
                                   T value[],
                                   const unsigned int array_index)
{
  int saved_variable_index = findSavedVariableIndex(saved_variable_name);
  if (saved_variable_index >= 0)
  {
    saved_variable_array_[saved_variable_index].getValue(value,array_index);
  }
}

template<typename K>
void Server::addKeyToResponse(K key)
{
  json_printer_.addKey(key);
}

template<typename T>
void Server::addToResponse(T value)
{
  json_printer_.add(value);
}

template<typename K, typename T>
void Server::addToResponse(K key, T value)
{
  json_printer_.add(key,value);
}

template<typename K>
void Server::addNullToResponse(K key)
{
  json_printer_.addNull(key);
}

template<typename T>
void Server::sendErrorResponse(T error)
{
  addToResponse(constants::status_constant_string,JsonPrinter::ERROR);
  addToResponse(constants::error_message_constant_string,error);
  error_ = true;
}

template<typename T>
void Server::addResultToResponse(T value)
{
  json_printer_.add(constants::result_constant_string,value);
}
}

#endif
