// ----------------------------------------------------------------------------
// Command.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _COMMAND_H_
#define _COMMAND_H_
#include "Streaming.h"
#include "StandardCplusplus.h"
#include "vector"
#include "Parameter.h"

const int COMMAND_NAME_LENGTH_MAX = 32;

class DeviceInterface;

typedef void (*Callback)(void);
typedef void (DeviceInterface::*ReservedCallback)(void);

class Command
{
public:
  Command(char *name);
  void setName(char *name);
  void attachCallback(Callback callback);
  void addParameter(Parameter parameter);
private:
  char name_[COMMAND_NAME_LENGTH_MAX];
  Callback callback_;
  boolean callback_attached_;
  boolean compareName(char *name_to_compare);
  char* getName();
  void printName();
  void callback();
  ReservedCallback reserved_callback_;
  boolean reserved_;
  void attachReservedCallback(ReservedCallback callback);
  boolean isReserved();
  void reservedCallback(DeviceInterface *dev_int);
  std::vector<Parameter> parameter_vector_;
  int getParameterIndex(char *parameter_name);
  int parameter_count_;
  friend class DeviceInterface;
};

#endif
