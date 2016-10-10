// ----------------------------------------------------------------------------
// Server.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_SERVER_H_
#define _MODULAR_SERVER_SERVER_H_
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
#include "Response.h"
#include "Constants.h"


namespace modular_server
{
class Server
{
public:
  Server();

  // Streams
  void addServerStream(Stream & stream);

  // Device ID
  void setDeviceName(const ConstantString & device_name);
  void setFormFactor(const ConstantString & form_factor);

  // Hardware Info
  void addHardwareInfo(const constants::HardwareInfo & hardware_info);

  // Firmware
  template <size_t FIELDS_MAX_SIZE,
            size_t PARAMETERS_MAX_SIZE,
            size_t METHODS_MAX_SIZE>
  void addFirmware(const constants::FirmwareInfo & firmware_info,
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
  Field & field(const ConstantString & field_name);
  void setFieldsToDefaults();

  // Parameters
  Parameter & createParameter(const ConstantString & parameter_name);
  Parameter & parameter(const ConstantString & parameter_name);
  Parameter & copyParameter(Parameter parameter,const ConstantString & parameter_name);

  // Methods
  Method & createMethod(const ConstantString & method_name);
  Method & method(const ConstantString & method_name);
  Method & copyMethod(Method method,const ConstantString & method_name);

  // Response
  Response & response();

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
  const ConstantString * form_factor_ptr_;
  Array<const constants::FirmwareInfo *,constants::STORAGE_ARRAY_COUNT_MAX> firmware_info_array_;
  Array<const constants::HardwareInfo *,constants::HARDWARE_INFO_ARRAY_COUNT_MAX> hardware_info_array_;

  int request_method_index_;
  int parameter_count_;
  bool error_;
  bool result_key_in_response_;
  bool eeprom_initialized_;
  SavedVariable eeprom_initialized_sv_;
  bool server_running_;

  void setup();
  ArduinoJson::JsonVariant getParameterValue(const ConstantString & parameter_name);
  void processRequestArray();
  int findRequestMethodIndex(const char * method_string);
  template <typename T>
  int findMethodIndex(T const & method_name);
  int countJsonArrayElements(ArduinoJson::JsonArray & json_array);
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
  long getSerialNumber();
  void initializeEeprom();
  void incrementServerStream();
  void help(bool verbose);
  void writeDeviceIdToResponse();
  void writeFirmwareInfoToResponse();
  void writeHardwareInfoToResponse();
  void writeDeviceInfoToResponse();
  void writeApiToResponse(bool verbose);
  void writeParameterNotInSubsetErrorToResponse(Parameter & parameter, Vector<const constants::SubsetMemberType> & subset);
  void writeParameterNotInRangeErrorToResponse(Parameter & parameter, char min_str[], char max_str[]);
  void writeFieldToResponse(Field & field, bool write_key=false, bool write_default=false, int element_index=-1);
  void writeFieldErrorToResponse(const ConstantString & error);
  void versionToString(char* destination, const long major, const long minor, const long patch=-1);
  void subsetToString(char * destination, Vector<const constants::SubsetMemberType> & subset, const JsonStream::JsonTypes type, const size_t num);

  // Callbacks
  void getMethodIdsCallback();
  void helpCallback();
  void verboseHelpCallback();
  void getDeviceIdCallback();
  void getDeviceInfoCallback();
  void getApiCallback();
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
