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
#include "ArduinoJson.h"

#include "NamedElement.h"
#include "Constants.h"


namespace modular_server
{
class FirmwareElement : public NamedElement
{
public:
  FirmwareElement();

  void setFirmwareName(const ConstantString & firmware_name);
  bool compareFirmwareName(const char * firmware_name_to_compare);
  bool compareFirmwareName(const ConstantString & firmware_name_to_compare);
  bool compareFirmwareName(constants::SubsetMemberType firmware_name_to_compare);
  bool firmwareNameInArray(ArduinoJson::JsonArray & firmware_name_array);
  const ConstantString &  getFirmwareName();

private:
  const ConstantString * firmware_name_ptr_;

};
}

#endif
