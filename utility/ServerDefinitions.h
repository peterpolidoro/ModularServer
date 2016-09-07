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
template <size_t MAX_SIZE>
void Server::setFieldStorage(Field (&fields)[MAX_SIZE])
{
  external_fields_.setStorage(fields);
}

template <size_t MAX_SIZE>
void Server::setParameterStorage(Parameter (&parameters)[MAX_SIZE])
{
  external_parameters_.setStorage(parameters);
}

template <size_t MAX_SIZE>
void Server::setMethodStorage(Method (&methods)[MAX_SIZE])
{
  external_methods_.setStorage(methods);
}

template <typename T>
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

template <typename T>
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

template <typename T, size_t N>
Field& Server::createField(const ConstantString &field_name,
                           const T (&default_value)[N])
{
  int field_index = findFieldIndex(field_name);
  if (field_index < 0)
  {
    external_fields_.push_back(Field(field_name,
                                     default_value));
    return external_fields_.back();
  }
}

template <typename T>
bool Server::setFieldValue(const ConstantString &field_name,
                           const T &value)
{
  int field_index;
  Field& field = findField(field_name,&field_index);
  if (field_index >= 0)
  {
    bool success = field.setValue(value);
    if (success)
    {
      field.setValueCallback();
    }
    return success;
  }
  else
  {
    return false;
  }
}

template <typename T, size_t N>
bool Server::setFieldValue(const ConstantString &field_name,
                           const T (&value)[N])
{
  int field_index;
  Field& field = findField(field_name,&field_index);
  if (field_index >= 0)
  {
    bool success = field.setValue(value);
    if (success)
    {
      field.setValueCallback();
    }
    return success;
  }
  else
  {
    return false;
  }
}

template <typename T>
bool Server::setFieldValue(const ConstantString &field_name,
                           const T *value,
                           const size_t N)
{
  int field_index;
  Field& field = findField(field_name,&field_index);
  if (field_index >= 0)
  {
    bool success;
    size_t array_length = field.getArrayLength();
    size_t array_length_min = min(array_length,N);
    for (size_t i=0;i<array_length_min;++i)
    {
      T v = value[i];
      success = setFieldElementValue(field_name,i,v);
      if (!success)
      {
        // terminate string
        if (field.getParameter().getType() == JsonStream::STRING_TYPE)
        {
          setFieldElementValue(field_name,i,'\0');
        }
        return false;
      }
    }
    // terminate string
    if (field.getParameter().getType() == JsonStream::STRING_TYPE)
    {
      setFieldElementValue(field_name,array_length_min-1,'\0');
    }
  }
  else
  {
    return false;
  }
  field.setValueCallback();
  return true;
}

template <typename T>
bool Server::setFieldElementValue(const ConstantString &field_name,
                                  const size_t element_index,
                                  const T &value)
{
  int field_index;
  Field& field = findField(field_name,&field_index);
  if (field_index >= 0)
  {
    bool success = field.setElementValue(value,element_index);
    if (success)
    {
      field.setElementValueCallback(element_index);
    }
    return success;
  }
  else
  {
    return false;
  }
}

template <typename T>
bool Server::setAllFieldElementValues(const ConstantString &field_name,
                                      const T &value)
{
  int field_index;
  Field& field = findField(field_name,&field_index);
  if (field_index >= 0)
  {
    bool success;
    size_t array_length = field.getArrayLength();
    for (size_t i=0;i<array_length;++i)
    {
      success = setFieldElementValue(field_name,i,value);
      if (!success)
      {
        // terminate string
        if (field.getParameter().getType() == JsonStream::STRING_TYPE)
        {
          setFieldElementValue(field_name,i,'\0');
        }
        return false;
      }
    }
    // terminate string
    if (field.getParameter().getType() == JsonStream::STRING_TYPE)
    {
      setFieldElementValue(field_name,array_length-1,'\0');
    }
  }
  else
  {
    return false;
  }
  field.setValueCallback();
  return true;
}

template <typename T>
bool Server::getFieldValue(const ConstantString &field_name,
                           T &value)
{
  int field_index;
  Field& field = findField(field_name,&field_index);
  if (field_index >= 0)
  {
    return field.getValue(value);
  }
  else
  {
    return false;
  }
}

template <typename T, size_t N>
bool Server::getFieldValue(const ConstantString &field_name,
                           T (&value)[N])
{
  int field_index;
  Field& field = findField(field_name,&field_index);
  if (field_index >= 0)
  {
    return field.getValue(value);
  }
  else
  {
    return false;
  }
}

template <typename T>
bool Server::getFieldValue(const ConstantString &field_name,
                           T *value,
                           const size_t N)
{
  int field_index;
  Field& field = findField(field_name,&field_index);
  if (field_index >= 0)
  {
    size_t array_length = field.getArrayLength();
    size_t array_length_min = min(array_length,N);
    bool success;
    for (size_t i=0;i<array_length_min;++i)
    {
      T v;
      success = getFieldElementValue(field_name,i,v);
      if (!success)
      {
        // terminate string
        if (field.getParameter().getType() == JsonStream::STRING_TYPE)
        {
          value[i] = '\0';
        }
        return false;
      }
      value[i] = v;
    }
    // terminate string
    if (field.getParameter().getType() == JsonStream::STRING_TYPE)
    {
      value[array_length_min-1] = '\0';
    }
  }
  else
  {
    return false;
  }
  return true;
}

template <typename T>
bool Server::getFieldElementValue(const ConstantString &field_name,
                                  const size_t element_index,
                                  T &value)
{
  int field_index;
  Field& field = findField(field_name,&field_index);
  if (field_index >= 0)
  {
    return field.getElementValue(value,element_index);
  }
  else
  {
    return false;
  }
}

