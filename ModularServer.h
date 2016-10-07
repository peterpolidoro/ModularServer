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


namespace modular_server
{
class Method;
using FirmwareInfo = constants::FirmwareInfo;
using HardwareInfo = constants::HardwareInfo;
using SubsetMemberType = constants::SubsetMemberType;

class ModularServer
{
public:
  ModularServer();

  // Streams
  void addServerStream(Stream & stream);

  // Device ID
  void setDeviceName(const ConstantString & device_name);
  void setFormFactor(const ConstantString & form_factor);

  // Hardware Info
  void addHardwareInfo(const HardwareInfo & hardware_info);

  // Firmware
  template <size_t FIELDS_MAX_SIZE,
            size_t PARAMETERS_MAX_SIZE,
            size_t METHODS_MAX_SIZE>
  void addFirmware(const FirmwareInfo & firmware_info,
                   Field (&fields)[FIELDS_MAX_SIZE],
                   Parameter (&parameters)[PARAMETERS_MAX_SIZE],
                   Method (&methods)[METHODS_MAX_SIZE]);

  // Fields
  template <typename T>
  Field & createField(const ConstantString & field_name,
                     const T & default_value);
  template <typename T, size_t N>
  Field & createField(const ConstantString & field_name,
                     const T (&default_value)[N]);
  Field & getField(const ConstantString & field_name);
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
  template <typename T>
  Field & setFieldDefaultValue(const ConstantString & field_name,
                               const T & default_value);
  size_t getFieldArrayLength(const ConstantString & field_name);
  size_t getFieldStringLength(const ConstantString & field_name);
  void setFieldsToDefaults();

  // Parameters
  Parameter & createParameter(const ConstantString & parameter_name);
  Parameter & getParameter(const ConstantString & parameter_name);
  Parameter & copyParameter(Parameter & parameter,const ConstantString & parameter_name);
  ArduinoJson::JsonVariant getParameterValue(const ConstantString & parameter_name);

  // Methods
  Method & createMethod(const ConstantString & method_name);
  Method & getMethod(const ConstantString & method_name);
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
