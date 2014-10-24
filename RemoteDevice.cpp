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

  Method& get_memory_free_method = createMethod(get_memory_free_method_name);
  get_memory_free_method.attachCallback(getMemoryFreeCallback);
}

void RemoteDevice::setServerStream(Stream &stream)
{
  server_.setRequestStream(stream);
}

void RemoteDevice::handleServerRequests()
{
  server_.handleRequest();
}

void RemoteDevice::setName(_FLASH_STRING &device_name)
{
  server_.setName(device_name);
}

void RemoteDevice::setModelNumber(int model_number)
{
  server_.setModelNumber(model_number);
}

void RemoteDevice::setFirmwareNumber(int firmware_number)
{
  server_.setFirmwareNumber(firmware_number);
}

Method& RemoteDevice::createMethod(_FLASH_STRING &method_name)
{
  return server_.createMethod(method_name);
}

Method& RemoteDevice::createMethod(Method &method)
{
  return server_.createMethod(method);
}

Parameter& RemoteDevice::createParameter(_FLASH_STRING &parameter_name)
{
  return server_.createParameter(parameter_name);
}

Parameter& RemoteDevice::createParameter(Parameter &parameter)
{
  return server_.createParameter(parameter);
}

Parser::JsonValue RemoteDevice::getParameterValue(_FLASH_STRING &parameter_name)
{
  return server_.getParameterValue(parameter_name);
}
}

RemoteDevice::RemoteDevice remote_device(Serial);

void getMemoryFreeCallback()
{
  remote_device.addToResponse("memory_free", freeMemory());
}
