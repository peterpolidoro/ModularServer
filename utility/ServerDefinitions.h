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
void Server::setFieldStorage(Field (&fields)[MAX_SIZE])
{
  external_fields_.setStorage(fields);
}

template<size_t MAX_SIZE>
void Server::setParameterStorage(Parameter (&parameters)[MAX_SIZE])
{
  external_parameters_.setStorage(parameters);
}

template<size_t MAX_SIZE>
void Server::setMethodStorage(Method (&methods)[MAX_SIZE])
{
  external_methods_.setStorage(methods);
}

template<typename T>
Field& Server::createInternalField(const ConstantString &field_name,
                                   const T &default_value)
{
  int field_index = findFieldIndex(field_name);
  if (field_index < 0)
  {
    internal_fields_.push_back(Field(field_name,
                                     default_value));
  }
  return internal_fields_.back();
}

template<typename T>
Field& Server::createField(const ConstantString &field_name,
                           const T &default_value)
{
  int field_index = findFieldIndex(field_name);
  if (field_index < 0)
  {
    external_fields_.push_back(Field(field_name,
                                     default_value));
    return external_fields_.back();
  }
}

template<typename T>
Field& Server::createField(const ConstantString &field_name,
                           const T default_value[],
                           const unsigned int array_length)
{
  int field_index = findFieldIndex(field_name);
  if (field_index < 0)
  {
    external_fields_.push_back(Field(field_name,
                                     default_value,
                                     array_length));
    return external_fields_.back();
  }
}

template<typename T>
void Server::setFieldValue(const ConstantString &field_name,
                           const T &value)
{
  int field_index = findFieldIndex(field_name);
  if (field_index >= 0)
  {
    if (field_index < internal_fields_.max_size())
    {
      internal_fields_[field_index].setValue(value);
    }
    else
    {
      field_index -= internal_fields_.max_size();
      external_fields_[field_index].setValue(value);
    }
  }
}

template<typename T>
void Server::setFieldElementValue(const ConstantString &field_name,
                                  const T &value,
                                  const unsigned int element_index)
{
  int field_index = findFieldIndex(field_name);
  if (field_index >= 0)
  {
    if (field_index < internal_fields_.max_size())
    {
      internal_fields_[field_index].setElementValue(value,element_index);
    }
    else
    {
      field_index -= internal_fields_.max_size();
      external_fields_[field_index].setElementValue(value,element_index);
    }
  }
}

template<typename T>
void Server::getFieldValue(const ConstantString &field_name,
                           T &value)
{
  int field_index = findFieldIndex(field_name);
  if (field_index >= 0)
  {
    if (field_index < internal_fields_.max_size())
    {
      internal_fields_[field_index].getValue(value);
    }
    else
    {
      field_index -= internal_fields_.max_size();
      external_fields_[field_index].getValue(value);
    }
  }
}

template<typename T>
void Server::getFieldElementValue(const ConstantString &field_name,
                                  T &value,
                                  const unsigned int element_index)
{
  int field_index = findFieldIndex(field_name);
  if (field_index >= 0)
  {
    if (field_index < internal_fields_.max_size())
    {
      internal_fields_[field_index].getElementValue(value,element_index);
    }
    else
    {
      field_index -= internal_fields_.max_size();
      external_fields_[field_index].getElementValue(value,element_index);
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

template<typename T, size_t N>
void Server::writeToResponse(T (&values)[N])
{
  json_stream_.write(values);
}

template<typename K, typename T>
void Server::writeToResponse(K key, T value)
{
  json_stream_.write(key,value);
}

template<typename K, typename T, size_t N>
void Server::writeToResponse(K key, T (&values)[N])
{
  json_stream_.write(key,values);
}

template<typename K>
void Server::writeNullToResponse(K key)
{
  json_stream_.writeNull(key);
}

template<typename T>
void Server::sendErrorResponse(T error)
{
  // Prevent multiple errors in one response
  if (!error_)
  {
    error_ = true;
    writeKeyToResponse(constants::error_constant_string);
    beginResponseObject();
    writeToResponse(constants::message_constant_string,constants::server_error_error_message);
    writeToResponse(constants::data_constant_string,error);
    writeToResponse(constants::code_constant_string,constants::server_error_error_code);
    endResponseObject();
  }
}

template<typename T>
void Server::writeResultToResponse(T value)
{
  // Prevent multiple results in one response
  if (!result_key_in_response_ && !error_)
  {
    result_key_in_response_ = true;
    json_stream_.write(constants::result_constant_string,value);
  }
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
