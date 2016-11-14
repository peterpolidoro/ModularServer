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

void Callback::addProperty(Property & property)
{
  const ConstantString & property_name = property.getName();
  int property_index = findPropertyIndex(property_name);
  if (property_index < 0)
  {
    property_ptrs_.push_back(&property);
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

int Callback::findPropertyIndex(const ConstantString & property_name)
{
  int property_index = -1;
  for (size_t i=0; i<property_ptrs_.size(); ++i)
  {
    if (property_ptrs_[i]->compareName(property_name))
    {
      property_index = i;
      break;
    }
  }
  return property_index;
}

size_t Callback::getPropertyCount()
{
  return property_ptrs_.size();
}

void Callback::functor()
{
  if (functor_)
  {
    functor_();
  }
}

}
