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

void Method::attachCallback(const Functor0 & callback)
{
  callback_ = callback;
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

void Method::setName(const ConstantString & name)
{
  name_ptr_ = &name;
}

bool Method::compareName(const char * name_to_compare)
{
  char name_str[name_ptr_->length()+1];
  name_str[0] = '\0';
  name_ptr_->copy(name_str);
  return String(name_str).equalsIgnoreCase(name_to_compare);
}

bool Method::compareName(const ConstantString & name_to_compare)
{
  return (&name_to_compare == name_ptr_);
}

const ConstantString & Method::getName()
{
  return *name_ptr_;
}

void Method::setFirmwareName(const ConstantString & firmware_name)
{
  firmware_name_ptr_ = &firmware_name;
}

bool Method::compareFirmwareName(const char * firmware_name_to_compare)
{
  char firmware_name_str[firmware_name_ptr_->length()+1];
  firmware_name_str[0] = '\0';
  firmware_name_ptr_->copy(firmware_name_str);
  return String(firmware_name_str).equalsIgnoreCase(firmware_name_to_compare);
}

bool Method::compareFirmwareName(const ConstantString & firmware_name_to_compare)
{
  return (&firmware_name_to_compare == firmware_name_ptr_);
}

const ConstantString & Method::getFirmwareName()
{
  return *firmware_name_ptr_;
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

void Method::callback()
{
  if (callback_)
  {
    callback_();
  }
}

}
