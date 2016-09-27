// ----------------------------------------------------------------------------
// Server.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_DEVICE_SERVER_H_
#define _MODULAR_DEVICE_SERVER_H_
#include "Arduino.h"
#include "Streaming.h"
#include "ArduinoJson.h"
#include "JsonSanitizer.h"
#include "Array.h"
#include "Vector.h"
#include "ConcatenatedArray.h"
#include "MemoryFree.h"
#include "ConstantVariable.h"
#include "SavedVariable.h"
#include "JsonStream.h"
#include "Functor.h"

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
  template <typename T, size_t N>
  bool setFieldValue(const ConstantString & field_name,
                     const T (&value)[N]);
  template <typename T>
  bool setFieldValue(const ConstantString & field_name,
                     const T * value,
                     const size_t N);
  template <typename T>
  bool setFieldElementValue(const ConstantString & field_name,
                            const size_t element_index,
                            const T & value);
  template <typename T>
  bool setAllFieldElementValues(const ConstantString & field_name,
                                const T & value);
  template <typename T>
  bool getFieldValue(const ConstantString & field_name,
                     T & value);
  template <typename T, size_t N>
  bool getFieldValue(const ConstantString & field_name,
                     T (&value)[N]);
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
  template <typename T, size_t N>
  bool getFieldDefaultValue(const ConstantString & field_name,
                            T (&value)[N]);
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
  Parameter & copyParameter(Parameter parameter,const ConstantString & parameter_name);
  ArduinoJson::JsonVariant getParameterValue(const ConstantString & parameter_name);

  // Method
  template <size_t MAX_SIZE>
  void addMethodStorage(Method (&methods)[MAX_SIZE]);
  Method & createMethod(const ConstantString & method_name);
  Method & copyMethod(Method method,const ConstantString & method_name);

  // Response
  template <typename K>
  void writeKeyToResponse(K key);
  template <typename T>
  void writeToResponse(T value);
  template <typename T, size_t N>
  void writeToResponse(T (&value)[N]);
  void writeToResponse(Vector<const constants::SubsetMemberType> & value, JsonStream::JsonTypes type);
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
  void handleRequest();

private:
  Array<Stream *,constants::SERVER_STREAM_COUNT_MAX> server_stream_ptrs_;
  size_t server_stream_index_;
  char request_[constants::STRING_LENGTH_REQUEST];
  ArduinoJson::JsonArray  * request_json_array_ptr_;
  Field server_fields_[constants::SERVER_FIELD_COUNT_MAX];
  Parameter server_parameters_[constants::SERVER_PARAMETER_COUNT_MAX];
  Method server_methods_[constants::SERVER_METHOD_COUNT_MAX];
  ConcatenatedArray<Field,constants::STORAGE_ARRAY_COUNT_MAX> fields_;
  ConcatenatedArray<Parameter,constants::STORAGE_ARRAY_COUNT_MAX> parameters_;
  ConcatenatedArray<Method,constants::STORAGE_ARRAY_COUNT_MAX> methods_;
  int private_method_index_;
  const ConstantString * device_name_ptr_;
  long model_number_;
  const ConstantString * firmware_name_ptr_;
  long firmware_major_;
  long firmware_minor_;
  long firmware_patch_;
  const ConstantString * hardware_name_ptr_;
  long hardware_major_;
  long hardware_minor_;
  int request_method_index_;
  int parameter_count_;
  JsonStream json_stream_;
  bool error_;
  bool result_key_in_response_;
  bool eeprom_initialized_;
  SavedVariable eeprom_initialized_sv_;
  bool server_running_;

  void setup();
  void processRequestArray();
  int processMethodString(const char * method_string);
  template <typename T>
  int findMethodIndex(T const & method_name);
  int countJsonArrayElements(ArduinoJson::JsonArray & json_array);
  void executeMethod();
  void methodHelp(bool verbose, int method_index);
  int processParameterString(const char * parameter_string);
  template <typename T>
  int findParameterIndex(T const & parameter_name);
  template <typename T>
  int findMethodParameterIndex(int method_index, T const & parameter_name);
  void parameterHelp(Parameter & parameter, bool end_object=true);
  void fieldHelp(Field & field);
  bool checkParameters();
  bool checkParameter(Parameter & parameter, ArduinoJson::JsonVariant & json_value);
  bool checkArrayParameterElement(Parameter & parameter, ArduinoJson::JsonVariant & json_value);
  template <typename T>
  int findFieldIndex(T const & field_name);
  template <typename T>
  Field & findField(T const & field_name, int * field_index_ptr);
  long getSerialNumber();
  void initializeEeprom();
  void incrementServerStream();
  void help(bool verbose);
  void writeDeviceInfoToResponse();
  void writeParameterNotInSubsetErrorToResponse(Parameter & parameter, Vector<const constants::SubsetMemberType> & subset);
  void writeParameterNotInRangeErrorToResponse(Parameter & parameter, char min_str[], char max_str[]);
  void writeFieldToResponse(Field & field, bool write_key=false, bool write_default=false, int element_index=-1);
  void writeFieldErrorToResponse(const ConstantString & error);
  void subsetToString(char * destination, Vector<const constants::SubsetMemberType> & subset, const JsonStream::JsonTypes type, const size_t num);

  // Callbacks
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
