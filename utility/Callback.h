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

#include "FirmwareElement.h"
#include "Property.h"
#include "Constants.h"


namespace modular_server
{

namespace callback
{
enum{PARAMETER_COUNT_MAX=3};
enum{FUNCTION_COUNT_MAX=3};

// Parameters
extern ConstantString interrupt_parameter_name;
extern ConstantString mode_parameter_name;
extern ConstantString pullup_parameter_name;

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

private:
  Functor0 functor_;
  Array<Property *,constants::CALLBACK_PROPERTY_COUNT_MAX> property_ptrs_;
  Callback(const ConstantString & name);
  void setup(const ConstantString & name);
  int findPropertyIndex(const ConstantString & property_name);
  size_t getPropertyCount();
  void functor();
  friend class Server;
};
}
#endif
