// ----------------------------------------------------------------------------
// Function.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Function.h"


namespace modular_server
{
// public
Function::Function()
{
  setup(constants::empty_constant_string);
}

void Function::attachFunctor(const Functor0 & functor)
{
  functor_ = functor;
}

void Function::addParameter(Parameter & parameter)
{
  const ConstantString & parameter_name = parameter.getName();
  int parameter_index = findParameterIndex(parameter_name);
  if (parameter_index < 0)
  {
    parameter_ptrs_.push_back(&parameter);
  }
}

void Function::setResultTypeLong()
{
  if (result_type_ != JsonStream::ARRAY_TYPE)
  {
    result_type_ = JsonStream::LONG_TYPE;
  }
  else
  {
    result_array_element_type_ = JsonStream::LONG_TYPE;
  }
}

void Function::setResultTypeDouble()
{
  if (result_type_ != JsonStream::ARRAY_TYPE)
  {
    result_type_ = JsonStream::DOUBLE_TYPE;
  }
  else
  {
    result_array_element_type_ = JsonStream::DOUBLE_TYPE;
  }
}

void Function::setResultTypeBool()
{
  if (result_type_ != JsonStream::ARRAY_TYPE)
  {
    result_type_ = JsonStream::BOOL_TYPE;
  }
  else
  {
    result_array_element_type_ = JsonStream::BOOL_TYPE;
  }
}

void Function::setResultTypeNull()
{
  result_type_ = JsonStream::NULL_TYPE;
  result_array_element_type_ = JsonStream::NULL_TYPE;
}

void Function::setResultTypeString()
{
  if (result_type_ != JsonStream::ARRAY_TYPE)
  {
    result_type_ = JsonStream::STRING_TYPE;
    result_array_element_type_ = JsonStream::STRING_TYPE;
  }
  else
  {
    result_array_element_type_ = JsonStream::STRING_TYPE;
  }
}

void Function::setResultTypeObject()
{
  result_type_ = JsonStream::OBJECT_TYPE;
}

void Function::setResultTypeArray()
{
  if (result_type_ != JsonStream::ARRAY_TYPE)
  {
    result_array_element_type_ = result_type_;
    result_type_ = JsonStream::ARRAY_TYPE;
  }
}

void Function::setResultTypeAny()
{
  if (result_type_ != JsonStream::ARRAY_TYPE)
  {
    result_type_ = JsonStream::ANY_TYPE;
  }
  else
  {
    result_array_element_type_ = JsonStream::ANY_TYPE;
  }
}

void Function::setResultType(JsonStream::JsonTypes type)
{
  if (result_type_ != JsonStream::ARRAY_TYPE)
  {
    result_type_ = type;
  }
  else
  {
    result_array_element_type_ = type;
  }
}

JsonStream::JsonTypes Function::getResultType()
{
  return result_type_;
}

JsonStream::JsonTypes Function::getResultArrayElementType()
{
  return result_array_element_type_;
}

void Function::setResultUnits(const ConstantString & units)
{
  result_units_ptr_ = &units;
}

const ConstantString & Function::getResultUnits()
{
  return *result_units_ptr_;
}

// protected

// private
Function::Function(const ConstantString & name)
{
  setup(name);
}

void Function::setup(const ConstantString & name)
{
  setName(name);
  result_type_ = JsonStream::NULL_TYPE;
  result_array_element_type_ = JsonStream::NULL_TYPE;
  setResultUnits(constants::empty_constant_string);
}

int Function::findParameterIndex(const ConstantString & parameter_name)
{
  int parameter_index = -1;
  for (size_t i=0; i<parameter_ptrs_.size(); ++i)
  {
    if (parameter_ptrs_[i]->compareName(parameter_name))
    {
      parameter_index = i;
      break;
    }
  }
  return parameter_index;
}

size_t Function::getParameterCount()
{
  return parameter_ptrs_.size();
}

void Function::functor()
{
  if (functor_)
  {
    functor_();
  }
}

}
