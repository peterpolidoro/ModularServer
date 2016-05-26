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

ModularServer::ModularServer(Stream &stream) :
  server_(stream)
{
}

void ModularServer::addServerStream(Stream &stream)
{
  server_.addServerStream(stream);
}

void ModularServer::setName(const ConstantString &device_name)
{
  server_.setName(device_name);
}

void ModularServer::setModelNumber(const long model_number)
{
  server_.setModelNumber(model_number);
}

void ModularServer::setFirmwareVersion(const long firmware_major,const long firmware_minor,const long firmware_patch)
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

size_t ModularServer::getFieldArrayLength(const ConstantString &field_name)
{
  return server_.getFieldArrayLength(field_name);
}

size_t ModularServer::getFieldStringLength(const ConstantString &field_name)
{
  return server_.getFieldStringLength(field_name);
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

bool ModularServer::setFieldValue(const ConstantString &field_name,
                                  ArduinoJson::JsonArray &value)
{
  return server_.setFieldValue(field_name,value);
}

void ModularServer::setFieldsToDefaults()
{
  server_.setFieldsToDefaults();
}
}
