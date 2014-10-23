// ----------------------------------------------------------------------------
// Method.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef METHOD_H
#define METHOD_H
#include "Streaming.h"
#include "StandardCplusplus.h"
#include "vector"
#include "JsonGenerator.h"
#include "Flash.h"
#include "Parameter.h"
#include "Constants.h"


namespace RemoteDevice
{
class Server;

typedef void (*Callback)(void);
typedef void (Server::*ReservedCallback)(void);

class Method
{
public:
  Method(_FLASH_STRING& name);
  void setName(_FLASH_STRING& name);
  void attachCallback(Callback callback);
  void addParameter(Parameter &parameter);
private:
  _FLASH_STRING *name_ptr_;
  Callback callback_;
  boolean callback_attached_;
  boolean compareName(const char *name_to_compare);
  boolean compareName(_FLASH_STRING& name_to_compare);
  _FLASH_STRING* getNamePointer();
  void callback();
  ReservedCallback reserved_callback_;
  boolean reserved_;
  void attachReservedCallback(ReservedCallback callback);
  boolean isReserved();
  void reservedCallback(Server *server);
  std::vector<Parameter*> parameter_ptr_vector_;
  int findParameterIndex(_FLASH_STRING& parameter_name);
  int parameter_count_;
  friend class Server;
};
}
#endif
