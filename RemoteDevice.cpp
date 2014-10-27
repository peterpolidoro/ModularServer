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
RemoteDevice::RemoteDevice(Stream &stream) :
  server_(stream)
{
  Method& get_memory_free_method = createMethod(get_memory_free_method_name);
  get_memory_free_method.attachCallback(getMemoryFreeCallback);
}

void RemoteDevice::setServerStream(Stream &stream)
{
  server_.setStream(stream);
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

Method& RemoteDevice::copyMethod(Method &method,_FLASH_STRING &method_name)
{
  return server_.copyMethod(method,method_name);
}

Parameter& RemoteDevice::createParameter(_FLASH_STRING &parameter_name)
{
  return server_.createParameter(parameter_name);
}

Parameter& RemoteDevice::copyParameter(Parameter &parameter,_FLASH_STRING &parameter_name)
{
  return server_.copyParameter(parameter,parameter_name);
}

Parser::JsonValue RemoteDevice::getParameterValue(_FLASH_STRING &parameter_name)
{
  return server_.getParameterValue(parameter_name);
}

void RemoteDevice::addNullToResponse(const char* key)
{
  return server_.addNullToResponse(key);
}

void RemoteDevice::addNullToResponse()
{
  return server_.addNullToResponse();
}

void RemoteDevice::addKeyToResponse(const char* key)
{
  return server_.addKeyToResponse(key);
}

void RemoteDevice::startResponseObject()
{
  return server_.startResponseObject();
}

void RemoteDevice::stopResponseObject()
{
  return server_.stopResponseObject();
}

void RemoteDevice::startResponseArray()
{
  return server_.startResponseArray();
}

void RemoteDevice::stopResponseArray()
{
  return server_.stopResponseArray();
}
}

RemoteDevice::RemoteDevice remote_device(Serial);

void getMemoryFreeCallback()
{
  remote_device.addToResponse("memory_free", freeMemory());
}
