// ----------------------------------------------------------------------------
// Interrupt.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Interrupt.h"


namespace modular_server
{
// public
Interrupt::Interrupt()
{
  setup(constants::empty_constant_string);
}

void Interrupt::attachCallback(const Functor0 & callback)
{
  callback_ = callback;
}

void Interrupt::addField(Field & field)
{
  const ConstantString & field_name = field.getName();
  int field_index = findFieldIndex(field_name);
  if (field_index < 0)
  {
    field_ptrs_.push_back(&field);
  }
}

// protected

// private
Interrupt::Interrupt(const ConstantString & name)
{
  setup(name);
}

void Interrupt::setup(const ConstantString & name)
{
  setName(name);
}

int Interrupt::findFieldIndex(const ConstantString & field_name)
{
  int field_index = -1;
  for (size_t i=0; i<field_ptrs_.size(); ++i)
  {
    if (field_ptrs_[i]->compareName(field_name))
    {
      field_index = i;
      break;
    }
  }
  return field_index;
}

size_t Interrupt::getFieldCount()
{
  return field_ptrs_.size();
}

void Interrupt::callback()
{
  if (callback_)
  {
    callback_();
  }
}

}
