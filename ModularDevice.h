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
  void setName(const ConstantString &device_name);
  void setModelNumber(const unsigned int model_number);
  void setFirmwareNumber(const unsigned int firmware_number);
  Method& createMethod(const ConstantString &method_name);
  Method& copyMethod(Method &method,const ConstantString &method_name);
  Parameter& createParameter(const ConstantString &parameter_name);
  Parameter& copyParameter(Parameter &parameter,const ConstantString &parameter_name);
  ArduinoJson::Parser::JsonValue getParameterValue(const ConstantString &parameter_name);
  template<typename T>
  SavedVariable& createSavedVariable(const ConstantString &saved_variable_name,
                                     const T &default_value);
  template<typename T>
  SavedVariable& createSavedVariable(const ConstantString &saved_variable_name,
                                     const T default_value[],
                                     const unsigned int array_length);
  template<typename T>
  void setSavedVariableValue(const ConstantString &saved_variable_name,
                             const T &value);
  template<typename T>
  void setSavedVariableValue(const ConstantString &saved_variable_name,
                             const T value[],
                             const unsigned int array_index);
  template<typename T>
  void getSavedVariableValue(const ConstantString &saved_variable_name,
                             T &value);
  template<typename T>
  void getSavedVariableValue(const ConstantString &saved_variable_name,
                             T value[],
                             const unsigned int array_index);
  template<typename T>
  void addToResponse(const char *key, T value);
  template<typename T>
  void addToResponse(T value);
  void addNullToResponse(const char *key);
  void addNullToResponse();
  void addBooleanToResponse(const char *key, const boolean value);
  void addBooleanToResponse(const boolean value);
  void addKeyToResponse(const char *key);
  template<typename T>
  void sendErrorResponse(T error);
  void startResponseObject();
  void stopResponseObject();
  void startResponseArray();
  void stopResponseArray();
  void startServer(const int baudrate);
  void handleServerRequests();
  void resetDefaults();
  void setSerialNumber(const unsigned int serial_number);
private:
  Server server_;
};
}
#include "ModularDeviceDefinitions.h"

extern ModularDevice::ModularDevice modular_device;

#endif
