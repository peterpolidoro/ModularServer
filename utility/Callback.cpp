// ----------------------------------------------------------------------------
// Callback.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Callback.h"


namespace modular_server
{
// public
Callback::Callback()
{
  setup(constants::empty_constant_string);
}

void Callback::attachFunctor(const Functor0 & functor)
{
  functor_ = functor;
}

void Callback::addField(Field & field)
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
Callback::Callback(const ConstantString & name)
{
  setup(name);
}

void Callback::setup(const ConstantString & name)
{
  setName(name);
}

int Callback::findFieldIndex(const ConstantString & field_name)
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

size_t Callback::getFieldCount()
{
  return field_ptrs_.size();
}

void Callback::functor()
{
  if (functor_)
  {
    functor_();
  }
}

}
