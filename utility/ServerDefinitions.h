// ----------------------------------------------------------------------------
// ServerDefinitions.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_SERVER_DEFINITIONS_H_
#define _MODULAR_SERVER_SERVER_DEFINITIONS_H_


namespace modular_server
{
// Streams

// Device ID

// Hardware Info

// Firmware
template <size_t FIELDS_MAX_SIZE,
          size_t PARAMETERS_MAX_SIZE,
          size_t METHODS_MAX_SIZE>
void Server::addFirmware(const constants::FirmwareInfo & firmware_info,
                         Field (&fields)[FIELDS_MAX_SIZE],
                         Parameter (&parameters)[PARAMETERS_MAX_SIZE],
                         Method (&methods)[METHODS_MAX_SIZE])
{
  firmware_info_array_.push_back(&firmware_info);
  fields_.addArray(fields);
  parameters_.addArray(parameters);
  methods_.addArray(methods);
}

// Fields
template <typename T>
Field & Server::createField(const ConstantString & field_name,
                           const T & default_value)
{
  int field_index = findFieldIndex(field_name);
  if (field_index < 0)
  {
    fields_.push_back(Field(field_name,
                            default_value));
    return fields_.back();
  }
}

template <typename T, size_t N>
Field & Server::createField(const ConstantString & field_name,
                           const T (&default_value)[N])
{
  int field_index = findFieldIndex(field_name);
  if (field_index < 0)
  {
    fields_.push_back(Field(field_name,
                            default_value));
    return fields_.back();
  }
}

// template <typename T>
// bool Server::setFieldValue(const ConstantString & field_name,
//                            const T & value)
// {
//   bool success = false;
//   int field_index = findFieldIndex(field_name);
//   if ((field_index >= 0) && (field_index < (int)fields_.size()))
//   {
//     Field & field = fields_[field_index];
//     success = field.setValue(value);
//   }
//   return success;
// }

// template <typename T, size_t N>
// bool Server::setFieldValue(const ConstantString & field_name,
//                            const T (&value)[N])
// {
//   bool success = false;
//   int field_index = findFieldIndex(field_name);
//   if ((field_index >= 0) && (field_index < (int)fields_.size()))
//   {
//     Field & field = fields_[field_index];
//     success = field.setValue(value);
//   }
//   return success;
// }

// template <typename T>
// bool Server::setFieldValue(const ConstantString & field_name,
//                            const T * value,
//                            const size_t N)
// {
//   bool success = false;
//   int field_index = findFieldIndex(field_name);
//   if ((field_index >= 0) && (field_index < (int)fields_.size()))
//   {
//     Field & field = fields_[field_index];
//     JsonStream::JsonTypes type = field.getType();
//     if ((type == JsonStream::STRING_TYPE) &&
//         !field.stringIsSavedAsCharArray())
//     {
//       int subset_value_index = field.findSubsetValueIndex((const char *)value);
//       if (subset_value_index >= 0)
//       {
//         Vector<const constants::SubsetMemberType> & subset = field.getSubset();
//         const ConstantString * const subset_value = subset[subset_value_index].cs_ptr;
//         success = setFieldValue(field_name,subset_value);
//       }
//       return success;
//     }
//     size_t array_length = field.getArrayLength();
//     size_t array_length_min = min(array_length,N);
//     field.preSetValueCallback();
//     for (size_t i=0;i<array_length_min;++i)
//     {
//       T v = value[i];
//       success = setFieldElementValue(field_name,i,v);
//       if (!success && (type == JsonStream::STRING_TYPE))
//       {
//         // terminate string
//         setFieldElementValue(field_name,i,'\0');
//         break;
//       }
//     }
//     if (success &&
//         (type == JsonStream::STRING_TYPE) &&
//         (array_length_min >= 1))
//     {
//       // terminate string just in case
//       setFieldElementValue(field_name,array_length_min-1,'\0');
//     }
//     field.postSetValueCallback();
//   }
//   return success;
// }

// template <typename T>
// bool Server::setFieldElementValue(const ConstantString & field_name,
//                                   const size_t element_index,
//                                   const T & value)
// {
//   bool success = false;
//   int field_index = findFieldIndex(field_name);
//   if ((field_index >= 0) && (field_index < (int)fields_.size()))
//   {
//     Field & field = fields_[field_index];
//     success = field.setElementValue(value,element_index);
//   }
//   return success;
// }

// template <typename T>
// bool Server::setAllFieldElementValues(const ConstantString & field_name,
//                                       const T & value)
// {
//   bool success = false;
//   int field_index = findFieldIndex(field_name);
//   if ((field_index >= 0) && (field_index < (int)fields_.size()))
//   {
//     Field & field = fields_[field_index];
//     JsonStream::JsonTypes type = field.getType();
//     if ((type == JsonStream::STRING_TYPE) &&
//         !field.stringIsSavedAsCharArray())
//     {
//       return success;
//     }
//     size_t array_length = field.getArrayLength();
//     field.preSetValueCallback();
//     for (size_t i=0;i<array_length;++i)
//     {
//       success = setFieldElementValue(field_name,i,value);
//       if (!success && (type == JsonStream::STRING_TYPE))
//       {
//         // terminate string
//         setFieldElementValue(field_name,i,'\0');
//         break;
//       }
//     }
//     if (success &&
//         (type == JsonStream::STRING_TYPE) &&
//         (array_length >= 1))
//     {
//       // terminate string just in case
//       setFieldElementValue(field_name,array_length-1,'\0');
//     }
//     field.postSetValueCallback();
//   }
//   return success;
// }

// template <typename T>
// bool Server::getFieldValue(const ConstantString & field_name,
//                            T & value)
// {
//   int field_index = findFieldIndex(field_name);
//   if ((field_index >= 0) && (field_index < (int)fields_.size()))
//   {
//     Field & field = fields_[field_index];
//     return field.getValue(value);
//   }
//   else
//   {
//     return false;
//   }
// }

// template <typename T, size_t N>
// bool Server::getFieldValue(const ConstantString & field_name,
//                            T (&value)[N])
// {
//   int field_index = findFieldIndex(field_name);
//   if ((field_index >= 0) && (field_index < (int)fields_.size()))
//   {
//     Field & field = fields_[field_index];
//     return field.getValue(value);
//   }
//   else
//   {
//     return false;
//   }
// }

// template <typename T>
// bool Server::getFieldValue(const ConstantString & field_name,
//                            T * value,
//                            const size_t N)
// {
//   int field_index = findFieldIndex(field_name);
//   if ((field_index >= 0) && (field_index < (int)fields_.size()))
//   {
//     Field & field = fields_[field_index];
//     JsonStream::JsonTypes type = field.getType();
//     size_t array_length = field.getArrayLength();
//     size_t array_length_min = min(array_length,N);
//     bool success;
//     for (size_t i=0;i<array_length_min;++i)
//     {
//       T v;
//       success = getFieldElementValue(field_name,i,v);
//       if (!success)
//       {
//         // terminate string
//         if (type == JsonStream::STRING_TYPE)
//         {
//           value[i] = '\0';
//         }
//         return false;
//       }
//       value[i] = v;
//     }
//     // terminate string
//     if ((type == JsonStream::STRING_TYPE) &&
//         (array_length_min >= 1))
//     {
//       value[array_length_min-1] = '\0';
//     }
//   }
//   else
//   {
//     return false;
//   }
//   return true;
// }

// template <typename T>
// bool Server::getFieldElementValue(const ConstantString & field_name,
//                                   const size_t element_index,
//                                   T & value)
// {
//   int field_index = findFieldIndex(field_name);
//   if ((field_index >= 0) && (field_index < (int)fields_.size()))
//   {
//     Field & field = fields_[field_index];
//     return field.getElementValue(value,element_index);
//   }
//   else
//   {
//     return false;
//   }
// }

// template <typename T>
// bool Server::getFieldDefaultValue(const ConstantString & field_name,
//                                   T & value)
// {
//   int field_index = findFieldIndex(field_name);
//   if ((field_index >= 0) && (field_index < (int)fields_.size()))
//   {
//     Field & field = fields_[field_index];
//     return field.getDefaultValue(value);
//   }
//   else
//   {
//     return false;
//   }
// }

// template <typename T, size_t N>
// bool Server::getFieldDefaultValue(const ConstantString & field_name,
//                                   T (&value)[N])
// {
//   int field_index = findFieldIndex(field_name);
//   if ((field_index >= 0) && (field_index < (int)fields_.size()))
//   {
//     Field & field = fields_[field_index];
//     return field.getDefaultValue(value);
//   }
//   else
//   {
//     return false;
//   }
// }

// template <typename T>
// bool Server::getFieldDefaultValue(const ConstantString & field_name,
//                                   T * value,
//                                   const size_t N)
// {
//   int field_index = findFieldIndex(field_name);
//   if ((field_index >= 0) && (field_index < (int)fields_.size()))
//   {
//     Field & field = fields_[field_index];
//     size_t array_length = field.getArrayLength();
//     size_t array_length_min = min(array_length,N);
//     bool success;
//     for (size_t i=0;i<array_length_min;++i)
//     {
//       T v;
//       success = getFieldDefaultElementValue(field_name,i,v);
//       if (!success)
//       {
//         return false;
//       }
//       value[i] = v;
//     }
//   }
//   else
//   {
//     return false;
//   }
//   return true;
// }

// template <typename T>
// bool Server::getFieldDefaultElementValue(const ConstantString & field_name,
//                                          const size_t element_index,
//                                          T & value)
// {
//   int field_index = findFieldIndex(field_name);
//   if ((field_index >= 0) && (field_index < (int)fields_.size()))
//   {
//     Field & field = fields_[field_index];
//     return field.getDefaultElementValue(value,element_index);
//   }
//   else
//   {
//     return false;
//   }
// }

// template <typename T>
// Field & Server::setFieldDefaultValue(const ConstantString & field_name,
//                                      const T & default_value)
// {
//   int field_index = findFieldIndex(field_name);
//   if ((field_index >= 0) && (field_index < (int)fields_.size()))
//   {
//     Field & field = fields_[field_index];
//     return field.setDefaultValue(default_value);
//   }
//   else
//   {
//     return false;
//   }
// }

// Parameters

// Methods

// Response
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
void Server::writeArrayToResponse(T * value, size_t N)
{
  json_stream_.writeArray(value,N);
}

template <typename K, typename T>
void Server::writeArrayToResponse(K key, T * value, size_t N)
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
void Server::writeResultToResponse(T * value, size_t N)
{
  // Prevent multiple results in one response
  if (!result_key_in_response_ && !error_)
  {
    result_key_in_response_ = true;
    json_stream_.writeArray(constants::result_constant_string,value,N);
  }
}

template <typename T>
int Server::findFieldIndex(T const & field_name)
{
  int field_index = -1;
  for (size_t i=0; i<fields_.size(); ++i)
  {
    if (fields_[i].getParameter().compareName(field_name))
    {
      field_index = i;
      break;
    }
  }
  return field_index;
}

template <typename T>
int Server::findMethodIndex(T const & method_name)
{
  int method_index = -1;
  for (size_t i=0; i<methods_.size(); ++i)
  {
    if (methods_[i].compareName(method_name))
    {
      method_index = i;
      break;
    }
  }
  return method_index;
}

template <typename T>
int Server::findParameterIndex(T const & parameter_name)
{
  int parameter_index = -1;
  for (size_t i=0; i<parameters_.size(); ++i)
  {
    if (parameters_[i].compareName(parameter_name))
    {
      parameter_index = i;
      break;
    }
  }
  return parameter_index;
}

template <typename T>
int Server::findMethodParameterIndex(int method_index, T const & parameter_name)
{
  int parameter_index = -1;
  if ((method_index >= 0) && (method_index < (int)methods_.size()))
  {
    Array<Parameter *,constants::METHOD_PARAMETER_COUNT_MAX> * parameter_ptrs_ptr = NULL;
    parameter_ptrs_ptr = &methods_[method_index].parameter_ptrs_;
    for (size_t i=0; i<parameter_ptrs_ptr->size(); ++i)
    {
      if ((*parameter_ptrs_ptr)[i]->compareName(parameter_name))
      {
        parameter_index = i;
        break;
      }
    }
  }
  return parameter_index;
}

}

#endif
