// ----------------------------------------------------------------------------
// Method.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Method.h"

Method::Method(char *name="")
{
  setName(name);
  callback_attached_ = false;
  reserved_ = false;
  parameter_count_ = 0;
}

void Method::setName(char *name)
{
  strncpy(name_,name,STRING_LENGTH_METHOD_NAME);
}

boolean Method::compareName(char *name_to_compare)
{
  return String(name_).equalsIgnoreCase(name_to_compare);
}

char* Method::getName()
{
  return name_;
}

void Method::attachCallback(Callback callback)
{
  callback_ = callback;
  callback_attached_ = true;
  reserved_ = false;
}

void Method::addParameter(Parameter parameter)
{
  char* name = parameter.getName();
  if (String(name).length() > 0)
  {
    int parameter_index = getParameterIndex(name);
    if (parameter_index < 0)
    {
      parameter_vector_.push_back(parameter);
      parameter_count_++;
    }
    else
    {
      parameter_vector_[parameter_index] = Parameter(name);
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

Generator::JsonArray<JSON_OBJECT_SIZE_METHOD_HELP> Method::help()
{
  // Generator::JsonObject<JSON_OBJECT_SIZE_METHOD_HELP> help_json_object;
  // int parameter_count = 0;
  // for (std::vector<Parameter>::iterator it = parameter_vector_.begin();
  //      it != parameter_vector_.end();
  //      ++it)
  // {
  //   if (++parameter_count < JSON_OBJECT_SIZE_METHOD_HELP)
  //   {
  //     help_json_object[it->getName()] = it->help();
  //   }
  // }
  // help_json_object.printTo(help_string,STRING_LENGTH_METHOD_HELP);
  // return help_string;
  help_json_array = Generator::JsonArray<JSON_OBJECT_SIZE_METHOD_HELP>();
  int parameter_index = 0;
  for (std::vector<Parameter>::iterator it = parameter_vector_.begin();
       it != parameter_vector_.end();
       ++it)
  {
    if (parameter_index < JSON_OBJECT_SIZE_METHOD_HELP)
    {
      help_json_array.add(it->getName());
      // help_json_array.add("test");
      parameter_index++;
    }
  }
  // char help_string[STRING_LENGTH_METHOD_HELP];
  // help_json_array.printTo(help_string,STRING_LENGTH_METHOD_HELP);
  return help_json_array;
}
