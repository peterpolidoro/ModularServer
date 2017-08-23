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
#include "Response.h"
#include "Constants.h"


namespace modular_server
{
class Function : private FirmwareElement
{
public:
  Function();

  void attachFunctor(const Functor0 & functor);
  void addParameter(Parameter & parameter);

  void setResultTypeLong();
  void setResultTypeDouble();
  void setResultTypeBool();
  void setResultTypeNull();
  void setResultTypeString();
  void setResultTypeObject();
  void setResultTypeArray();
  void setResultTypeAny();
  void setResultType(JsonStream::JsonTypes type);
  JsonStream::JsonTypes getResultType();
  JsonStream::JsonTypes getResultArrayElementType();

  void setResultUnits(const ConstantString & units);
  const ConstantString & getResultUnits();

  void writeApi(Response & response,
                bool write_name_only,
                bool write_firmware,
                bool verbose);

private:
  Functor0 functor_;
  Array<Parameter *,constants::FUNCTION_PARAMETER_COUNT_MAX> parameter_ptrs_;
  JsonStream::JsonTypes result_type_;
  JsonStream::JsonTypes result_array_element_type_;
  const ConstantString * result_units_ptr_;

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
