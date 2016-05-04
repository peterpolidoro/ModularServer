// ----------------------------------------------------------------------------
// FieldDefinitions.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_DEVICE_FIELD_DEFINITIONS_H_
#define _MODULAR_DEVICE_FIELD_DEFINITIONS_H_


namespace ModularDevice
{
template<typename T>
Field::Field(const ConstantString &name,
             T &storage,
             const T &default_value) :
  Parameter(name),
  saved_variable_(default_value)
{
  storage_ptr_ = &storage;
}

template<typename T>
Field::Field(const ConstantString &name,
             T &storage,
             const T &default_value[],
             const unsigned int array_length) :
  Parameter(name),
  saved_variable_(default_value,array_length)
{
  storage_ptr_ = &storage;
}
}

#endif
