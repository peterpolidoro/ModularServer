// ----------------------------------------------------------------------------
// ModularServer.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
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

// Hardware
void ModularServer::removeHardware()
{
  server_.removeHardware();
}

// Pins
Pin & ModularServer::createPin(const ConstantString & pin_name,
  size_t pin_number)
{
  return server_.createPin(pin_name,pin_number);
}

Pin & ModularServer::pin(const ConstantString & pin_name)
{
  return server_.pin(pin_name);
}

// Firmware

// Properties
Property & ModularServer::property(const ConstantString & property_name)
{
  return server_.property(property_name);
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

Parameter & ModularServer::copyParameter(Parameter & parameter,
  const ConstantString & parameter_name)
{
  return server_.copyParameter(parameter,parameter_name);
}

// Functions
Function & ModularServer::createFunction(const ConstantString & function_name)
{
  return server_.createFunction(function_name);
}

Function & ModularServer::function(const ConstantString & function_name)
{
  return server_.function(function_name);
}

Function & ModularServer::copyFunction(Function & function,
  const ConstantString & function_name)
{
  return server_.copyFunction(function,function_name);
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
