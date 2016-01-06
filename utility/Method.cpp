// ----------------------------------------------------------------------------
// Method.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Method.h"


namespace ModularDevice
{
// Method
Method::Method()
{
  setup(constants::empty_constant_string);
}

Method::Method(const ConstantString &name)
{
  setup(name);
}

void Method::setup(const ConstantString &name)
{
  setName(name);
  callback_attached_ = false;
  parameter_count_ = 0;
  return_type_ = JsonStream::NULL_TYPE;
}

void Method::setName(const ConstantString &name)
{
  name_ptr_ = &name;
}

void Method::attachCallback(Callback callback)
{
  callback_ = callback;
  callback_attached_ = true;
}

void Method::addParameter(Parameter &parameter)
{
  char parameter_name[constants::STRING_LENGTH_PARAMETER_NAME] = {0};
  const ConstantString& name = parameter.getName();
  name.copy(parameter_name);
  if (strlen(parameter_name) > 0)
  {
    int parameter_index = findParameterIndex(parameter_name);
    if (parameter_index < 0)
    {
      parameter_ptrs_.push_back(&parameter);
      parameter_count_++;
    }
  }
}

bool Method::compareName(const char *name_to_compare)
{
  char name[constants::STRING_LENGTH_METHOD_NAME] = {0};
  name_ptr_->copy(name);
  return String(name).equalsIgnoreCase(name_to_compare);
}

bool Method::compareName(const ConstantString &name_to_compare)
{
  return (&name_to_compare == name_ptr_);
}

const ConstantString& Method::getName()
{
  return *name_ptr_;
}

void Method::callback()
{
  if (callback_attached_)
  {
    (*callback_)();
  }
}

int Method::findParameterIndex(const ConstantString &parameter_name)
{
  int parameter_index = -1;
  for (unsigned int i=0; i<parameter_ptrs_.size(); ++i)
  {
    if (parameter_ptrs_[i]->compareName(parameter_name))
    {
      parameter_index = i;
      break;
    }
  }
  return parameter_index;
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

JsonStream::JsonTypes Method::getReturnType()
{
  return return_type_;
}

// InternalMethod
InternalMethod::InternalMethod()
{
  setup(constants::empty_constant_string);
  private_ = false;
}

InternalMethod::InternalMethod(const ConstantString &name)
{
  setup(name);
  private_ = false;
}

void InternalMethod::attachCallback(InternalCallback callback)
{
  internal_callback_ = callback;
  callback_attached_ = true;
}

void InternalMethod::callback(Server *server)
{
  if (callback_attached_)
  {
    (server->*internal_callback_)();
  }
}

void InternalMethod::setPrivacy(bool is_private)
{
  private_ = is_private;
}

bool InternalMethod::isPrivate()
{
  return private_;
}
}
