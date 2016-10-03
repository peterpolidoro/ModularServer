// ----------------------------------------------------------------------------
// Method.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_METHOD_H_
#define _MODULAR_SERVER_METHOD_H_
#include "Streaming.h"
#include "Array.h"
#include "ConstantVariable.h"
#include "JsonStream.h"
#include "Functor.h"

#include "Parameter.h"
#include "Constants.h"


namespace modular_server
{
class Server;

class Method
{
public:
  Method();
  Method(const ConstantString & name);
  void setName(const ConstantString & name);
  void attachCallback(const Functor0 & callback);
  void addParameter(Parameter & parameter);
  void setReturnTypeLong();
  void setReturnTypeDouble();
  void setReturnTypeBool();
  void setReturnTypeNull();
  void setReturnTypeString();
  void setReturnTypeObject();
  void setReturnTypeArray();
  void setReturnTypeValue();
  JsonStream::JsonTypes getReturnType();
protected:
  const ConstantString * name_ptr_;
  Functor0 callback_;
  Array<Parameter *,constants::METHOD_PARAMETER_COUNT_MAX> parameter_ptrs_;
  int findParameterIndex(const ConstantString & parameter_name);
  JsonStream::JsonTypes return_type_;
  void setup(const ConstantString & name);
  bool compareName(const char * name_to_compare);
  bool compareName(const ConstantString & name_to_compare);
  const ConstantString &  getName();
  size_t getParameterCount();
  void callback();
  friend class Server;
};
}
#endif
