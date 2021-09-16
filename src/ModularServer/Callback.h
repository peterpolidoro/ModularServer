// ----------------------------------------------------------------------------
// Callback.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_CALLBACK_H_
#define _MODULAR_SERVER_CALLBACK_H_
#include <Streaming.h>
#include <Array.h>
#include <ConstantVariable.h>
#include <JsonStream.h>
#include <Functor.h>
#include <IndexedContainer.h>

#include "FirmwareElement.h"
#include "Property.h"
#include "Pin.h"
#include "Response.h"
#include "Constants.h"


namespace modular_server
{

namespace callback
{
enum{PARAMETER_COUNT_MAX=2};
enum{FUNCTION_COUNT_MAX=3};

// Parameters
enum{PIN_MODE_SUBSET_LENGTH=4};
extern constants::SubsetMemberType pin_mode_ptr_subset[PIN_MODE_SUBSET_LENGTH];

// Functions
extern ConstantString trigger_function_name;
extern ConstantString attach_to_function_name;
extern ConstantString detach_from_function_name;
}

class Pin;

class Callback : private FirmwareElement
{
public:
  Callback();

  void attachFunctor(const Functor1<Pin *> & functor);
  void addProperty(Property & property);
  Functor1<Pin *> & getFunctor();
  void attachTo(Pin & pin,
    const ConstantString & pin_mode);
  void attachTo(const ConstantString & pin_name,
    const ConstantString & pin_mode);
  void attachTo(const char * pin_name,
    const char * pin_mode);
  void attachToAll(const ConstantString & pin_mode);
  void attachToAll(const char * pin_mode);
  void detachFrom(Pin & pin);
  void detachFrom(const ConstantString & pin_name);
  void detachFrom(const char * pin_name);
  void detachFromAll();

  void writeApi(Response & response,
    bool write_name_only,
    bool write_firmware,
    bool write_function_parameter_pin_details,
    bool write_property_details,
    bool write_instance_details);

private:
  static Array<Parameter,callback::PARAMETER_COUNT_MAX> parameters_;
  static Array<Function,callback::FUNCTION_COUNT_MAX> functions_;
  static Array<constants::SubsetMemberType,constants::PIN_COUNT_MAX+1> * pin_name_array_ptr_;
  static Functor1wRet<const char *,Pin *> find_pin_ptr_by_chars_functor_;
  static Functor1wRet<const ConstantString &,Pin *> find_pin_ptr_by_constant_string_functor_;
  static Functor1wRet<const char *,Pin *> find_pin_ptr_functor_;
  static Functor1wRet<const ConstantString &,ArduinoJson::JsonVariant> get_parameter_value_functor_;

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
  static Parameter & copyParameter(Parameter parameter,
    const ConstantString & parameter_name);

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

  Functor1<Pin *> functor_;
  Array<Property *,constants::CALLBACK_PROPERTY_COUNT_MAX> property_ptrs_;
  IndexedContainer<Pin *,constants::CALLBACK_PIN_COUNT_MAX> pin_ptrs_;

  Callback(const ConstantString & name);
  void setup(const ConstantString & name);
  int findPropertyIndex(const ConstantString & property_name);
  size_t getPropertyCount();
  int findPinPtrIndex(Pin & pin);
  int findPinPtrIndex(const ConstantString & pin_name);
  int findPinPtrIndex(const char * pin_name);
  void functor(Pin * pin_ptr);
  void updateFunctionsAndParameters();

  // Handlers
  void triggerHandler();
  void attachToHandler();
  void detachFromHandler();

  friend class Server;
  friend class Pin;
};
}
#endif
