// ----------------------------------------------------------------------------
// ModularServer.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef MODULAR_SERVER_H
#define MODULAR_SERVER_H
#include "utility/Server.h"


namespace ModularDevice
{
class Method;

class ModularServer
{
public:
  ModularServer();
  ModularServer(Stream &stream);
  void addServerStream(Stream &stream);
  void setName(const ConstantString &device_name);
  void setModelNumber(const unsigned int model_number);
  void setFirmwareVersion(const unsigned char firmware_major,const unsigned char firmware_minor,const unsigned char firmware_patch);
  template <size_t MAX_SIZE>
  void setMethodStorage(Method (&methods)[MAX_SIZE]);
  Method& createMethod(const ConstantString &method_name);
  Method& copyMethod(Method &method,const ConstantString &method_name);
  template <size_t MAX_SIZE>
  void setParameterStorage(Parameter (&parameters)[MAX_SIZE]);
  Parameter& createParameter(const ConstantString &parameter_name);
  Parameter& copyParameter(Parameter &parameter,const ConstantString &parameter_name);
  ArduinoJson::JsonVariant getParameterValue(const ConstantString &parameter_name);
  template <size_t MAX_SIZE>
  void setFieldStorage(Field (&fields)[MAX_SIZE]);
  template<typename T>
  Field& createField(const ConstantString &field_name,
                     const T &default_value);
  template<typename T>
  Field& createField(const ConstantString &field_name,
                     const T default_value[],
                     const unsigned int array_length);
  template<typename T>
  void setFieldValue(const ConstantString &field_name,
                     const T &value);
  template<typename T>
  void setFieldElementValue(const ConstantString &field_name,
                            const T &value,
                            const unsigned int element_index);
  template<typename T>
  void getFieldValue(const ConstantString &field_name,
                     T &value);
  template<typename T>
  void getFieldElementValue(const ConstantString &field_name,
                            T &value,
                            const unsigned int element_index);
  template<typename K>
  void writeKeyToResponse(K key);
  template<typename T>
  void writeToResponse(T value);
  template<typename K, typename T>
  void writeToResponse(K key, T value);
  void writeNullToResponse();
  template<typename K>
  void writeNullToResponse(K key);
  template<typename T>
  void sendErrorResponse(T error);
  void writeResultKeyToResponse();
  template<typename T>
  void writeResultToResponse(T value);
  void beginResponseObject();
  void endResponseObject();
  void beginResponseArray();
  void endResponseArray();
  void startServer();
  void stopServer();
  void handleServerRequests();
  void resetDefaults();
  void setSerialNumber(const unsigned int serial_number);
private:
  Server server_;
};
}
#include "ModularServerDefinitions.h"

#endif
