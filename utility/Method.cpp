// ----------------------------------------------------------------------------
// Method.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Method.h"


namespace modular_server
{
// public
Method::Method()
{
  setup(constants::empty_constant_string);
}

void Method::attachFunctor(const Functor0 & functor)
{
  functor_ = functor;
}

void Method::addParameter(Parameter & parameter)
{
  const ConstantString & parameter_name = parameter.getName();
  int parameter_index = findParameterIndex(parameter_name);
  if (parameter_index < 0)
  {
    parameter_ptrs_.push_back(&parameter);
  }
}

void Method::setReturnTypeLong()
{
  return_type_ = JsonStream::LONG_TYPE;
}

void Method::setReturnTypeDouble()
{
  return_type_ = JsonStream::DOUBLE_TYPE;
}

void Method::setReturnTypeBool()
{
  return_type_ = JsonStream::BOOL_TYPE;
}

void Method::setReturnTypeNull()
{
  return_type_ = JsonStream::NULL_TYPE;
}

void Method::setReturnTypeString()
{
  return_type_ = JsonStream::STRING_TYPE;
}

void Method::setReturnTypeObject()
{
  return_type_ = JsonStream::OBJECT_TYPE;
}

void Method::setReturnTypeArray()
{
  return_type_ = JsonStream::ARRAY_TYPE;
}

void Method::setReturnTypeValue()
{
  return_type_ = JsonStream::VALUE_TYPE;
}

JsonStream::JsonTypes Method::getReturnType()
{
  return return_type_;
}

// protected

// private
Method::Method(const ConstantString & name)
{
  setup(name);
}

void Method::setup(const ConstantString & name)
{
  setName(name);
  return_type_ = JsonStream::NULL_TYPE;
}

int Method::findParameterIndex(const ConstantString & parameter_name)
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

size_t Method::getParameterCount()
{
  return parameter_ptrs_.size();
}

void Method::functor()
{
  if (functor_)
  {
    functor_();
  }
}

}
