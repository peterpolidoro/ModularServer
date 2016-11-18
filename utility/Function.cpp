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

void Function::setReturnTypeLong()
{
  return_type_ = JsonStream::LONG_TYPE;
}

void Function::setReturnTypeDouble()
{
  return_type_ = JsonStream::DOUBLE_TYPE;
}

void Function::setReturnTypeBool()
{
  return_type_ = JsonStream::BOOL_TYPE;
}

void Function::setReturnTypeNull()
{
  return_type_ = JsonStream::NULL_TYPE;
}

void Function::setReturnTypeString()
{
  return_type_ = JsonStream::STRING_TYPE;
}

void Function::setReturnTypeObject()
{
  return_type_ = JsonStream::OBJECT_TYPE;
}

void Function::setReturnTypeArray()
{
  return_type_ = JsonStream::ARRAY_TYPE;
}

void Function::setReturnTypeAny()
{
  return_type_ = JsonStream::ANY_TYPE;
}

void Function::setReturnType(JsonStream::JsonTypes type)
{
  return_type_ = type;
}

JsonStream::JsonTypes Function::getReturnType()
{
  return return_type_;
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
  setReturnTypeNull();
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
