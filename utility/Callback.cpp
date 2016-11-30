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
    {.cs_ptr=&interrupt::mode_low},
    {.cs_ptr=&interrupt::mode_change},
    {.cs_ptr=&interrupt::mode_rising},
    {.cs_ptr=&interrupt::mode_falling},
  };

// Functions
CONSTANT_STRING(trigger_function_name,"trigger");
CONSTANT_STRING(attach_to_function_name,"attachTo");
CONSTANT_STRING(detach_from_function_name,"detachFrom");
CONSTANT_STRING(detach_from_all_function_name,"detachFromAll");
}

Array<Parameter,callback::PARAMETER_COUNT_MAX> Callback::parameters_;
Array<Function,callback::FUNCTION_COUNT_MAX> Callback::functions_;
Array<constants::SubsetMemberType,constants::INTERRUPT_COUNT_MAX> * Callback::interrupt_name_array_ptr_;
Functor1wRet<const char *, Interrupt *> Callback::find_interrupt_ptr_by_chars_functor_;
Functor1wRet<const ConstantString &, Interrupt *> Callback::find_interrupt_ptr_by_constant_string_functor_;
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
}

Parameter & Callback::parameter(const ConstantString & parameter_name)
{
  int parameter_index = findParameterIndex(parameter_name);
  if ((parameter_index >= 0) && (parameter_index < (int)parameters_.size()))
  {
    return parameters_[parameter_index];
  }
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
}

Function & Callback::function(const ConstantString & function_name)
{
  int function_index = findFunctionIndex(function_name);
  if ((function_index >= 0) && (function_index < (int)functions_.size()))
  {
    return functions_[function_index];
  }
}

// public
Callback::Callback()
{
  setup(constants::empty_constant_string);
}

