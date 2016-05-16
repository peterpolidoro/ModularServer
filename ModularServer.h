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
  void setModelNumber(const long model_number);
  void setFirmwareVersion(const long firmware_major,const long firmware_minor,const long firmware_patch);
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
  template <typename T>
  Field& createField(const ConstantString &field_name,
                     const T &default_value);
  template <typename T, size_t N>
  Field& createField(const ConstantString &field_name,
                     const T (&default_value)[N]);
  template <typename T>
  void setFieldValue(const ConstantString &field_name,
                     const T &value);
  void setFieldValue(const ConstantString &field_name,
                     ArduinoJson::JsonArray &value);
  template <typename T>
  void setFieldValue(const ConstantString &field_name,
                     const T *value,
                     const size_t N);
  template <typename T>
  void setFieldElementValue(const ConstantString &field_name,
                            const unsigned int element_index,
                            const T &value);
  template <typename T>
  void getFieldValue(const ConstantString &field_name,
                     T &value);
  template <typename T>
  void getFieldValue(const ConstantString &field_name,
                     T *value,
                     const size_t N);
  template <typename T>
  void getFieldElementValue(const ConstantString &field_name,
                            const unsigned int element_index,
                            T &value);
  template <typename T>
  void getFieldDefaultValue(const ConstantString &field_name,
                            T &value);
  template <typename T>
  void getFieldDefaultValue(const ConstantString &field_name,
                            T *value,
                            const size_t N);
  template <typename T>
  void getFieldDefaultElementValue(const ConstantString &field_name,
                                   const unsigned int element_index,
                                   T &value);
  unsigned int getFieldArrayLength(const ConstantString &field_name);
  template <typename K>
  void writeKeyToResponse(K key);
  template <typename T>
  void writeToResponse(T value);
  template <typename T, size_t N>
  void writeToResponse(T (&value)[N]);
  template <typename K, typename T>
  void writeToResponse(K key, T value);
  template <typename T>
  void writeToResponse(T *value, size_t N);
  template <typename K, typename T, size_t N>
  void writeToResponse(K key, T (&value)[N]);
  template <typename K, typename T>
  void writeToResponse(K key, T *value, size_t N);
  void writeNullToResponse();
  template <typename K>
  void writeNullToResponse(K key);
  template <typename T>
  void sendErrorResponse(T error);
  void writeResultKeyToResponse();
  template <typename T>
  void writeResultToResponse(T value);
  template <typename T, size_t N>
  void writeResultToResponse(T (&value)[N]);
  template <typename T>
  void writeResultToResponse(T *value, size_t N);
  void beginResponseObject();
  void endResponseObject();
  void beginResponseArray();
  void endResponseArray();
  void startServer();
  void stopServer();
  void handleServerRequests();
  void setFieldsToDefaults();
private:
  Server server_;
};
}
#include "ModularServerDefinitions.h"

#endif
