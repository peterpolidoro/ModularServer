// ----------------------------------------------------------------------------
// Interrupt.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_INTERRUPT_H_
#define _MODULAR_SERVER_INTERRUPT_H_
#include "Streaming.h"
#include "Array.h"
#include "ConstantVariable.h"
#include "JsonStream.h"
#include "Functor.h"

#include "FirmwareElement.h"
#include "Field.h"
#include "Constants.h"


namespace modular_server
{
class Interrupt : private FirmwareElement
{
public:
  Interrupt();

  void attachCallback(const Functor0 & callback);
  void addField(Field & field);

private:
  Functor0 callback_;
  Array<Field *,constants::INTERRUPT_FIELD_COUNT_MAX> field_ptrs_;
  Interrupt(const ConstantString & name);
  void setup(const ConstantString & name);
  int findFieldIndex(const ConstantString & field_name);
  size_t getFieldCount();
  void callback();
  friend class Server;
};
}
#endif
