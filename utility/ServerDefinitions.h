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
template<size_t MAX_SIZE>
void Server::setMethodStorage(Method (&methods)[MAX_SIZE])
{
  external_methods_.setStorage(methods);
}

template<size_t MAX_SIZE>
void Server::setParameterStorage(Parameter (&parameters)[MAX_SIZE])
{
  external_parameters_.setStorage(parameters);
}

template<size_t MAX_SIZE>
void Server::setSavedVariableStorage(SavedVariable (&saved_variables)[MAX_SIZE])
{
  external_saved_variables_.setStorage(saved_variables);
}

template<typename T>
SavedVariable& Server::createInternalSavedVariable(const ConstantString &saved_variable_name,
                                                   const T &default_value)
{
  int saved_variable_index = findSavedVariableIndex(saved_variable_name);
  if (saved_variable_index < 0)
  {
    internal_saved_variables_.push_back(SavedVariable(saved_variable_name,
                                                      eeprom_index_,
                                                      default_value));
    unsigned char eeprom_init_value = 0;
    getSavedVariableValue(*eeprom_init_name_ptr_,eeprom_init_value);
    if (eeprom_init_value != constants::eeprom_initialized_value)
    {
      internal_saved_variables_.back().setDefaultValue();
    }
    eeprom_index_ += internal_saved_variables_.back().getSize();
    return internal_saved_variables_.back();
  }
}

template<typename T>
SavedVariable& Server::createSavedVariable(const ConstantString &saved_variable_name,
                                           const T &default_value)
{
  int saved_variable_index = findSavedVariableIndex(saved_variable_name);
  if (saved_variable_index < 0)
  {
    external_saved_variables_.push_back(SavedVariable(saved_variable_name,
                                                      eeprom_index_,
                                                      default_value));
    unsigned char eeprom_init_value = 0;
    getSavedVariableValue(*eeprom_init_name_ptr_,eeprom_init_value);
    if (eeprom_init_value != constants::eeprom_initialized_value)
    {
      external_saved_variables_.back().setDefaultValue();
    }
    eeprom_index_ += external_saved_variables_.back().getSize();
    return external_saved_variables_.back();
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
    external_saved_variables_.push_back(SavedVariable(saved_variable_name,
                                                      eeprom_index_,
                                                      default_value,
                                                      array_length));
    unsigned char eeprom_init_value;
    getSavedVariableValue(*eeprom_init_name_ptr_,eeprom_init_value);
    if (eeprom_init_value != constants::eeprom_initialized_value)
    {
      external_saved_variables_.back().setDefaultValue();
    }
    eeprom_index_ += external_saved_variables_.back().getSize();
    return external_saved_variables_.back();
  }
}

template<typename T>
void Server::setSavedVariableValue(const ConstantString &saved_variable_name,
                                   const T &value)
{
  int saved_variable_index = findSavedVariableIndex(saved_variable_name);
  if (saved_variable_index >= 0)
  {
    if (saved_variable_index < internal_saved_variables_.max_size())
    {
      internal_saved_variables_[saved_variable_index].setValue(value);
    }
    else
    {
      saved_variable_index -= internal_saved_variables_.max_size();
      external_saved_variables_[saved_variable_index].setValue(value);
    }
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
    if (saved_variable_index < internal_saved_variables_.max_size())
    {
      internal_saved_variables_[saved_variable_index].setValue(value,array_index);
    }
    else
    {
      saved_variable_index -= internal_saved_variables_.max_size();
      external_saved_variables_[saved_variable_index].setValue(value,array_index);
    }
  }
}

template<typename T>
void Server::getSavedVariableValue(const ConstantString &saved_variable_name,
                                   T &value)
{
  int saved_variable_index = findSavedVariableIndex(saved_variable_name);
  if (saved_variable_index >= 0)
  {
    if (saved_variable_index < internal_saved_variables_.max_size())
    {
      internal_saved_variables_[saved_variable_index].getValue(value);
    }
    else
    {
      saved_variable_index -= internal_saved_variables_.max_size();
      external_saved_variables_[saved_variable_index].getValue(value);
    }
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
    if (saved_variable_index < internal_saved_variables_.max_size())
    {
      internal_saved_variables_[saved_variable_index].getValue(value,array_index);
    }
    else
    {
      saved_variable_index -= internal_saved_variables_.max_size();
      external_saved_variables_[saved_variable_index].getValue(value,array_index);
    }
  }
}

template<typename K>
void Server::writeKeyToResponse(K key)
{
  json_stream_.writeKey(key);
}

template<typename T>
void Server::writeToResponse(T value)
{
  json_stream_.write(value);
}

template<typename K, typename T>
void Server::writeToResponse(K key, T value)
{
  json_stream_.write(key,value);
}

template<typename K>
void Server::writeNullToResponse(K key)
{
  json_stream_.writeNull(key);
}

template<typename T>
void Server::sendErrorResponse(T error)
{
  error_ = true;
  writeKeyToResponse(constants::error_constant_string);
  beginResponseObject();
  writeToResponse(constants::message_constant_string,constants::server_error_error_message);
  writeToResponse(constants::data_constant_string,error);
  writeToResponse(constants::code_constant_string,constants::server_error_error_code);
  endResponseObject();
}

template<typename T>
void Server::writeResultToResponse(T value)
{
  json_stream_.write(constants::result_constant_string,value);
  result_in_response_ = true;
}

template<typename T>
int Server::findMethodIndex(T const&method_name)
{
  int method_index = -1;
  for (unsigned int i=0; i<internal_methods_.size(); ++i)
  {
    if (internal_methods_[i].compareName(method_name))
    {
      method_index = i;
      return method_index;
    }
  }
  for (unsigned int i=0; i<external_methods_.size(); ++i)
  {
    if (external_methods_[i].compareName(method_name))
    {
      method_index = i + internal_methods_.max_size();
      return method_index;
    }
  }
  return method_index;
}

template<typename T>
int Server::findParameterIndex(T const&parameter_name)
{
  int parameter_index = -1;
  for (unsigned int i=0; i<internal_parameters_.size(); ++i)
  {
    if (internal_parameters_[i].compareName(parameter_name))
    {
      parameter_index = i;
      return parameter_index;
    }
  }
  for (unsigned int i=0; i<external_parameters_.size(); ++i)
  {
    if (external_parameters_[i].compareName(parameter_name))
    {
      parameter_index = i + internal_parameters_.max_size();
      return parameter_index;
    }
  }
  return parameter_index;
}

template<typename T>
int Server::findMethodParameterIndex(int method_index, T const&parameter_name)
{
  int parameter_index = -1;
  if (method_index >= 0)
  {
    Array<Parameter*,constants::METHOD_PARAMETER_COUNT_MAX>* parameter_ptrs_ptr = NULL;
    if (method_index < internal_methods_.max_size())
    {
      parameter_ptrs_ptr = &internal_methods_[method_index].parameter_ptrs_;
    }
    else
    {
      int index = method_index - internal_methods_.max_size();
      parameter_ptrs_ptr = &external_methods_[index].parameter_ptrs_;
    }
    for (unsigned int i=0; i<parameter_ptrs_ptr->size(); ++i)
    {
      if ((*parameter_ptrs_ptr)[i]->compareName(parameter_name))
      {
        parameter_index = i;
        return parameter_index;
      }
    }
  }
  return parameter_index;
}
}

#endif
