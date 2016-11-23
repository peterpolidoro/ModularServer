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

namespace callback
{
// Parameters
constants::SubsetMemberType mode_ptr_subset[MODE_SUBSET_LENGTH] =
  {
    {.cs_ptr=&interrupt::mode_low},
    {.cs_ptr=&interrupt::mode_change},
    {.cs_ptr=&interrupt::mode_rising},
    {.cs_ptr=&interrupt::mode_falling},
  };

// Functions
CONSTANT_STRING(attach_to_function_name,"attachTo");
CONSTANT_STRING(detach_from_function_name,"detachFrom");
CONSTANT_STRING(detach_from_all_function_name,"detachFromAll");
}

// public
Callback::Callback()
{
  setup(constants::empty_constant_string);
}

void Callback::attachFunctor(const Functor0 & functor)
{
  FunctorCallbacks::remove(isr_);
  isr_ = FunctorCallbacks::add(functor);
  for (size_t i=0; i<interrupt_ptrs_.max_size(); ++i)
  {
    if (interrupt_ptrs_.indexHasValue(i))
    {
      interrupt_ptrs_[i]->reattach();
    }
  }
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

FunctorCallbacks::Callback Callback::getIsr()
{
  return isr_;
}

void Callback::attachTo(const Interrupt & interrupt, const ConstantString & mode)
{
  if ((&mode == &interrupt::mode_low) ||
      (&mode == &interrupt::mode_change) ||
      (&mode == &interrupt::mode_rising) ||
      (&mode == &interrupt::mode_falling))
  {
    int index = interrupt_ptrs_.add(&interrupt);
    if (index >= 0)
    {
      interrupt.attach(*this,mode);
    }
  }
}

void Callback::detachFrom(const Interrupt & interrupt)
{
  int interrupt_ptr_index = findInterruptPtrIndex(interrupt);
  if (interrupt_ptr_index >= 0)
  {
    interrupt.removeCallback();
    interrupt_ptrs_.remove(interrupt_ptr_index);
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
  isr_ = NULL;
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

int Callback::findInterruptPtrIndex(const Interrupt & interrupt)
{
  int interrupt_ptr_index = -1;
  for (size_t i=0; i<interrupt_ptrs_.max_size(); ++i)
  {
    if (interrupt_ptrs_.indexHasValue(i))
    {
      if (interrupts_ptrs_[i] == &interrupt)
      {
        interrupt_ptr_index = i;
        break;
      }
    }
  }
  return interrupt_ptr_index;
}

}
