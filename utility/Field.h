// ----------------------------------------------------------------------------
// Field.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_DEVICE_FIELD_H_
#define _MODULAR_DEVICE_FIELD_H_
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "SavedVariable.h"
#include "Parameter.h"
#include "Constants.h"


namespace ModularDevice
{
class Field : public Parameter
{
public:
  Field();
  template<typename T>
  Field(const ConstantString &name,
        T &storage,
        const T &default_value);
  template<typename T>
  Field(const ConstantString &name,
        T &storage,
        const T &default_value[],
        const unsigned int array_length);
private:
  SavedVariable saved_variable_;
  const void *storage_ptr_;
  friend class Method;
  friend class Server;
};
}
#include "FieldDefinitions.h"

#endif
