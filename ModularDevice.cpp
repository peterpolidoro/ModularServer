// ----------------------------------------------------------------------------
// ModularDevice.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "ModularDevice.h"


using namespace ArduinoJson;

namespace ModularDevice
{

FLASH_STRING(get_memory_free_method_name,"getMemoryFree");
FLASH_STRING(reset_defaults_method_name,"resetDefaults");
FLASH_STRING(set_serial_number_method_name,"setSerialNumber");
FLASH_STRING(serial_number_parameter_name,"serial_number");

ModularDevice::ModularDevice(HardwareSerial &serial) :
  server_(serial)
{
  Method& get_memory_free_method = createMethod(get_memory_free_method_name);
  get_memory_free_method.attachCallback(getMemoryFreeCallback);

  Method& reset_defaults_method = createMethod(reset_defaults_method_name);
  reset_defaults_method.attachCallback(resetDefaultsCallback);

  Method& set_serial_number_method = createMethod(set_serial_number_method_name);
  set_serial_number_method.attachCallback(setSerialNumberCallback);
  Parameter& serial_number_parameter = modular_device.createParameter(serial_number_parameter_name);
  serial_number_parameter.setRange(SERIAL_NUMBER_MIN,SERIAL_NUMBER_MAX);
  set_serial_number_method.addParameter(serial_number_parameter);
}

void ModularDevice::setServerSerial(HardwareSerial &serial)
{
  server_.setSerial(serial);
}

void ModularDevice::setName(const _FLASH_STRING &device_name)
{
  server_.setName(device_name);
}

void ModularDevice::setModelNumber(const unsigned int model_number)
{
  server_.setModelNumber(model_number);
}

void ModularDevice::setFirmwareNumber(const unsigned int firmware_number)
{
  server_.setFirmwareNumber(firmware_number);
}

Method& ModularDevice::createMethod(const _FLASH_STRING &method_name)
{
  return server_.createMethod(method_name);
}

Method& ModularDevice::copyMethod(Method &method,const _FLASH_STRING &method_name)
{
  return server_.copyMethod(method,method_name);
}

Parameter& ModularDevice::createParameter(const _FLASH_STRING &parameter_name)
{
  return server_.createParameter(parameter_name);
}

Parameter& ModularDevice::copyParameter(Parameter &parameter,const _FLASH_STRING &parameter_name)
{
  return server_.copyParameter(parameter,parameter_name);
}

Parser::JsonValue ModularDevice::getParameterValue(const _FLASH_STRING &parameter_name)
{
  return server_.getParameterValue(parameter_name);
}

void ModularDevice::addNullToResponse(const char *key)
{
  server_.addNullToResponse(key);
}

void ModularDevice::addNullToResponse()
{
  server_.addNullToResponse();
}

void ModularDevice::addBooleanToResponse(const char *key, const boolean value)
{
  server_.addBooleanToResponse(key,value);
}

void ModularDevice::addBooleanToResponse(const boolean value)
{
  server_.addBooleanToResponse(value);
}

void ModularDevice::addKeyToResponse(const char *key)
{
  server_.addKeyToResponse(key);
}

void ModularDevice::startResponseObject()
{
  server_.startResponseObject();
}

void ModularDevice::stopResponseObject()
{
  server_.stopResponseObject();
}

void ModularDevice::startResponseArray()
{
  server_.startResponseArray();
}

void ModularDevice::stopResponseArray()
{
  server_.stopResponseArray();
}

void ModularDevice::startServer(const int baudrate)
{
  server_.startServer(baudrate);
}

void ModularDevice::handleServerRequests()
{
  server_.handleRequest();
}

void ModularDevice::resetDefaults()
{
  server_.resetDefaults();
}

void ModularDevice::setSerialNumber(const unsigned int serial_number)
{
  server_.setSerialNumber(serial_number);
}
}

ModularDevice::ModularDevice modular_device(Serial);

void ModularDevice::getMemoryFreeCallback()
{
  modular_device.addToResponse("memory_free", freeMemory());
}

void ModularDevice::resetDefaultsCallback()
{
  modular_device.resetDefaults();
}

void ModularDevice::setSerialNumberCallback()
{
  unsigned int serial_number = (long)modular_device.getParameterValue(serial_number_parameter_name);
  modular_device.setSerialNumber(serial_number);
}
