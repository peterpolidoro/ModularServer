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
enum{PARAMETER_COUNT_MAX=3};
enum{FUNCTION_COUNT_MAX=3};

// Parameters

// Functions
extern ConstantString attach_function_name;
extern ConstantString detach_function_name;
extern ConstantString detach_all_function_name;
}

class Callback : private FirmwareElement
{
public:
  Callback();

  void attachFunctor(const Functor0 & functor);
  void addProperty(Property & property);
  FunctorCallbacks::Callback getIsr();
  void attachTo(const Interrupt & interrupt, const ConstantString & mode);

private:
  // static Array<Parameter,callback::PARAMETER_COUNT_MAX> parameters_;
  // static Array<Function,callback::FUNCTION_COUNT_MAX> functions_;

  FunctorCallbacks::Callback isr_;
  Array<Property *,constants::CALLBACK_PROPERTY_COUNT_MAX> property_ptrs_;
  IndexedContainer<Interrupt *,constants::CALLBACK_INTERRUPT_COUNT_MAX> interrupts_;

  Callback(const ConstantString & name);
  void setup(const ConstantString & name);
  int findPropertyIndex(const ConstantString & property_name);
  size_t getPropertyCount();
  friend class Server;
};
}
#endif
