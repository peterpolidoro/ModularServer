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

#ifdef __AVR__
CONSTANT_STRING(get_memory_free_method_name,"getMemoryFree");
#endif
CONSTANT_STRING(reset_defaults_method_name,"resetDefaults");
CONSTANT_STRING(set_serial_number_method_name,"setSerialNumber");
CONSTANT_STRING(serial_number_parameter_name,"serial_number");

ModularDevice::ModularDevice(GenericSerial &serial) :
  server_(serial)
{
#ifdef __AVR__
  Method& get_memory_free_method = createMethod(get_memory_free_method_name);
  get_memory_free_method.attachCallback(getMemoryFreeCallback);
#endif

  Method& reset_defaults_method = createMethod(reset_defaults_method_name);
  reset_defaults_method.attachCallback(resetDefaultsCallback);

  Method& set_serial_number_method = createMethod(set_serial_number_method_name);
  set_serial_number_method.attachCallback(setSerialNumberCallback);

  Parameter& serial_number_parameter = modular_device.createParameter(serial_number_parameter_name);
  serial_number_parameter.setRange(constants::serial_number_min,constants::serial_number_max);
  set_serial_number_method.addParameter(serial_number_parameter);
}

void ModularDevice::setServerSerial(GenericSerial &serial)
{
  server_.setSerial(serial);
}

void ModularDevice::setName(const ConstantString &device_name)
{
  server_.setName(device_name);
}

void ModularDevice::setModelNumber(const unsigned int model_number)
{
  server_.setModelNumber(model_number);
}

void ModularDevice::setFirmwareVersion(const unsigned char firmware_major,const unsigned char firmware_minor,const unsigned char firmware_patch)
{
  server_.setFirmwareVersion(firmware_major,firmware_minor,firmware_patch);
}

Method& ModularDevice::createMethod(const ConstantString &method_name)
{
  return server_.createMethod(method_name);
}

Method& ModularDevice::copyMethod(Method &method,const ConstantString &method_name)
{
  return server_.copyMethod(method,method_name);
}

Parameter& ModularDevice::createParameter(const ConstantString &parameter_name)
{
  return server_.createParameter(parameter_name);
}

Parameter& ModularDevice::copyParameter(Parameter &parameter,const ConstantString &parameter_name)
{
  return server_.copyParameter(parameter,parameter_name);
}

Parser::JsonValue ModularDevice::getParameterValue(const ConstantString &parameter_name)
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

GenericSerial generic_serial(Serial);
ModularDevice::ModularDevice modular_device(generic_serial);

#ifdef __AVR__
void ModularDevice::getMemoryFreeCallback()
{
  modular_device.addToResponse("memory_free", freeMemory());
}
#endif

void ModularDevice::resetDefaultsCallback()
{
  modular_device.resetDefaults();
}

void ModularDevice::setSerialNumberCallback()
{
  unsigned int serial_number = (long)modular_device.getParameterValue(serial_number_parameter_name);
  modular_device.setSerialNumber(serial_number);
}
