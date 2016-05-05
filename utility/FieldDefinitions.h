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
             const T &default_value) :
  Parameter(name),
  SavedVariable(default_value)
{
}

template<typename T>
Field::Field(const ConstantString &name,
             const T default_value[],
             const unsigned int array_length) :
  Parameter(name),
  SavedVariable(default_value,array_length)
{
}
}

#endif
