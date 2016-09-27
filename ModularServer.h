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
using SubsetMemberType = constants::SubsetMemberType;

class ModularServer
{
public:
  ModularServer();

  // Stream
  void addServerStream(Stream & stream);

  // Device Info
  void setDeviceName(const ConstantString & device_name);
  void setModelNumber(const long model_number);
  void setFirmwareName(const ConstantString & firmware_name);
  void setFirmwareVersion(const long firmware_major, const long firmware_minor, const long firmware_patch);
  void setHardwareName(const ConstantString & hardware_name);
  void setHardwareVersion(const long hardware_major, const long hardware_minor);

  // Field
  template <size_t MAX_SIZE>
  void addFieldStorage(Field (&fields)[MAX_SIZE]);
  template <typename T>
  Field & createField(const ConstantString & field_name,
                     const T & default_value);
  template <typename T, size_t N>
  Field & createField(const ConstantString & field_name,
                     const T (&default_value)[N]);
  template <typename T>
  bool setFieldValue(const ConstantString & field_name,
                     const T & value);
  bool setFieldValue(const ConstantString & field_name,
                     ArduinoJson::JsonArray & value);
  template <typename T>
  bool setFieldValue(const ConstantString & field_name,
                     const T * value,
                     const size_t N);
  template <typename T>
  bool setFieldElementValue(const ConstantString & field_name,
                            const size_t element_index,
                            const T & value);
  template <typename T>
  bool getFieldValue(const ConstantString & field_name,
                     T & value);
  template <typename T>
  bool getFieldValue(const ConstantString & field_name,
                     T * value,
                     const size_t N);
  template <typename T>
  bool getFieldElementValue(const ConstantString & field_name,
                            const size_t element_index,
                            T & value);
  template <typename T>
  bool getFieldDefaultValue(const ConstantString & field_name,
                            T & value);
  template <typename T>
  bool getFieldDefaultValue(const ConstantString & field_name,
                            T * value,
                            const size_t N);
  template <typename T>
  bool getFieldDefaultElementValue(const ConstantString & field_name,
                                   const size_t element_index,
                                   T & value);
  size_t getFieldArrayLength(const ConstantString & field_name);
  size_t getFieldStringLength(const ConstantString & field_name);
  void setFieldsToDefaults();

  // Parameter
  template <size_t MAX_SIZE>
  void addParameterStorage(Parameter (&parameters)[MAX_SIZE]);
  Parameter & createParameter(const ConstantString & parameter_name);
  Parameter & copyParameter(Parameter & parameter,const ConstantString & parameter_name);
  ArduinoJson::JsonVariant getParameterValue(const ConstantString & parameter_name);

  // Method
  template <size_t MAX_SIZE>
  void addMethodStorage(Method (&methods)[MAX_SIZE]);
  Method & createMethod(const ConstantString & method_name);
  Method & copyMethod(Method & method,const ConstantString & method_name);

  // Response
  template <typename K>
  void writeKeyToResponse(K key);
  template <typename T>
  void writeToResponse(T value);
  template <typename T, size_t N>
  void writeToResponse(T (&value)[N]);
  template <typename K, typename T>
  void writeToResponse(K key, T value);
  template <typename K, typename T, size_t N>
  void writeToResponse(K key, T (&value)[N]);
  template <typename T>
  void writeArrayToResponse(T * value, size_t N);
  template <typename K, typename T>
  void writeArrayToResponse(K key, T * value, size_t N);
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
  void writeResultToResponse(T * value, size_t N);
  void beginResponseObject();
  void endResponseObject();
  void beginResponseArray();
  void endResponseArray();

  // Server
  void startServer();
  void stopServer();
  void handleServerRequests();

private:
  Server server_;
};
}
#include "ModularServerDefinitions.h"

#endif
