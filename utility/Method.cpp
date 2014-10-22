// ----------------------------------------------------------------------------
// Method.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Method.h"


using namespace ArduinoJson;

namespace RemoteDevice
{
FLASH_STRING(default_method_name,"");

Method::Method(_FLASH_STRING& name=default_method_name)
{
  setName(name);
  callback_attached_ = false;
  reserved_ = false;
  parameter_count_ = 0;
}

void Method::setName(_FLASH_STRING& name)
{
  name_ptr_ = &name;
}

boolean Method::compareName(char *name_to_compare)
{
  char name[STRING_LENGTH_METHOD_NAME] = {0};
  name_ptr_->copy(name);
  return String(name).equalsIgnoreCase(name_to_compare);
}

_FLASH_STRING* Method::getNamePointer()
{
  return name_ptr_;
}

void Method::attachCallback(Callback callback)
{
  callback_ = callback;
  callback_attached_ = true;
  reserved_ = false;
}

void Method::addParameter(Parameter parameter)
{
  char parameter_name[STRING_LENGTH_PARAMETER_NAME] = {0};
  _FLASH_STRING* parameter_name_ptr = parameter.getNamePointer();
  parameter_name_ptr->copy(parameter_name);
  if (String(parameter_name).length() > 0)
  {
    int parameter_index = getParameterIndex(parameter_name);
    if (parameter_index < 0)
    {
      parameter_vector_.push_back(parameter);
      parameter_count_++;
    }
    else
    {
      parameter_vector_[parameter_index] = Parameter(*parameter_name_ptr);
    }
  }
}

int Method::getParameterIndex(char *parameter_name)
{
  int parameter_index = -1;
  for (std::vector<Parameter>::iterator it = parameter_vector_.begin();
       it != parameter_vector_.end();
       ++it)
  {
    if (it->compareName(parameter_name))
    {
      parameter_index = std::distance(parameter_vector_.begin(),it);
      break;
    }
  }
  return parameter_index;
}

void Method::callback()
{
  if ((callback_attached_) && (!isReserved()))
  {
    (*callback_)();
  }
}

void Method::attachReservedCallback(ReservedCallback callback)
{
  reserved_callback_ = callback;
  callback_attached_ = true;
  reserved_ = true;
}

boolean Method::isReserved()
{
  return reserved_;
}

void Method::reservedCallback(RemoteDevice *dev)
{
  if ((callback_attached_) && (isReserved()))
  {
    (dev->*reserved_callback_)();
  }
}
}
