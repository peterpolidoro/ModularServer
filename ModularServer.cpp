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

void ModularServer::setup()
{
  server_.setup();
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

// Properties
Property & ModularServer::property(const ConstantString & property_name)
{
  return server_.property(property_name);
}

void ModularServer::setPropertiesToDefaults()
{
  server_.setPropertiesToDefaults();
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

// Callbacks
Callback & ModularServer::createCallback(const ConstantString & callback_name)
{
  return server_.createCallback(callback_name);
}

Callback & ModularServer::callback(const ConstantString & callback_name)
{
  return server_.callback(callback_name);
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
