// ----------------------------------------------------------------------------
// NamedElement.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_NAMED_ELEMENT_H_
#define _MODULAR_SERVER_NAMED_ELEMENT_H_
#include <ConstantVariable.h>
#include <ArduinoJson.h>

#include "Constants.h"


namespace modular_server
{
class NamedElement
{
public:
  NamedElement();

  void setName(const ConstantString & name);
  bool compareName(const char * name_to_compare);
  bool compareName(const ConstantString & name_to_compare);
  const ConstantString & getName();

private:
  const ConstantString * name_ptr_;

};
}

#endif
