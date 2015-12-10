// ----------------------------------------------------------------------------
// ModularServer.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "ModularServer.h"


namespace ModularDevice
{
ModularServer::ModularServer(Stream &stream) :
  server_(stream)
{
  Parameter& serial_number_parameter = modular_server.createParameter(constants::serial_number_constant_string);
  serial_number_parameter.setRange(constants::serial_number_min,constants::serial_number_max);

#ifdef __AVR__
  Method& get_memory_free_method = createMethod(constants::get_memory_free_method_name);
  get_memory_free_method.attachCallback(getMemoryFreeCallback);
  get_memory_free_method.setReturnTypeLong();
#endif

  Method& reset_defaults_method = createMethod(constants::reset_defaults_method_name);
  reset_defaults_method.attachCallback(resetDefaultsCallback);

  Method& set_serial_number_method = createMethod(constants::set_serial_number_method_name);
  set_serial_number_method.attachCallback(setSerialNumberCallback);
  set_serial_number_method.addParameter(serial_number_parameter);
}

void ModularServer::addServerStream(Stream &stream)
{
  server_.addServerStream(stream);
}

void ModularServer::setName(const ConstantString &device_name)
{
  server_.setName(device_name);
}

void ModularServer::setModelNumber(const unsigned int model_number)
{
  server_.setModelNumber(model_number);
}

void ModularServer::setFirmwareVersion(const unsigned char firmware_major,const unsigned char firmware_minor,const unsigned char firmware_patch)
{
  server_.setFirmwareVersion(firmware_major,firmware_minor,firmware_patch);
}

Method& ModularServer::createMethod(const ConstantString &method_name)
{
  return server_.createMethod(method_name);
}

Method& ModularServer::copyMethod(Method &method,const ConstantString &method_name)
{
  return server_.copyMethod(method,method_name);
}

Parameter& ModularServer::createParameter(const ConstantString &parameter_name)
{
  return server_.createParameter(parameter_name);
}

Parameter& ModularServer::copyParameter(Parameter &parameter,const ConstantString &parameter_name)
{
  return server_.copyParameter(parameter,parameter_name);
}

ArduinoJson::JsonVariant ModularServer::getParameterValue(const ConstantString &parameter_name)
{
  return server_.getParameterValue(parameter_name);
}

void ModularServer::writeNullToResponse()
{
  server_.writeNullToResponse();
}

void ModularServer::writeResultKeyToResponse()
{
  server_.writeResultKeyToResponse();
}

void ModularServer::beginResponseObject()
{
  server_.beginResponseObject();
}

void ModularServer::endResponseObject()
{
  server_.endResponseObject();
}

void ModularServer::beginResponseArray()
{
  server_.beginResponseArray();
}

void ModularServer::endResponseArray()
{
  server_.endResponseArray();
}

void ModularServer::startServer()
{
  server_.startServer();
}

void ModularServer::stopServer()
{
  server_.stopServer();
}

void ModularServer::handleServerRequests()
{
  server_.handleRequest();
}

void ModularServer::resetDefaults()
{
  server_.resetDefaults();
}

void ModularServer::setSerialNumber(const unsigned int serial_number)
{
  server_.setSerialNumber(serial_number);
}
}

ModularDevice::ModularServer modular_server(Serial);

#ifdef __AVR__
void ModularDevice::getMemoryFreeCallback()
{
  modular_server.writeResultToResponse(freeMemory());
}
#endif

void ModularDevice::resetDefaultsCallback()
{
  modular_server.resetDefaults();
}

void ModularDevice::setSerialNumberCallback()
{
  unsigned int serial_number = (long)modular_server.getParameterValue(constants::serial_number_constant_string);
  modular_server.setSerialNumber(serial_number);
}
