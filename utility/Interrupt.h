// ----------------------------------------------------------------------------
// Interrupt.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_INTERRUPT_H_
#define _MODULAR_SERVER_INTERRUPT_H_
#include "Streaming.h"
#include "Array.h"
#include "ConstantVariable.h"

#include "NamedElement.h"
#include "Callback.h"
#include "Constants.h"


namespace modular_server
{
class Interrupt : private NamedElement
{
public:
  Interrupt();

  // void addParameter(Parameter & parameter);
  // void setReturnTypeLong();
  // void setReturnTypeDouble();
  // void setReturnTypeBool();
  // void setReturnTypeNull();
  // void setReturnTypeString();
  // void setReturnTypeObject();
  // void setReturnTypeArray();
  // void setReturnTypeAny();
  // void setReturnType(JsonStream::JsonTypes type);
  // JsonStream::JsonTypes getReturnType();

private:
  // Callback * callback_ptr_;
  // Functor0 functor_;
  // Array<Parameter *,constants::INTERRUPT_PARAMETER_COUNT_MAX> parameter_ptrs_;
  // JsonStream::JsonTypes return_type_;
  Interrupt(const ConstantString & name);
  void setup(const ConstantString & name);
  // int findParameterIndex(const ConstantString & parameter_name);
  // size_t getParameterCount();
  // void functor();
  // friend class Property;
  friend class Server;
};
}
#endif
