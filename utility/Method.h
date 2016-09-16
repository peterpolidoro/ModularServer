// ----------------------------------------------------------------------------
// Method.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_DEVICE_METHOD_H_
#define _MODULAR_DEVICE_METHOD_H_
#include "Streaming.h"
#include "Array.h"
#include "ConstantVariable.h"
#include "JsonStream.h"
#include "Parameter.h"
#include "Constants.h"


namespace ModularDevice
{
class Server;

typedef void (*Callback)(void);
typedef void (Server::*InternalCallback)(void);

class Method
{
public:
  Method();
  Method(const ConstantString &name);
  void setName(const ConstantString &name);
  void attachCallback(Callback callback);
  void addParameter(Parameter &parameter);
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
  Callback callback_;
  Array<Parameter *,constants::METHOD_PARAMETER_COUNT_MAX> parameter_ptrs_;
  int findParameterIndex(const ConstantString &parameter_name);
  int parameter_count_;
  JsonStream::JsonTypes return_type_;
  void setup(const ConstantString &name);
  bool compareName(const char * name_to_compare);
  bool compareName(const ConstantString &name_to_compare);
  const ConstantString& getName();
  void callback();
  friend class Server;
};

class InternalMethod : public Method
{
public:
  InternalMethod();
  InternalMethod(const ConstantString &name);
  void attachCallback(InternalCallback callback);
  void callback(Server * server);
  void setPrivacy(bool is_private);
  bool isPrivate();
private:
  InternalCallback internal_callback_;
  bool private_;
};
}
#endif
