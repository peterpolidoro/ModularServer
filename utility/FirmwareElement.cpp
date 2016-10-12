// ----------------------------------------------------------------------------
// FirmwareElement.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "FirmwareElement.h"


namespace modular_server
{
// public
FirmwareElement::FirmwareElement()
{
  setName(constants::empty_constant_string);
  setFirmwareName(constants::empty_constant_string);
}

void FirmwareElement::setName(const ConstantString & name)
{
  name_ptr_ = &name;
}

bool FirmwareElement::compareName(const char * name_to_compare)
{
  char name_str[name_ptr_->length()+1];
  name_str[0] = '\0';
  name_ptr_->copy(name_str);
  return String(name_str).equalsIgnoreCase(name_to_compare);
}

bool FirmwareElement::compareName(const ConstantString & name_to_compare)
{
  return (&name_to_compare == name_ptr_);
}

const ConstantString & FirmwareElement::getName()
{
  return *name_ptr_;
}

void FirmwareElement::setFirmwareName(const ConstantString & firmware_name)
{
  firmware_name_ptr_ = &firmware_name;
}

bool FirmwareElement::compareFirmwareName(const char * firmware_name_to_compare)
{
  char firmware_name_str[firmware_name_ptr_->length()+1];
  firmware_name_str[0] = '\0';
  firmware_name_ptr_->copy(firmware_name_str);
  return String(firmware_name_str).equalsIgnoreCase(firmware_name_to_compare);
}

bool FirmwareElement::compareFirmwareName(const ConstantString & firmware_name_to_compare)
{
  return (&firmware_name_to_compare == firmware_name_ptr_);
}

const ConstantString & FirmwareElement::getFirmwareName()
{
  return *firmware_name_ptr_;
}

}
