// ----------------------------------------------------------------------------
// HardwareElement.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "HardwareElement.h"


namespace modular_server
{
// public
HardwareElement::HardwareElement()
{
  setHardwareName(constants::empty_constant_string);
}

void HardwareElement::setHardwareName(const ConstantString & hardware_name)
{
  hardware_name_ptr_ = &hardware_name;
}

bool HardwareElement::compareHardwareName(const char * hardware_name_to_compare)
{
  if (constants::all_constant_string == hardware_name_to_compare)
  {
    return true;
  }
  char hardware_name_str[hardware_name_ptr_->length()+1];
  hardware_name_str[0] = '\0';
  hardware_name_ptr_->copy(hardware_name_str);
  return (strcmp(hardware_name_str,hardware_name_to_compare) == 0);
}

bool HardwareElement::compareHardwareName(const ConstantString & hardware_name_to_compare)
{
  if (constants::all_constant_string == hardware_name_to_compare)
  {
    return true;
  }
  return (&hardware_name_to_compare == hardware_name_ptr_);
}

bool HardwareElement::hardwareNameInArray(ArduinoJson::JsonArray hardware_name_array)
{
  for (ArduinoJson::JsonVariant value : hardware_name_array)
  {
    const char * name = value.as<const char *>();
    if (compareHardwareName(name))
    {
      return true;
    }
  }
  return false;
}

const ConstantString & HardwareElement::getHardwareName()
{
  return *hardware_name_ptr_;
}

}
