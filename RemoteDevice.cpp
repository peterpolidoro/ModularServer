// ----------------------------------------------------------------------------
// RemoteDevice.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "RemoteDevice.h"


using namespace ArduinoJson;

namespace RemoteDevice
{
FLASH_STRING(get_memory_free_method_name,"getMemoryFree");
RemoteDevice::RemoteDevice(Stream &stream)
{
  server_ = Server(stream);

  Method get_memory_free_method(get_memory_free_method_name);
  get_memory_free_method.attachCallback(getMemoryFreeCallback);
  addMethod(get_memory_free_method);
}

void RemoteDevice::setServerStream(Stream &stream)
{
  server_.setRequestStream(stream);
}

void RemoteDevice::handleServerRequests()
{
  server_.handleRequest();
}

void RemoteDevice::addMethod(Method method)
{
  server_.addMethod(method);
}

void RemoteDevice::setName(_FLASH_STRING& name)
{
  server_.setName(name);
}

void RemoteDevice::setModelNumber(int model_number)
{
  server_.setModelNumber(model_number);
}

void RemoteDevice::setFirmwareNumber(int firmware_number)
{
  server_.setFirmwareNumber(firmware_number);
}

Parser::JsonValue RemoteDevice::getParameter(_FLASH_STRING& name)
{
  return server_.getParameter(name);
}
}

RemoteDevice::RemoteDevice remote_device(Serial);

void getMemoryFreeCallback()
{
  remote_device.addToResponse("memory_free", freeMemory());
}
