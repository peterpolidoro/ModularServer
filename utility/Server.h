// ----------------------------------------------------------------------------
// Server.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_DEVICE_SERVER_H_
#define _MODULAR_DEVICE_SERVER_H_
#ifdef ARDUINO
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#else
#include <cstddef>
#include <cstring>
#endif
#include "Streaming.h"
#include "ArduinoJson.h"
#include "JsonSanitizer.h"
#include "Array.h"
#include "Vector.h"
#include "MemoryFree.h"
#include "ConstantVariable.h"
#include "SavedVariable.h"
#include "JsonStream.h"

#include "Field.h"
#include "Parameter.h"
#include "Method.h"
#include "Constants.h"


namespace ModularDevice
{
class Server
{
public:
  Server();
  Server(Stream &stream);
  void addServerStream(Stream &stream);
  void setName(const ConstantString &name);
  void setModelNumber(const long model_number);
  void setFirmwareVersion(const long firmware_major,const long firmware_minor,const long firmware_patch);
  template <size_t MAX_SIZE>
  void setMethodStorage(Method (&methods)[MAX_SIZE]);
  Method& createMethod(const ConstantString &method_name);
  Method& copyMethod(Method method,const ConstantString &method_name);
  template <size_t MAX_SIZE>
  void setParameterStorage(Parameter (&parameters)[MAX_SIZE]);
  Parameter& createParameter(const ConstantString &parameter_name);
  Parameter& copyParameter(Parameter parameter,const ConstantString &parameter_name);
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
  template <typename T, size_t N>
  void setFieldValue(const ConstantString &field_name,
                     const T (&value)[N]);
  template <typename T>
  void setFieldValue(const ConstantString &field_name,
                     const T *value,
                     const size_t N);
  void setFieldValue(const ConstantString &field_name,
                     ArduinoJson::JsonArray &value);
  template <typename T>
  void setFieldElementValue(const ConstantString &field_name,
                            const unsigned int element_index,
                            const T &value);
  template <typename T>
  void setAllFieldElementValues(const ConstantString &field_name,
                                const T &value);
  template <typename T>
  void getFieldValue(const ConstantString &field_name,
                     T &value);
  template <typename T, size_t N>
  void getFieldValue(const ConstantString &field_name,
                     T (&value)[N]);
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
  template <typename T, size_t N>
  void getFieldDefaultValue(const ConstantString &field_name,
                            T (&value)[N]);
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
  template <typename T>
  void writeToResponse(T *value, size_t N);
  template <typename K, typename T>
  void writeToResponse(K key, T value);
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
  void setFieldsToDefaults();
  void startServer();
  void stopServer();
  void handleRequest();
private:
  Array<Stream*,constants::SERVER_STREAM_COUNT_MAX> server_stream_ptrs_;
  unsigned char server_stream_index_;
  char request_[constants::STRING_LENGTH_REQUEST];
  ArduinoJson::JsonArray *request_json_array_ptr_;
  Array<Field,constants::INTERNAL_FIELD_COUNT_MAX> internal_fields_;
  Array<Parameter,constants::INTERNAL_PARAMETER_COUNT_MAX> internal_parameters_;
  Array<InternalMethod,constants::INTERNAL_METHOD_COUNT_MAX> internal_methods_;
  Vector<Field> external_fields_;
  Vector<Parameter> external_parameters_;
  Vector<Method> external_methods_;
  const ConstantString *name_ptr_;
  long model_number_;
  long firmware_major_;
  long firmware_minor_;
  long firmware_patch_;
  int request_method_index_;
  int parameter_count_;
  JsonStream json_stream_;
  bool error_;
  bool result_key_in_response_;
  bool eeprom_initialized_;
  SavedVariable eeprom_initialized_sv_;
  bool server_running_;

  void setup();
  InternalMethod& createInternalMethod(const ConstantString &method_name, bool is_private=false);
  Parameter& createInternalParameter(const ConstantString &parameter_name);
  template <typename T>
  Field& createInternalField(const ConstantString &field_name,
                             const T &default_value);
  void processRequestArray();
  int processMethodString(const char *method_string);
  template <typename T>
  int findMethodIndex(T const&method_name);
  int countJsonArrayElements(ArduinoJson::JsonArray &json_array);
  void executeMethod();
  void methodHelp(bool verbose, int method_index);
  int processParameterString(const char *parameter_string);
  template <typename T>
  int findParameterIndex(T const&parameter_name);
  template <typename T>
  int findMethodParameterIndex(int method_index, T const&parameter_name);
  void parameterHelp(Parameter &parameter, bool end_object=true);
  void fieldHelp(Field &field);
  bool checkParameters();
  bool checkParameter(Parameter &parameter, ArduinoJson::JsonVariant &json_value);
  bool checkArrayParameterElement(Parameter &parameter, ArduinoJson::JsonVariant &json_value);
  template <typename T>
  int findFieldIndex(T const&field_name);
  template <typename T>
  Field& findField(T const&field_name, int *field_index_ptr);
  long getSerialNumber();
  void initializeEeprom();
  void incrementServerStream();
  void help(bool verbose);
  void writeDeviceInfoToResponse();
  void writeParameterOutOfRangeErrorToResponse(Parameter &parameter, char min_str[], char max_str[]);
  void writeFieldToResponse(Field &field, bool write_key=false, bool write_default=false, int element_index=-1);
  void writeFieldErrorToResponse(const ConstantString &error);

  // internal methods
  void getDeviceInfoCallback();
  void getMethodIdsCallback();
  void getResponseCodesCallback();
  void getParametersCallback();
  void helpCallback();
  void verboseHelpCallback();
  void getMemoryFreeCallback();
  void getFieldDefaultValuesCallback();
  void setFieldsToDefaultsCallback();
  void setFieldToDefaultCallback();
  void getFieldValuesCallback();
  void getFieldValueCallback();
  void getFieldElementValueCallback();
  void setFieldValueCallback();
  void setFieldElementValueCallback();
  void setAllFieldElementValuesCallback();
};
}
#include "ServerDefinitions.h"

#endif
