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

#include "Property.h"
#include "Parameter.h"
#include "Method.h"
#include "Callback.h"
#include "Response.h"
#include "Constants.h"


namespace modular_server
{
class Server
{
public:
  Server();
  void setup();

  // Streams
  void addServerStream(Stream & stream);

  // Device ID
  void setDeviceName(const ConstantString & device_name);
  void setFormFactor(const ConstantString & form_factor);

  // Hardware Info
  void addHardwareInfo(const constants::HardwareInfo & hardware_info);

  // Firmware
  template <size_t PROPERTIES_MAX_SIZE,
            size_t PARAMETERS_MAX_SIZE,
            size_t METHODS_MAX_SIZE,
            size_t CALLBACKS_MAX_SIZE>
  void addFirmware(const constants::FirmwareInfo & firmware_info,
                   Property (&properties)[PROPERTIES_MAX_SIZE],
                   Parameter (&parameters)[PARAMETERS_MAX_SIZE],
                   Method (&methods)[METHODS_MAX_SIZE],
                   Callback (&callbacks)[CALLBACKS_MAX_SIZE]);

  // Properties
  template <typename T>
  Property & createProperty(const ConstantString & property_name,
                     const T & default_value);
  template <typename T, size_t N>
  Property & createProperty(const ConstantString & property_name,
                     const T (&default_value)[N]);
  Property & property(const ConstantString & property_name);
  void setPropertiesToDefaults();

  // Parameters
  Parameter & createParameter(const ConstantString & parameter_name);
  Parameter & parameter(const ConstantString & parameter_name);
  Parameter & copyParameter(Parameter parameter,const ConstantString & parameter_name);

  // Methods
  Method & createMethod(const ConstantString & method_name);
  Method & method(const ConstantString & method_name);
  Method & copyMethod(Method method,const ConstantString & method_name);

  // Callbacks
  Callback & createCallback(const ConstantString & callback_name);
  Callback & callback(const ConstantString & callback_name);

  // Response
  Response & response();

  // Server
  void startServer();
  void stopServer();
  void handleRequest();

private:
  Array<Stream *,constants::SERVER_STREAM_COUNT_MAX> server_stream_ptrs_;
  size_t server_stream_index_;
  JsonStream json_stream_;
  char request_[constants::STRING_LENGTH_REQUEST];
  ArduinoJson::JsonArray  * request_json_array_ptr_;
  Response response_;

  Property server_properties_[constants::SERVER_PROPERTY_COUNT_MAX];
  Parameter server_parameters_[constants::SERVER_PARAMETER_COUNT_MAX];
  Method server_methods_[constants::SERVER_METHOD_COUNT_MAX];
  Callback server_callbacks_[constants::SERVER_CALLBACK_COUNT_MAX];
  ConcatenatedArray<Property,constants::FIRMWARE_COUNT_MAX> properties_;
  ConcatenatedArray<Parameter,constants::FIRMWARE_COUNT_MAX> parameters_;
  ConcatenatedArray<Method,constants::FIRMWARE_COUNT_MAX> methods_;
  ConcatenatedArray<Callback,constants::FIRMWARE_COUNT_MAX> callbacks_;
  Property dummy_property_;
  Parameter dummy_parameter_;
  Method dummy_method_;
  Callback dummy_callback_;
  int private_method_index_;
  const ConstantString * device_name_ptr_;
  const ConstantString * form_factor_ptr_;
  Array<const constants::FirmwareInfo *,constants::FIRMWARE_COUNT_MAX> firmware_info_array_;
  Array<constants::SubsetMemberType,constants::FIRMWARE_COUNT_MAX+1> firmware_name_array_;
  Array<const constants::HardwareInfo *,constants::HARDWARE_INFO_ARRAY_COUNT_MAX> hardware_info_array_;

  int request_procedure_index_;
  int parameter_count_;
  bool eeprom_initialized_;
  SavedVariable eeprom_initialized_sv_;
  bool server_running_;

  ArduinoJson::JsonVariant getParameterValue(const ConstantString & parameter_name);
  void processRequestArray();
  int findRequestProcedureIndex(const char * procedure_string);
  template <typename T>
  int findPropertyIndex(T const & property_name);
  template <typename T>
  int findParameterIndex(T const & parameter_name);
  template <typename T>
  int findMethodParameterIndex(Method & method, T const & parameter_name);
  template <typename T>
  int findMethodIndex(T const & method_name);
  template <typename T>
  int findCallbackIndex(T const & callback_name);
  int countJsonArrayElements(ArduinoJson::JsonArray & json_array);
  int processParameterString(const char * parameter_string);
  bool checkParameters();
  bool checkParameter(Parameter & parameter, ArduinoJson::JsonVariant & json_value);
  bool checkArrayParameterElement(Parameter & parameter, ArduinoJson::JsonVariant & json_value);
  long getSerialNumber();
  void initializeEeprom();
  void incrementServerStream();
  void propertyHelp(Property & property, bool verbose);
  void parameterHelp(Parameter & parameter, bool end_object=true);
  void methodHelp(Method & method, bool verbose);
  void callbackHelp(Callback & callback, bool verbose);
  void help(bool verbose);
  void writeDeviceIdToResponse();
  void writeFirmwareInfoToResponse();
  void writeHardwareInfoToResponse();
  void writeDeviceInfoToResponse();
  void writeApiToResponse(bool verbose, ArduinoJson::JsonArray & firmware_name_array);
  void writePropertyToResponse(Property & property,
                            bool write_key=false,
                            bool write_default=false,
                            int element_index=-1);
  void versionToString(char * destination,
                       const long major,
                       const long minor,
                       const long patch,
                       const size_t num);
  void subsetToString(char * destination,
                      Vector<constants::SubsetMemberType> & subset,
                      const JsonStream::JsonTypes & parameter_type,
                      const JsonStream::JsonTypes & parameter_array_element_type,
                      const size_t num);

  // Handlers
  void getProcedureIdsHandler();
  void helpHandler();
  void verboseHelpHandler();
  void getDeviceIdHandler();
  void getDeviceInfoHandler();
  void getApiHandler();
  void getApiVerboseHandler();
  void getMemoryFreeHandler();
  void getPropertyDefaultValuesHandler();
  void setPropertiesToDefaultsHandler();
  void setPropertyToDefaultHandler();
  void getPropertyValuesHandler();
  void getPropertyValueHandler();
  void getPropertyElementValueHandler();
  void setPropertyValueHandler();
  void setPropertyElementValueHandler();
  void setAllPropertyElementValuesHandler();

};
}
#include "ServerDefinitions.h"

#endif
