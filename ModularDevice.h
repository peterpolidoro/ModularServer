// ----------------------------------------------------------------------------
// ModularDevice.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef MODULAR_DEVICE_H
#define MODULAR_DEVICE_H
#include "utility/Server.h"


namespace ModularDevice
{
class Method;
void getMemoryFreeCallback();
void resetDefaultsCallback();
void setSerialNumberCallback();

class ModularDevice
{
public:
  ModularDevice(HardwareSerial &serial);
  void setServerSerial(HardwareSerial &serial);
  void setName(const _FLASH_STRING &device_name);
  void setModelNumber(const unsigned int model_number);
  void setFirmwareNumber(const unsigned int firmware_number);
  Method& createMethod(const _FLASH_STRING &method_name);
  Method& copyMethod(Method &method,const _FLASH_STRING &method_name);
  Parameter& createParameter(const _FLASH_STRING &parameter_name);
  Parameter& copyParameter(Parameter &parameter,const _FLASH_STRING &parameter_name);
  ArduinoJson::Parser::JsonValue getParameterValue(const _FLASH_STRING &parameter_name);
  template<typename T>
  void createSavedVariable(const _FLASH_STRING &saved_variable_name, const T &default_value)
  {
    server_.createSavedVariable(saved_variable_name,default_value);
  }
  template<typename T>
  void setSavedVariableValue(const _FLASH_STRING &saved_variable_name, T value)
  {
    server_.setSavedVariableValue(saved_variable_name,value);
  }
  template<typename T>
  void getSavedVariableValue(const _FLASH_STRING &saved_variable_name, T &value)
  {
    server_.getSavedVariableValue(saved_variable_name,value);
  }
  template<typename T>
  void addToResponse(const char *key, T value)
  {
    server_.addToResponse(key,value);
  }
  template<typename T>
  void addToResponse(T value)
  {
    server_.addToResponse(value);
  }
  void addNullToResponse(const char *key);
  void addNullToResponse();
  void addBooleanToResponse(const char *key, const boolean value);
  void addBooleanToResponse(const boolean value);
  void addKeyToResponse(const char *key);
  void startResponseObject();
  void stopResponseObject();
  void startResponseArray();
  void stopResponseArray();
  void startServer(const int baudrate);
  void handleServerRequests();
private:
  Server server_;
  void resetDefaults();
  void setSerialNumber(const unsigned int serial_number);
  friend void resetDefaultsCallback();
  friend void setSerialNumberCallback();
};
}

extern ModularDevice::ModularDevice modular_device;

#endif
