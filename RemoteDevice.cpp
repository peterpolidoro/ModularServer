// ----------------------------------------------------------------------------
// RemoteDevice.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "RemoteDevice.h"


namespace RemoteDevice
{

FLASH_STRING(get_memory_free_method_name,"getMemoryFree");
FLASH_STRING(reset_defaults_method_name,"resetDefaults");
FLASH_STRING(set_serial_number_method_name,"setSerialNumber");
FLASH_STRING(serial_number_parameter_name,"serial_number");

RemoteDevice::RemoteDevice(Stream &stream) :
  server_(stream)
{
  Method& get_memory_free_method = createMethod(get_memory_free_method_name);
  get_memory_free_method.attachCallback(getMemoryFreeCallback);

  Method& reset_defaults_method = createMethod(reset_defaults_method_name);
  reset_defaults_method.attachCallback(resetDefaultsCallback);

  Method& set_serial_number_method = createMethod(set_serial_number_method_name);
  set_serial_number_method.attachCallback(setSerialNumberCallback);
  Parameter& serial_number_parameter = remote_device.createParameter(serial_number_parameter_name);
  serial_number_parameter.setRange(SERIAL_NUMBER_MIN,SERIAL_NUMBER_MAX);
  set_serial_number_method.addParameter(serial_number_parameter);
}

void RemoteDevice::setServerStream(Stream &stream)
{
  server_.setStream(stream);
}

void RemoteDevice::setName(const _FLASH_STRING &device_name)
{
  server_.setName(device_name);
}

void RemoteDevice::setModelNumber(const unsigned int model_number)
{
  server_.setModelNumber(model_number);
}

void RemoteDevice::setFirmwareNumber(const unsigned int firmware_number)
{
  server_.setFirmwareNumber(firmware_number);
}

Method& RemoteDevice::createMethod(const _FLASH_STRING &method_name)
{
  return server_.createMethod(method_name);
}

Method& RemoteDevice::copyMethod(Method &method,const _FLASH_STRING &method_name)
{
  return server_.copyMethod(method,method_name);
}

Parameter& RemoteDevice::createParameter(const _FLASH_STRING &parameter_name)
{
  return server_.createParameter(parameter_name);
}

Parameter& RemoteDevice::copyParameter(Parameter &parameter,const _FLASH_STRING &parameter_name)
{
  return server_.copyParameter(parameter,parameter_name);
}

JsonVariant RemoteDevice::getParameterValue(const _FLASH_STRING &parameter_name)
{
  return server_.getParameterValue(parameter_name);
}

void RemoteDevice::addNullToResponse(const char *key)
{
  server_.addNullToResponse(key);
}

void RemoteDevice::addNullToResponse()
{
  server_.addNullToResponse();
}

void RemoteDevice::addBooleanToResponse(const char *key, const boolean value)
{
  server_.addBooleanToResponse(key,value);
}

void RemoteDevice::addBooleanToResponse(const boolean value)
{
  server_.addBooleanToResponse(value);
}

void RemoteDevice::addKeyToResponse(const char *key)
{
  server_.addKeyToResponse(key);
}

void RemoteDevice::startResponseObject()
{
  server_.startResponseObject();
}

void RemoteDevice::stopResponseObject()
{
  server_.stopResponseObject();
}

void RemoteDevice::startResponseArray()
{
  server_.startResponseArray();
}

void RemoteDevice::stopResponseArray()
{
  server_.stopResponseArray();
}

void RemoteDevice::startServer(const int baudrate)
{
  server_.startServer(baudrate);
}

void RemoteDevice::handleServerRequests()
{
  server_.handleRequest();
}

void RemoteDevice::resetDefaults()
{
  server_.resetDefaults();
}

void RemoteDevice::setSerialNumber(const unsigned int serial_number)
{
  server_.setSerialNumber(serial_number);
}
}

RemoteDevice::RemoteDevice remote_device(Serial);

void RemoteDevice::getMemoryFreeCallback()
{
  remote_device.addToResponse("memory_free", freeMemory());
}

void RemoteDevice::resetDefaultsCallback()
{
  remote_device.resetDefaults();
}

void RemoteDevice::setSerialNumberCallback()
{
  unsigned int serial_number = (long)remote_device.getParameterValue(serial_number_parameter_name);
  remote_device.setSerialNumber(serial_number);
}
