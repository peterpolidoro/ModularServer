// ----------------------------------------------------------------------------
// FirmwareElement.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_FIRMWARE_ELEMENT_H_
#define _MODULAR_SERVER_FIRMWARE_ELEMENT_H_
#include "ConstantVariable.h"

#include "Constants.h"


namespace modular_server
{
class FirmwareElement
{
public:
  FirmwareElement();

  void setName(const ConstantString & name);
  bool compareName(const char * name_to_compare);
  bool compareName(const ConstantString & name_to_compare);
  const ConstantString & getName();

  void setFirmwareName(const ConstantString & firmware_name);
  bool compareFirmwareName(const char * firmware_name_to_compare);
  bool compareFirmwareName(const ConstantString & firmware_name_to_compare);
  const ConstantString &  getFirmwareName();

private:
  const ConstantString * name_ptr_;
  const ConstantString * firmware_name_ptr_;

};
}

#endif
