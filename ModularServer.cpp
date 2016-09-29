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
ModularServer::ModularServer()
{
}

// Stream
void ModularServer::addServerStream(Stream & stream)
{
  server_.addServerStream(stream);
}

// Device Info
void ModularServer::setDeviceName(const ConstantString & device_name)
{
  server_.setDeviceName(device_name);
}

void ModularServer::setFormFactor(const ConstantString & form_factor)
{
  server_.setFormFactor(form_factor);
}

void ModularServer::addFirmwareInfo(const FirmwareInfo & firmware_info)
{
  server_.addFirmwareInfo(firmware_info);
}

void ModularServer::addHardwareInfo(const HardwareInfo & hardware_info)
{
  server_.addHardwareInfo(hardware_info);
}

// Storage

// Field
bool ModularServer::setFieldValue(const ConstantString & field_name,
                                  ArduinoJson::JsonArray & value)
{
  return server_.setFieldValue(field_name,value);
}

size_t ModularServer::getFieldArrayLength(const ConstantString & field_name)
{
  return server_.getFieldArrayLength(field_name);
}

size_t ModularServer::getFieldStringLength(const ConstantString & field_name)
{
  return server_.getFieldStringLength(field_name);
}

void ModularServer::setFieldsToDefaults()
{
  server_.setFieldsToDefaults();
}

// Parameter
Parameter & ModularServer::createParameter(const ConstantString & parameter_name)
{
  return server_.createParameter(parameter_name);
}

Parameter & ModularServer::copyParameter(Parameter & parameter,const ConstantString & parameter_name)
{
  return server_.copyParameter(parameter,parameter_name);
}

ArduinoJson::JsonVariant ModularServer::getParameterValue(const ConstantString & parameter_name)
{
  return server_.getParameterValue(parameter_name);
}

// Method
Method & ModularServer::createMethod(const ConstantString & method_name)
{
  return server_.createMethod(method_name);
}

Method & ModularServer::copyMethod(Method & method,const ConstantString & method_name)
{
  return server_.copyMethod(method,method_name);
}

// Response
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

// Server
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
}
