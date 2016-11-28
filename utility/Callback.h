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
enum{FUNCTION_COUNT_MAX=4};

// Parameters
enum{MODE_SUBSET_LENGTH=4};
extern constants::SubsetMemberType mode_ptr_subset[MODE_SUBSET_LENGTH];

// Functions
extern ConstantString call_function_name;
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
  void attachTo(const char * interrupt_name, const char * mode_str);
  void detachFrom(const Interrupt & interrupt);
  void detachFrom(const ConstantString & interrupt_name);
  void detachFrom(const char * interrupt_name);
  void detachFromAll();

private:
  static Array<Parameter,callback::PARAMETER_COUNT_MAX> parameters_;
  static Array<Function,callback::FUNCTION_COUNT_MAX> functions_;
  static Array<constants::SubsetMemberType,constants::INTERRUPT_COUNT_MAX> * interrupt_name_array_ptr_;
  static Functor1wRet<const char *, Interrupt *> find_interrupt_ptr_functor_;
  static Functor1wRet<const ConstantString &, ArduinoJson::JsonVariant> get_parameter_value_functor_;

  template <typename T>
  static int findParameterIndex(T const & parameter_name)
  {
    int parameter_index = -1;
    for (size_t i=0; i<parameters_.size(); ++i)
    {
      if (parameters_[i].compareName(parameter_name))
      {
        parameter_index = i;
        break;
      }
    }
    return parameter_index;
  };
  static Parameter & createParameter(const ConstantString & parameter_name);
  static Parameter & parameter(const ConstantString & parameter_name);
  static Parameter & copyParameter(Parameter parameter, const ConstantString & parameter_name);

  template <typename T>
  static int findFunctionIndex(T const & function_name)
  {
    int function_index = -1;
    for (size_t i=0; i<functions_.size(); ++i)
    {
      if (functions_[i].compareName(function_name))
      {
        function_index = i;
        break;
      }
    }
    return function_index;
  };
  static Function & createFunction(const ConstantString & function_name);
  static Function & function(const ConstantString & function_name);

  FunctorCallbacks::Callback isr_;
  Array<Property *,constants::CALLBACK_PROPERTY_COUNT_MAX> property_ptrs_;
  IndexedContainer<Interrupt *,constants::CALLBACK_INTERRUPT_COUNT_MAX> interrupt_ptrs_;

  Callback(const ConstantString & name);
  void setup(const ConstantString & name);
  int findPropertyIndex(const ConstantString & property_name);
  size_t getPropertyCount();
  int findInterruptPtrIndex(const Interrupt & interrupt);
  int findInterruptPtrIndex(const ConstantString & interrupt_name);
  int findInterruptPtrIndex(const char * interrupt_name);
  void updateFunctionsAndParameters();

  // Handlers
  void callHandler();
  void attachToHandler();
  void detachFromHandler();
  void detachFromAllHandler();

  friend class Server;
};
}
#endif
