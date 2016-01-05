// ----------------------------------------------------------------------------
// Server.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_DEVICE_SERVER_H_
#define _MODULAR_DEVICE_SERVER_H_
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Streaming.h"
#include "ArduinoJson.h"
#include "JsonSanitizer.h"
#include "Array.h"
#include "Vector.h"
#include "MemoryFree.h"
#include "ConstantVariable.h"
#include "Parameter.h"
#include "Method.h"
#include "SavedVariable.h"
#include "Constants.h"
#include "JsonStream.h"


namespace ModularDevice
{
class Server
{
public:
  Server(Stream &stream);
  void addServerStream(Stream &stream);
  void setName(const ConstantString &name);
  void setModelNumber(const unsigned int model_number);
  void setSerialNumber(const unsigned int serial_number);
  void setFirmwareVersion(const unsigned char firmware_major,const unsigned char firmware_minor,const unsigned char firmware_patch);
  template <size_t MAX_SIZE>
  void setMethodStorageArray(Method (&methods)[MAX_SIZE]);
  Method& createMethod(const ConstantString &method_name);
  Method& copyMethod(Method method,const ConstantString &method_name);
  template <size_t MAX_SIZE>
  void setParameterStorageArray(Parameter (&parameters)[MAX_SIZE]);
  Parameter& createParameter(const ConstantString &parameter_name);
  Parameter& copyParameter(Parameter parameter,const ConstantString &parameter_name);
  ArduinoJson::JsonVariant getParameterValue(const ConstantString &name);
  template <size_t MAX_SIZE>
  void setSavedVariableStorageArray(SavedVariable (&saved_variables)[MAX_SIZE]);
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
  void resetDefaults();
  void startServer();
  void stopServer();
  void handleRequest();
private:
  Array<Stream*,constants::SERVER_STREAM_COUNT_MAX> server_stream_ptrs_;
  unsigned char server_stream_index_;
  char request_[constants::STRING_LENGTH_REQUEST];
  ArduinoJson::JsonArray *request_json_array_ptr_;
  Array<InternalMethod,constants::INTERNAL_METHOD_COUNT_MAX> internal_methods_;
  Array<Parameter,constants::INTERNAL_PARAMETER_COUNT_MAX> internal_parameters_;
  Array<SavedVariable,constants::INTERNAL_SAVED_VARIABLE_COUNT_MAX> internal_saved_variables_;
  Vector<Method> external_methods_;
  Vector<Parameter> external_parameters_;
  Vector<SavedVariable> external_saved_variables_;
  const ConstantString *name_ptr_;
  unsigned int model_number_;
  unsigned char firmware_major_;
  unsigned char firmware_minor_;
  unsigned char firmware_patch_;
  int request_method_index_;
  int parameter_count_;
  JsonStream json_stream_;
  bool error_;
  bool result_in_response_;
  unsigned int eeprom_index_;
  const ConstantString *eeprom_init_name_ptr_;
  bool eeprom_uninitialized_;
  unsigned int eeprom_initialized_index_;
  bool server_running_;

  InternalMethod& createInternalMethod(const ConstantString &method_name, bool is_private=false);
  Parameter& createInternalParameter(const ConstantString &parameter_name);
  template<typename T>
  SavedVariable& createInternalSavedVariable(const ConstantString &saved_variable_name,
                                             const T &default_value);
  void processRequestArray();
  int processMethodString(const char *method_string);
  template<typename T>
  int findMethodIndex(T method_name);
  int countJsonArrayElements(ArduinoJson::JsonArray &json_array);
  void executeMethod();
  void methodHelp(bool verbose, int method_index);
  int processParameterString(const char *parameter_string);
  template<typename T>
  int findParameterIndex(T parameter_name);
  // int findParameterIndex(const char *parameter_name);
  // int findParameterIndex(const ConstantString &parameter_name);
  template<typename T>
  int findMethodParameterIndex(int method_index, T parameter_name);
  // int findMethodParameterIndex(int method_index, const char *parameter_name);
  // int findMethodParameterIndex(int method_index, const ConstantString &parameter_name);
  void parameterHelp(Parameter &parameter);
  bool checkParameters();
  bool checkParameter(int parameter_index, ArduinoJson::JsonVariant &json_value);
  int findSavedVariableIndex(const ConstantString &saved_variable_name);
  unsigned int getSerialNumber();
  void initializeEeprom();
  void incrementServerStream();
  void help(bool verbose);
  void writeDeviceInfoToResponse();

  // internal methods
  void getDeviceInfoCallback();
  void getMethodIdsCallback();
  void getResponseCodesCallback();
  void getParametersCallback();
  void helpCallback();
  void verboseHelpCallback();
  void getMemoryFreeCallback();
  void resetDefaultsCallback();
  void setSerialNumberCallback();
};
}
#include "ServerDefinitions.h"

#endif
