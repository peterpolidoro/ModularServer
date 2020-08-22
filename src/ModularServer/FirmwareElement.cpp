// ----------------------------------------------------------------------------
// FirmwareElement.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "FirmwareElement.h"


namespace modular_server
{
// public
FirmwareElement::FirmwareElement()
{
  setFirmwareName(constants::empty_constant_string);
}

void FirmwareElement::setFirmwareName(const ConstantString & firmware_name)
{
  firmware_name_ptr_ = &firmware_name;
}

bool FirmwareElement::compareFirmwareName(const char * firmware_name_to_compare)
{
  if (constants::all_constant_string == firmware_name_to_compare)
  {
    return true;
  }
  char firmware_name_str[firmware_name_ptr_->length()+1];
  firmware_name_str[0] = '\0';
  firmware_name_ptr_->copy(firmware_name_str);
  return (strcmp(firmware_name_str,firmware_name_to_compare) == 0);
}

bool FirmwareElement::compareFirmwareName(const ConstantString & firmware_name_to_compare)
{
  if (constants::all_constant_string == firmware_name_to_compare)
  {
    return true;
  }
  return (&firmware_name_to_compare == firmware_name_ptr_);
}

bool FirmwareElement::compareFirmwareName(const ConstantString * firmware_name_to_compare)
{
  return compareFirmwareName(*firmware_name_to_compare);
}

bool FirmwareElement::compareFirmwareName(constants::SubsetMemberType firmware_name_to_compare)
{
  char firmware_name_to_compare_str[firmware_name_to_compare.cs_ptr->length() + 1];
  firmware_name_to_compare_str[0] = '\0';
  firmware_name_to_compare.cs_ptr->copy(firmware_name_to_compare_str);

  char firmware_name_str[firmware_name_ptr_->length()+1];
  firmware_name_str[0] = '\0';
  firmware_name_ptr_->copy(firmware_name_str);


  return (strcmp(firmware_name_str,firmware_name_to_compare_str) == 0);
}

template <>
bool FirmwareElement::firmwareNameInArray<ArduinoJson::JsonArray>(ArduinoJson::JsonArray firmware_name_array)
{
  for (ArduinoJson::JsonVariant value : firmware_name_array)
  {
    const char * name = value.as<const char *>();
    if (compareFirmwareName(name))
    {
      return true;
    }
  }
  return false;
}

const ConstantString & FirmwareElement::getFirmwareName()
{
  return *firmware_name_ptr_;
}

}
