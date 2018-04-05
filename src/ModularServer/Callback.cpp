// ----------------------------------------------------------------------------
// Callback.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Callback.h"


namespace modular_server
{

namespace callback
{
// Parameters
constants::SubsetMemberType mode_ptr_subset[MODE_SUBSET_LENGTH] =
{
  {.cs_ptr=&pin::mode_low},
  {.cs_ptr=&pin::mode_change},
  {.cs_ptr=&pin::mode_rising},
  {.cs_ptr=&pin::mode_falling},
};

// Functions
CONSTANT_STRING(trigger_function_name,"trigger");
CONSTANT_STRING(attach_to_function_name,"attachTo");
CONSTANT_STRING(detach_from_function_name,"detachFrom");
}

Array<Parameter,callback::PARAMETER_COUNT_MAX> Callback::parameters_;
Array<Function,callback::FUNCTION_COUNT_MAX> Callback::functions_;
Array<constants::SubsetMemberType,constants::PIN_COUNT_MAX+1> * Callback::pin_name_array_ptr_;
Functor1wRet<const char *, Pin *> Callback::find_pin_ptr_by_chars_functor_;
Functor1wRet<const ConstantString &, Pin *> Callback::find_pin_ptr_by_constant_string_functor_;
Functor1wRet<const ConstantString &, ArduinoJson::JsonVariant> Callback::get_parameter_value_functor_;

Parameter & Callback::createParameter(const ConstantString & parameter_name)
{
  int parameter_index = findParameterIndex(parameter_name);
  if (parameter_index < 0)
  {
    parameters_.push_back(Parameter(parameter_name));
    parameters_.back().setFirmwareName(constants::firmware_name);
    return parameters_.back();
  }
  return parameters_[0]; // bad reference
}

Parameter & Callback::parameter(const ConstantString & parameter_name)
{
  int parameter_index = findParameterIndex(parameter_name);
  if ((parameter_index >= 0) && (parameter_index < (int)parameters_.size()))
  {
    return parameters_[parameter_index];
  }
  return parameters_[0]; // bad reference
}

Parameter & Callback::copyParameter(Parameter parameter, const ConstantString & parameter_name)
{
  parameters_.push_back(parameter);
  parameters_.back().setName(parameter_name);
  return parameters_.back();
}

Function & Callback::createFunction(const ConstantString & function_name)
{
  int function_index = findFunctionIndex(function_name);
  if (function_index < 0)
  {
    functions_.push_back(Function(function_name));
    functions_.back().setFirmwareName(constants::firmware_name);
    return functions_.back();
  }
  return functions_[0]; // bad reference
}

Function & Callback::function(const ConstantString & function_name)
{
  int function_index = findFunctionIndex(function_name);
  if ((function_index >= 0) && (function_index < (int)functions_.size()))
  {
    return functions_[function_index];
  }
  return functions_[0]; // bad reference
}

// public
Callback::Callback()
{
  setup(constants::empty_constant_string);
}

void Callback::attachFunctor(const Functor1<Pin *> & functor)
{
  functor_ = functor;
  for (size_t i=0; i<pin_ptrs_.max_size(); ++i)
  {
    if (pin_ptrs_.indexHasValue(i))
    {
      pin_ptrs_[i]->reattach();
    }
  }
}

void Callback::addProperty(Property & property)
{
  const ConstantString & property_name = property.getName();
  int property_index = findPropertyIndex(property_name);
  if (property_index < 0)
  {
    property_ptrs_.push_back(&property);
  }
}

Functor1<Pin *> & Callback::getFunctor()
{
  return functor_;
}

void Callback::attachTo(Pin & pin, const ConstantString & mode)
{
  if ((&mode == &pin::mode_low) ||
      (&mode == &pin::mode_change) ||
      (&mode == &pin::mode_rising) ||
      (&mode == &pin::mode_falling))
  {
    detachFrom(pin);
    int index = pin_ptrs_.add(&pin);
    if (index >= 0)
    {
      pin.attach(*this,mode);
    }
  }
}

void Callback::attachTo(const ConstantString & pin_name, const ConstantString & mode)
{
  if (pin_name == constants::all_constant_string)
  {
    attachToAll(mode);
    return;
  }
  Pin * pin_ptr = find_pin_ptr_by_constant_string_functor_(pin_name);
  if (!pin_ptr)
  {
    return;
  }
  attachTo(*pin_ptr,mode);
}

void Callback::attachTo(const char * pin_name, const char * mode_str)
{
  const ConstantString * mode_ptr = NULL;
  if (mode_str == pin::mode_low)
  {
    mode_ptr = &pin::mode_low;
  }
  else if (mode_str == pin::mode_change)
  {
    mode_ptr = &pin::mode_change;
  }
  else if (mode_str == pin::mode_rising)
  {
    mode_ptr = &pin::mode_rising;
  }
  else if (mode_str == pin::mode_falling)
  {
    mode_ptr = &pin::mode_falling;
  }
  else
  {
    return;
  }
  if (pin_name == constants::all_constant_string)
  {
    attachToAll(*mode_ptr);
    return;
  }
  Pin * pin_ptr = find_pin_ptr_by_chars_functor_(pin_name);
  if (!pin_ptr)
  {
    return;
  }
  attachTo(*pin_ptr,*mode_ptr);
}

void Callback::attachToAll(const char * mode_str)
{
  for (size_t i=0; i<pin_name_array_ptr_->size(); ++i)
  {
    const ConstantString * name_ptr = pin_name_array_ptr_->at(i).cs_ptr;
    if (name_ptr != &constants::all_constant_string)
    {
      attachTo(*name_ptr,mode_str);
    }
  }
}

void Callback::detachFrom(Pin & pin)
{
  int pin_ptr_index = findPinPtrIndex(pin);
  if (pin_ptr_index >= 0)
  {
    pin.removeCallback();
    pin_ptrs_.remove(pin_ptr_index);
  }
}

void Callback::detachFrom(const ConstantString & pin_name)
{
  if (pin_name == constants::all_constant_string)
  {
    detachFromAll();
    return;
  }
  int pin_ptr_index = findPinPtrIndex(pin_name);
  if (pin_ptr_index >= 0)
  {
    pin_ptrs_[pin_ptr_index]->removeCallback();
    pin_ptrs_.remove(pin_ptr_index);
  }
}

void Callback::detachFrom(const char * pin_name)
{
  if (pin_name == constants::all_constant_string)
  {
    detachFromAll();
    return;
  }
  int pin_ptr_index = findPinPtrIndex(pin_name);
  if (pin_ptr_index >= 0)
  {
    pin_ptrs_[pin_ptr_index]->removeCallback();
    pin_ptrs_.remove(pin_ptr_index);
  }
}

void Callback::detachFromAll()
{
  for (size_t i=0; i<pin_ptrs_.max_size(); ++i)
  {
    if (pin_ptrs_.indexHasValue(i))
    {
      detachFrom(*pin_ptrs_[i]);
    }
  }
}

void Callback::writeApi(Response & response,
                        bool write_name_only,
                        bool write_firmware,
                        bool write_function_parameter_pin_details,
                        bool write_property_details,
                        bool write_instance_details)
{
  if (response.error())
  {
    return;
  }

  const ConstantString & name = getName();
  if (write_name_only)
  {
    response.write(name);
    return;
  }

  updateFunctionsAndParameters();

  response.beginObject();

  response.write(constants::name_constant_string,name);
  if (write_firmware)
  {
    const ConstantString & firmware_name = getFirmwareName();
    response.write(constants::firmware_constant_string,firmware_name);
  }

  Array<Property *,constants::CALLBACK_PROPERTY_COUNT_MAX> * property_ptrs_ptr = NULL;
  property_ptrs_ptr = &property_ptrs_;
  if (property_ptrs_ptr->size() > 0)
  {
    response.writeKey(constants::properties_constant_string);
    response.beginArray();
    for (size_t i=0; i<property_ptrs_ptr->size(); ++i)
    {
      Property & property = *((*property_ptrs_ptr)[i]);
      property.writeApi(response,!write_property_details,false,true,write_instance_details);
    }
    response.endArray();
  }

  if (write_instance_details)
  {
    response.writeKey(constants::pins_constant_string);
    response.beginArray();
    IndexedContainer<Pin *,constants::CALLBACK_PIN_COUNT_MAX> * pin_ptrs_ptr = NULL;
    pin_ptrs_ptr = &pin_ptrs_;
    for (size_t i=0; i<pin_ptrs_ptr->max_size(); ++i)
    {
      if (pin_ptrs_ptr->indexHasValue(i))
      {
        Pin & pin = *((*pin_ptrs_ptr)[i]);
        pin.writeApi(response,!write_function_parameter_pin_details,false);
      }
    }
    response.endArray();
  }

  response.writeKey(constants::functions_constant_string);
  response.beginArray();
  for (size_t i=0; i<Callback::functions_.size(); ++i)
  {
    Function & function = Callback::functions_[i];
    function.writeApi(response,!write_function_parameter_pin_details,false,false);
  }
  response.endArray();

  response.writeKey(constants::parameters_constant_string);
  response.beginArray();
  for (size_t i=0; i<Callback::parameters_.size(); ++i)
  {
    Parameter & parameter = Callback::parameters_[i];
    parameter.writeApi(response,!write_function_parameter_pin_details,false,false,write_instance_details);
  }
  response.endArray();

  response.endObject();
}

// protected

// private
Callback::Callback(const ConstantString & name)
{
  setup(name);
}

void Callback::setup(const ConstantString & name)
{
  setName(name);
}

int Callback::findPropertyIndex(const ConstantString & property_name)
{
  int property_index = -1;
  for (size_t i=0; i<property_ptrs_.size(); ++i)
  {
    if (property_ptrs_[i]->compareName(property_name))
    {
      property_index = i;
      break;
    }
  }
  return property_index;
}

size_t Callback::getPropertyCount()
{
  return property_ptrs_.size();
}

int Callback::findPinPtrIndex(Pin & pin)
{
  int pin_ptr_index = -1;
  for (size_t i=0; i<pin_ptrs_.max_size(); ++i)
  {
    if (pin_ptrs_.indexHasValue(i))
    {
      if (pin_ptrs_[i] == &pin)
      {
        pin_ptr_index = i;
        break;
      }
    }
  }
  return pin_ptr_index;
}

int Callback::findPinPtrIndex(const ConstantString & pin_name)
{
  int pin_ptr_index = -1;
  for (size_t i=0; i<pin_ptrs_.max_size(); ++i)
  {
    if (pin_ptrs_.indexHasValue(i))
    {
      if (pin_ptrs_[i]->compareName(pin_name))
      {
        pin_ptr_index = i;
        break;
      }
    }
  }
  return pin_ptr_index;
}

int Callback::findPinPtrIndex(const char * pin_name)
{
  int pin_ptr_index = -1;
  for (size_t i=0; i<pin_ptrs_.max_size(); ++i)
  {
    if (pin_ptrs_.indexHasValue(i))
    {
      if (pin_ptrs_[i]->compareName(pin_name))
      {
        pin_ptr_index = i;
        break;
      }
    }
  }
  return pin_ptr_index;
}

void Callback::functor(Pin * pin_ptr)
{
  if (functor_)
  {
    functor_(pin_ptr);
  }
}

void Callback::updateFunctionsAndParameters()
{
  // Parameters
  parameters_.clear();

  Parameter & pin_parameter = createParameter(constants::pin_constant_string);
  pin_parameter.setTypeString();
  pin_parameter.setSubset(pin_name_array_ptr_->data(),
                          pin_name_array_ptr_->max_size(),
                          pin_name_array_ptr_->size());

  Parameter & mode_parameter = createParameter(constants::mode_constant_string);
  mode_parameter.setTypeString();
  mode_parameter.setSubset(callback::mode_ptr_subset);

  // Functions
  functions_.clear();

  Function & trigger_function = createFunction(callback::trigger_function_name);
  trigger_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Callback::triggerHandler));

  Function & attach_to_function = createFunction(callback::attach_to_function_name);
  attach_to_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Callback::attachToHandler));
  attach_to_function.addParameter(pin_parameter);
  attach_to_function.addParameter(mode_parameter);

  Function & detach_from_function = createFunction(callback::detach_from_function_name);
  detach_from_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Callback::detachFromHandler));
  detach_from_function.addParameter(pin_parameter);
}

void Callback::triggerHandler()
{
  functor(NULL);
}

void Callback::attachToHandler()
{
  const char * pin_str = get_parameter_value_functor_(constants::pin_constant_string);
  const char * mode_str = get_parameter_value_functor_(constants::mode_constant_string);
  attachTo(pin_str,mode_str);
}

void Callback::detachFromHandler()
{
  const char * pin_str = get_parameter_value_functor_(constants::pin_constant_string);
  detachFrom(pin_str);
}

}
