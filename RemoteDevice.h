// ----------------------------------------------------------------------------
// RemoteDevice.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef REMOTE_DEVICE_H
#define REMOTE_DEVICE_H
#include "utility/Server.h"


namespace RemoteDevice
{
class Method;

class RemoteDevice
{
public:
  RemoteDevice(Stream &stream);
  void setServerStream(Stream &stream);
  void handleServerRequests();
  void addMethod(Method method);
  void setName(_FLASH_STRING& name);
  void setModelNumber(int model_number);
  void setFirmwareNumber(int firmware_number);
  ArduinoJson::Parser::JsonValue getParameter(_FLASH_STRING& name);
  template<typename T>
  void addToResponse(const char* key, T value)
  {
    server_.response[key] = value;
  }
private:
  Server server_;
};
}

extern RemoteDevice::RemoteDevice remote_device;

void getMemoryFreeCallback();

#endif