void Callback::attachFunctor(const Functor0 & functor)
{
  FunctorCallbacks::remove(isr_);
  isr_ = FunctorCallbacks::add(functor);
  for (size_t i=0; i<interrupt_ptrs_.max_size(); ++i)
  {
    if (interrupt_ptrs_.indexHasValue(i))
    {
      interrupt_ptrs_[i]->reattach();
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

FunctorCallbacks::Callback Callback::getIsr()
{
  return isr_;
}

void Callback::attachTo(Interrupt & interrupt, const ConstantString & mode)
{
  if ((&mode == &interrupt::mode_low) ||
      (&mode == &interrupt::mode_change) ||
      (&mode == &interrupt::mode_rising) ||
      (&mode == &interrupt::mode_falling))
  {
    int index = interrupt_ptrs_.add(&interrupt);
    if (index >= 0)
    {
      interrupt.attach(*this,mode);
    }
  }
}

void Callback::attachTo(const ConstantString & interrupt_name, const ConstantString & mode)
{
  if ((&mode == &interrupt::mode_low) ||
      (&mode == &interrupt::mode_change) ||
      (&mode == &interrupt::mode_rising) ||
      (&mode == &interrupt::mode_falling))
  {
    Interrupt * interrupt_ptr = find_interrupt_ptr_by_constant_string_functor_(interrupt_name);
    if (!interrupt_ptr)
    {
      return;
    }
    int index = interrupt_ptrs_.add(interrupt_ptr);
    if (index >= 0)
    {
      interrupt_ptr->attach(*this,mode);
    }
  }
}

void Callback::attachTo(const char * interrupt_name, const char * mode_str)
{
  Interrupt * interrupt_ptr = find_interrupt_ptr_by_chars_functor_(interrupt_name);
  if (!interrupt_ptr)
  {
    return;
  }
  const ConstantString * mode_ptr = NULL;
  if (mode_str == interrupt::mode_low)
  {
    mode_ptr = &interrupt::mode_low;
  }
  else if (mode_str == interrupt::mode_change)
  {
    mode_ptr = &interrupt::mode_change;
  }
  else if (mode_str == interrupt::mode_rising)
  {
    mode_ptr = &interrupt::mode_rising;
  }
  else if (mode_str == interrupt::mode_falling)
  {
    mode_ptr = &interrupt::mode_falling;
  }
  else
  {
    return;
  }
  int index = interrupt_ptrs_.add(interrupt_ptr);
  if (index >= 0)
  {
    interrupt_ptr->attach(*this,*mode_ptr);
  }
}

void Callback::detachFrom(Interrupt & interrupt)
{
  int interrupt_ptr_index = findInterruptPtrIndex(interrupt);
  if (interrupt_ptr_index >= 0)
  {
    interrupt.removeCallback();
    interrupt_ptrs_.remove(interrupt_ptr_index);
  }
}

void Callback::detachFrom(const ConstantString & interrupt_name)
{
  int interrupt_ptr_index = findInterruptPtrIndex(interrupt_name);
  if (interrupt_ptr_index >= 0)
  {
    interrupt_ptrs_[interrupt_ptr_index]->removeCallback();
    interrupt_ptrs_.remove(interrupt_ptr_index);
  }
}

void Callback::detachFrom(const char * interrupt_name)
{
  int interrupt_ptr_index = findInterruptPtrIndex(interrupt_name);
  if (interrupt_ptr_index >= 0)
  {
    interrupt_ptrs_[interrupt_ptr_index]->removeCallback();
    interrupt_ptrs_.remove(interrupt_ptr_index);
  }
}

void Callback::detachFromAll()
{
  for (size_t i=0; i<interrupt_ptrs_.max_size(); ++i)
  {
    if (interrupt_ptrs_.indexHasValue(i))
    {
      detachFrom(*interrupt_ptrs_[i]);
    }
  }
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
  isr_ = NULL;
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

int Callback::findInterruptPtrIndex(Interrupt & interrupt)
{
  int interrupt_ptr_index = -1;
  for (size_t i=0; i<interrupt_ptrs_.max_size(); ++i)
  {
    if (interrupt_ptrs_.indexHasValue(i))
    {
      if (interrupt_ptrs_[i] == &interrupt)
      {
        interrupt_ptr_index = i;
        break;
      }
    }
  }
  return interrupt_ptr_index;
}

int Callback::findInterruptPtrIndex(const ConstantString & interrupt_name)
{
  int interrupt_ptr_index = -1;
  for (size_t i=0; i<interrupt_ptrs_.max_size(); ++i)
  {
    if (interrupt_ptrs_.indexHasValue(i))
    {
      if (interrupt_ptrs_[i]->compareName(interrupt_name))
      {
        interrupt_ptr_index = i;
        break;
      }
    }
  }
  return interrupt_ptr_index;
}

int Callback::findInterruptPtrIndex(const char * interrupt_name)
{
  int interrupt_ptr_index = -1;
  for (size_t i=0; i<interrupt_ptrs_.max_size(); ++i)
  {
    if (interrupt_ptrs_.indexHasValue(i))
    {
      if (interrupt_ptrs_[i]->compareName(interrupt_name))
      {
        interrupt_ptr_index = i;
        break;
      }
    }
  }
  return interrupt_ptr_index;
}

void Callback::updateFunctionsAndParameters()
{
  // Parameters
  parameters_.clear();

  Parameter & interrupt_parameter = createParameter(constants::interrupt_constant_string);
  interrupt_parameter.setTypeString();
  interrupt_parameter.setSubset(interrupt_name_array_ptr_->data(),
                                interrupt_name_array_ptr_->max_size(),
                                interrupt_name_array_ptr_->size());

  Parameter & mode_parameter = createParameter(constants::mode_constant_string);
  mode_parameter.setTypeString();
  mode_parameter.setSubset(callback::mode_ptr_subset);

  // Functions
  functions_.clear();

  Function & trigger_function = createFunction(callback::trigger_function_name);
  trigger_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Callback::callHandler));

  Function & attach_to_function = createFunction(callback::attach_to_function_name);
  attach_to_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Callback::attachToHandler));
  attach_to_function.addParameter(interrupt_parameter);
  attach_to_function.addParameter(mode_parameter);

  Function & detach_from_function = createFunction(callback::detach_from_function_name);
  detach_from_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Callback::detachFromHandler));
  detach_from_function.addParameter(interrupt_parameter);

  Function & detach_from_all_function = createFunction(callback::detach_from_all_function_name);
  detach_from_all_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Callback::detachFromAllHandler));
}

void Callback::callHandler()
{
  if (isr_)
  {
    isr_();
  }
}

void Callback::attachToHandler()
{
  const char * interrupt_str = get_parameter_value_functor_(constants::interrupt_constant_string);
  const char * mode_str = get_parameter_value_functor_(constants::mode_constant_string);
  attachTo(interrupt_str,mode_str);
}

void Callback::detachFromHandler()
{
  const char * interrupt_str = get_parameter_value_functor_(constants::interrupt_constant_string);
  detachFrom(interrupt_str);
}

void Callback::detachFromAllHandler()
{
  detachFromAll();
}

}
