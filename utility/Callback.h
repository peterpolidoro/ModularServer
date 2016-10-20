// ----------------------------------------------------------------------------
// Callback.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_CALLBACK_H_
#define _MODULAR_SERVER_CALLBACK_H_
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
class Callback : private FirmwareElement
{
public:
  Callback();

  void attachFunctor(const Functor0 & functor);
  void addField(Field & field);

private:
  Functor0 functor_;
  Array<Field *,constants::CALLBACK_FIELD_COUNT_MAX> field_ptrs_;
  Callback(const ConstantString & name);
  void setup(const ConstantString & name);
  int findFieldIndex(const ConstantString & field_name);
  size_t getFieldCount();
  void functor();
  friend class Server;
};
}
#endif