template <typename T>
bool Server::getFieldDefaultValue(const ConstantString &field_name,
                                  T &value)
{
  int field_index;
  Field& field = findField(field_name,&field_index);
  if (field_index >= 0)
  {
    return field.getDefaultValue(value);
  }
  else
  {
    return false;
  }
}

template <typename T, size_t N>
bool Server::getFieldDefaultValue(const ConstantString &field_name,
                                  T (&value)[N])
{
  int field_index;
  Field& field = findField(field_name,&field_index);
  if (field_index >= 0)
  {
    return field.getDefaultValue(value);
  }
  else
  {
    return false;
  }
}

template <typename T>
bool Server::getFieldDefaultValue(const ConstantString &field_name,
                                  T *value,
                                  const size_t N)
{
  int field_index;
  Field& field = findField(field_name,&field_index);
  if (field_index >= 0)
  {
    size_t array_length = field.getArrayLength();
    size_t array_length_min = min(array_length,N);
    bool success;
    for (size_t i=0;i<array_length_min;++i)
    {
      T v;
      success = getFieldDefaultElementValue(field_name,i,v);
      if (!success)
      {
        return false;
      }
      value[i] = v;
    }
  }
  else
  {
    return false;
  }
  return true;
}

template <typename T>
bool Server::getFieldDefaultElementValue(const ConstantString &field_name,
                                         const size_t element_index,
                                         T &value)
{
  int field_index;
  Field& field = findField(field_name,&field_index);
  if (field_index >= 0)
  {
    return field.getDefaultElementValue(value,element_index);
  }
  else
  {
    return false;
  }
}

template <typename K>
void Server::writeKeyToResponse(K key)
{
  json_stream_.writeKey(key);
}

template <typename T>
void Server::writeToResponse(T value)
{
  json_stream_.write(value);
}

template <typename T, size_t N>
void Server::writeToResponse(T (&value)[N])
{
  json_stream_.write(value);
}

template <typename K, typename T>
void Server::writeToResponse(K key, T value)
{
  json_stream_.write(key,value);
}

template <typename K, typename T, size_t N>
void Server::writeToResponse(K key, T (&value)[N])
{
  json_stream_.write(key,value);
}

template <typename T>
void Server::writeArrayToResponse(T *value, size_t N)
{
  json_stream_.writeArray(value,N);
}

template <typename K, typename T>
void Server::writeArrayToResponse(K key, T *value, size_t N)
{
  json_stream_.writeArray(key,value,N);
}

template <typename K>
void Server::writeNullToResponse(K key)
{
  json_stream_.writeNull(key);
}

template <typename T>
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

template <typename T>
void Server::writeResultToResponse(T value)
{
  // Prevent multiple results in one response
  if (!result_key_in_response_ && !error_)
  {
    result_key_in_response_ = true;
    json_stream_.write(constants::result_constant_string,value);
  }
}

template <typename T, size_t N>
void Server::writeResultToResponse(T (&value)[N])
{
  // Prevent multiple results in one response
  if (!result_key_in_response_ && !error_)
  {
    result_key_in_response_ = true;
    json_stream_.write(constants::result_constant_string,value);
  }
}

template <typename T>
void Server::writeResultToResponse(T *value, size_t N)
{
  // Prevent multiple results in one response
  if (!result_key_in_response_ && !error_)
  {
    result_key_in_response_ = true;
    json_stream_.write(constants::result_constant_string,value,N);
  }
}

template <typename T>
int Server::findMethodIndex(T const&method_name)
{
  int method_index = -1;
  for (size_t i=0; i<internal_methods_.size(); ++i)
  {
    if (internal_methods_[i].compareName(method_name))
    {
      method_index = i;
      return method_index;
    }
  }
  for (size_t i=0; i<external_methods_.size(); ++i)
  {
    if (external_methods_[i].compareName(method_name))
    {
      method_index = i + internal_methods_.max_size();
      return method_index;
    }
  }
  return method_index;
}

template <typename T>
int Server::findParameterIndex(T const&parameter_name)
{
  int parameter_index = -1;
  for (size_t i=0; i<internal_parameters_.size(); ++i)
  {
    if (internal_parameters_[i].compareName(parameter_name))
    {
      parameter_index = i;
      return parameter_index;
    }
  }
  for (size_t i=0; i<external_parameters_.size(); ++i)
  {
    if (external_parameters_[i].compareName(parameter_name))
    {
      parameter_index = i + internal_parameters_.max_size();
      return parameter_index;
    }
  }
  return parameter_index;
}

template <typename T>
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
    for (size_t i=0; i<parameter_ptrs_ptr->size(); ++i)
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

template <typename T>
int Server::findFieldIndex(T const&field_name)
{
  int field_index = -1;
  for (size_t i=0; i<internal_fields_.size(); ++i)
  {
    if (internal_fields_[i].getParameter().compareName(field_name))
    {
      field_index = i;
      return field_index;
    }
  }
  for (size_t i=0; i<external_fields_.size(); ++i)
  {
    if (external_fields_[i].getParameter().compareName(field_name))
    {
      field_index = i + internal_fields_.max_size();
      return field_index;
    }
  }
  return field_index;
}

template <typename T>
Field& Server::findField(T const&field_name, int *field_index_ptr)
{
  int field_index = findFieldIndex(field_name);
  *field_index_ptr = field_index;
  if ((field_index >= 0) && (field_index < (int)internal_fields_.max_size()))
  {
    return internal_fields_[field_index];
  }
  else if (field_index >= (int)internal_fields_.max_size())
  {
    field_index -=  internal_fields_.max_size();
    return external_fields_[field_index];
  }
  else
  {
    return internal_fields_[0];
  }
}

}

#endif
