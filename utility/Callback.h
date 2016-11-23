// ----------------------------------------------------------------------------
// Callback.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_CALLBACK_H_
#define _MODULAR_SERVER_CALLBACK_H_
#include "Streaming.h"
#include "Array.h"
#include "ConstantVariable.h"
#include "JsonStream.h"
#include "Functor.h"
#include "IndexedContainer.h"
#include "FunctorCallbacks.h"

#include "FirmwareElement.h"
#include "Property.h"
#include "Interrupt.h"
#include "Constants.h"


namespace modular_server
{

namespace callback
{
enum{PARAMETER_COUNT_MAX=2};
enum{FUNCTION_COUNT_MAX=3};

// Parameters
enum{MODE_SUBSET_LENGTH=4};
extern constants::SubsetMemberType mode_ptr_subset[MODE_SUBSET_LENGTH];

// Functions
extern ConstantString attach_to_function_name;
extern ConstantString detach_from_function_name;
extern ConstantString detach_from_all_function_name;
}

class Interrupt;

class Callback : private FirmwareElement
{
public:
  Callback();

  void attachFunctor(const Functor0 & functor);
  void addProperty(Property & property);
  FunctorCallbacks::Callback getIsr();
  void attachTo(const Interrupt & interrupt, const ConstantString & mode);
  void detachFrom(const Interrupt & interrupt);
  template <typename T>
  void detachFrom(T const & interrupt_name)
  {
    int interrupt_ptr_index = findInterruptPtrIndex(interrupt);
    if (interrupt_ptr_index >= 0)
    {
      interrupt_ptrs_[interrupt_ptr_index]->removeCallback();
      interrupt_ptrs_.remove(interrupt_ptr_index);
    }
  };
  void detachFromAll();

private:
  // static Array<Parameter,callback::PARAMETER_COUNT_MAX> parameters_;
  // static Array<Function,callback::FUNCTION_COUNT_MAX> functions_;

  FunctorCallbacks::Callback isr_;
  Array<Property *,constants::CALLBACK_PROPERTY_COUNT_MAX> property_ptrs_;
  IndexedContainer<Interrupt *,constants::CALLBACK_INTERRUPT_COUNT_MAX> interrupt_ptrs_;

  Callback(const ConstantString & name);
  void setup(const ConstantString & name);
  int findPropertyIndex(const ConstantString & property_name);
  size_t getPropertyCount();
  int findInterruptPtrIndex(const Interrupt & interrupt);
  template <typename T>
  int findInterruptPtrIndex(T const & interrupt_name)
  {
    int interrupt_ptr_index = -1;
    for (size_t i=0; i<interrupt_ptrs_.max_size(); ++i)
    {
      if (interrupt_ptrs_.indexHasValue(i))
      {
        if (interrupts_ptrs_[i]->compareName(interrupt_name))
        {
          interrupt_ptr_index = i;
          break;
        }
      }
    }
    return interrupt_ptr_index;
  };
  friend class Server;
};
}
#endif
