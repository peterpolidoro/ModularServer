// ----------------------------------------------------------------------------
// HardwareElement.h
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_HARDWARE_ELEMENT_H_
#define _MODULAR_SERVER_HARDWARE_ELEMENT_H_
#include <ConstantVariable.h>
#include <ArduinoJson.h>

#include "NamedElement.h"
#include "Constants.h"


namespace modular_server
{
class HardwareElement : public NamedElement
{
public:
  HardwareElement();

  void setHardwareName(const ConstantString & hardware_name);
  bool compareHardwareName(const char * hardware_name_to_compare);
  bool compareHardwareName(const ConstantString & hardware_name_to_compare);
  bool hardwareNameInArray(ArduinoJson::JsonArray hardware_name_array);
  const ConstantString &  getHardwareName();

private:
  const ConstantString * hardware_name_ptr_;

};
}

#endif
