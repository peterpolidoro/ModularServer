// ----------------------------------------------------------------------------
// Method.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Method.h"


using namespace ArduinoJson;

namespace ModularDevice
{
FLASH_STRING(default_method_name,"");

Method::Method(const _FLASH_STRING &name=default_method_name)
{
  setName(name);
  callback_attached_ = false;
  reserved_ = false;
  parameter_count_ = 0;
}

void Method::setName(const _FLASH_STRING &name)
{
  name_ptr_ = &name;
}

void Method::attachCallback(Callback callback)
{
  callback_ = callback;
  callback_attached_ = true;
  reserved_ = false;
}

void Method::addParameter(Parameter &parameter)
{
  char parameter_name[STRING_LENGTH_PARAMETER_NAME] = {0};
  const _FLASH_STRING* parameter_name_ptr = parameter.getNamePointer();
  parameter_name_ptr->copy(parameter_name);
  if (String(parameter_name).length() > 0)
  {
    int parameter_index = findParameterIndex(*parameter_name_ptr);
    if (parameter_index < 0)
    {
      parameter_ptr_vector_.push_back(&parameter);
      parameter_count_++;
    }
  }
}

boolean Method::compareName(const char *name_to_compare)
{
  char name[STRING_LENGTH_METHOD_NAME] = {0};
  name_ptr_->copy(name);
  return String(name).equalsIgnoreCase(name_to_compare);
}

boolean Method::compareName(const _FLASH_STRING &name_to_compare)
{
  return (&name_to_compare == name_ptr_);
}

const _FLASH_STRING* Method::getNamePointer()
{
  return name_ptr_;
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

void Method::reservedCallback(Server *server)
{
  if ((callback_attached_) && (isReserved()))
  {
    (server->*reserved_callback_)();
  }
}

int Method::findParameterIndex(const _FLASH_STRING &parameter_name)
{
  int parameter_index = -1;
  for (std::vector<Parameter*>::iterator it = parameter_ptr_vector_.begin();
       it != parameter_ptr_vector_.end();
       ++it)
  {
    if ((*it)->compareName(parameter_name))
    {
      parameter_index = std::distance(parameter_ptr_vector_.begin(),it);
      break;
    }
  }
  return parameter_index;
}
}
