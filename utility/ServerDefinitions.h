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
// public

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

// Parameters

// Methods

// Response

// private
template <typename T>
int Server::findFieldIndex(T const & field_name)
{
  int field_index = -1;
  for (size_t i=0; i<fields_.size(); ++i)
  {
    if (fields_[i].parameter().compareName(field_name))
    {
      field_index = i;
      break;
    }
  }
  return field_index;
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

}

#endif
