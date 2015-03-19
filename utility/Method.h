// ----------------------------------------------------------------------------
// Method.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_DEVICE_METHOD_H_
#define _MODULAR_DEVICE_METHOD_H_
#include "Streaming.h"
#include "Array.h"
#include "JsonGenerator.h"
#include "Flash.h"
#include "Parameter.h"
#include "Constants.h"


namespace ModularDevice
{
class Server;

typedef void (*Callback)(void);
typedef void (Server::*ReservedCallback)(void);

class Method
{
public:
  Method();
  Method(const _FLASH_STRING &name);
  void setName(const _FLASH_STRING &name);
  void attachCallback(Callback callback);
  void addParameter(Parameter &parameter);
private:
  const _FLASH_STRING *name_ptr_;
  Callback callback_;
  boolean callback_attached_;
  boolean compareName(const char *name_to_compare);
  boolean compareName(const _FLASH_STRING &name_to_compare);
  const _FLASH_STRING* getNamePointer();
  void callback();
  ReservedCallback reserved_callback_;
  boolean reserved_;
  void attachReservedCallback(ReservedCallback callback);
  boolean isReserved();
  void reservedCallback(Server *server);
  Array<Parameter*,constants::METHOD_PARAMETER_COUNT_MAX> parameter_ptr_array_;
  int findParameterIndex(const _FLASH_STRING &parameter_name);
  int parameter_count_;
  friend class Server;
};
}
#endif
