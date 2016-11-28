// ----------------------------------------------------------------------------
// Function.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_FUNCTION_H_
#define _MODULAR_SERVER_FUNCTION_H_
#include "Streaming.h"
#include "Array.h"
#include "ConstantVariable.h"
#include "JsonStream.h"
#include "Functor.h"

#include "FirmwareElement.h"
#include "Parameter.h"
#include "Constants.h"


namespace modular_server
{
class Function : private FirmwareElement
{
public:
  Function();

  void attachFunctor(const Functor0 & functor);
  void addParameter(Parameter & parameter);
  void setReturnTypeLong();
  void setReturnTypeDouble();
  void setReturnTypeBool();
  void setReturnTypeNull();
  void setReturnTypeString();
  void setReturnTypeObject();
  void setReturnTypeArray();
  void setReturnTypeAny();
  void setReturnType(JsonStream::JsonTypes type);
  JsonStream::JsonTypes getReturnType();

private:
  Functor0 functor_;
  Array<Parameter *,constants::FUNCTION_PARAMETER_COUNT_MAX> parameter_ptrs_;
  JsonStream::JsonTypes return_type_;
  Function(const ConstantString & name);
  void setup(const ConstantString & name);
  int findParameterIndex(const ConstantString & parameter_name);
  size_t getParameterCount();
  void functor();
  friend class Property;
  friend class Callback;
  friend class Server;
};
}
#endif
