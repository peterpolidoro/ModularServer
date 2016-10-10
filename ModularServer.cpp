// ----------------------------------------------------------------------------
// ModularServer.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "ModularServer.h"


namespace modular_server
{
ModularServer::ModularServer()
{
}

// Streams
void ModularServer::addServerStream(Stream & stream)
{
  server_.addServerStream(stream);
}

// Device ID
void ModularServer::setDeviceName(const ConstantString & device_name)
{
  server_.setDeviceName(device_name);
}

void ModularServer::setFormFactor(const ConstantString & form_factor)
{
  server_.setFormFactor(form_factor);
}

// Hardware Info
void ModularServer::addHardwareInfo(const HardwareInfo & hardware_info)
{
  server_.addHardwareInfo(hardware_info);
}

// Firmware

// Fields
Field & ModularServer::field(const ConstantString & field_name)
{
  return server_.field(field_name);
}

void ModularServer::setFieldsToDefaults()
{
  server_.setFieldsToDefaults();
}

// Parameters
Parameter & ModularServer::createParameter(const ConstantString & parameter_name)
{
  return server_.createParameter(parameter_name);
}

Parameter & ModularServer::parameter(const ConstantString & parameter_name)
{
  return server_.parameter(parameter_name);
}

Parameter & ModularServer::copyParameter(Parameter & parameter,const ConstantString & parameter_name)
{
  return server_.copyParameter(parameter,parameter_name);
}

// Methods
Method & ModularServer::createMethod(const ConstantString & method_name)
{
  return server_.createMethod(method_name);
}

Method & ModularServer::method(const ConstantString & method_name)
{
  return server_.method(method_name);
}

Method & ModularServer::copyMethod(Method & method,const ConstantString & method_name)
{
  return server_.copyMethod(method,method_name);
}

// Response
Response & ModularServer::response()
{
  return server_.response();
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
