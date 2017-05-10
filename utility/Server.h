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
#include "Function.h"
#include "Callback.h"
#include "Response.h"
#include "Interrupt.h"
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

  // Hardware
  template <size_t INTERRUPTS_MAX_SIZE>
  void addHardware(const constants::HardwareInfo & hardware_info,
                   Interrupt (&interrupts)[INTERRUPTS_MAX_SIZE]);

  // Interrupts
  Interrupt & createInterrupt(const ConstantString & interrupt_name,
                              const size_t pin);
  Interrupt & interrupt(const ConstantString & interrupt_name);

  // Firmware
  template <size_t PROPERTIES_MAX_SIZE,
            size_t PARAMETERS_MAX_SIZE,
            size_t FUNCTIONS_MAX_SIZE,
            size_t CALLBACKS_MAX_SIZE>
  void addFirmware(const constants::FirmwareInfo & firmware_info,
                   Property (&properties)[PROPERTIES_MAX_SIZE],
                   Parameter (&parameters)[PARAMETERS_MAX_SIZE],
                   Function (&functions)[FUNCTIONS_MAX_SIZE],
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
  void setAllPropertiesToDefaults();

  // Parameters
  Parameter & createParameter(const ConstantString & parameter_name);
  Parameter & parameter(const ConstantString & parameter_name);
  Parameter & copyParameter(Parameter parameter,
                            const ConstantString & parameter_name);

  // Functions
  Function & createFunction(const ConstantString & function_name);
  Function & function(const ConstantString & function_name);
  Function & copyFunction(Function function,
                          const ConstantString & function_name);

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
  JsonStream server_json_stream_;

  char request_[constants::STRING_LENGTH_REQUEST];
  ArduinoJson::JsonArray  * request_json_array_ptr_;

  Response response_;

  Array<const constants::HardwareInfo *,constants::HARDWARE_COUNT_MAX> hardware_info_array_;
  Interrupt dummy_interrupt_;
  ConcatenatedArray<Interrupt,constants::HARDWARE_COUNT_MAX> interrupts_;
  Array<constants::SubsetMemberType,constants::INTERRUPT_COUNT_MAX> interrupt_name_array_;

  Property server_properties_[constants::SERVER_PROPERTY_COUNT_MAX];
  Parameter server_parameters_[constants::SERVER_PARAMETER_COUNT_MAX];
  Function server_functions_[constants::SERVER_FUNCTION_COUNT_MAX];
  Callback server_callbacks_[constants::SERVER_CALLBACK_COUNT_MAX];
  ConcatenatedArray<Property,constants::FIRMWARE_COUNT_MAX> properties_;
  ConcatenatedArray<Parameter,constants::FIRMWARE_COUNT_MAX> parameters_;
  ConcatenatedArray<Function,constants::FIRMWARE_COUNT_MAX> functions_;
  ConcatenatedArray<Callback,constants::FIRMWARE_COUNT_MAX> callbacks_;
  Property dummy_property_;
  Parameter dummy_parameter_;
  Function dummy_function_;
  Callback dummy_callback_;
  int private_function_index_;
  const ConstantString * device_name_ptr_;
  const ConstantString * form_factor_ptr_;
  Array<const constants::FirmwareInfo *,constants::FIRMWARE_COUNT_MAX> firmware_info_array_;
  Array<constants::SubsetMemberType,constants::FIRMWARE_COUNT_MAX+1> firmware_name_array_;

  int request_method_index_;
  int property_function_index_;
  int callback_function_index_;
  bool eeprom_initialized_;
  SavedVariable eeprom_initialized_sv_;
  bool server_running_;

  template <typename T>
  int findInterruptIndex(T const & interrupt_name);
  ArduinoJson::JsonVariant getParameterValue(const ConstantString & parameter_name);
  void processRequestArray();
  int findMethodIndex(const char * method_string);
  template <typename T>
  int findPropertyIndex(T const & property_name);
  template <typename T>
  int findParameterIndex(T const & parameter_name);
  template <typename T>
  int findFunctionParameterIndex(Function & function,
                                 T const & parameter_name);
  template <typename T>
  int findFunctionIndex(T const & function_name);
  template <typename T>
  int findCallbackIndex(T const & callback_name);
  int countJsonArrayElements(ArduinoJson::JsonArray & json_array);
  int processParameterString(Function & function,
                             const char * parameter_string);
  bool checkParameters(Function & function,
                       ArduinoJson::JsonArray::iterator iterator);
  bool checkParameter(Parameter & parameter,
                      ArduinoJson::JsonVariant & json_value);
  bool checkArrayParameterElement(Parameter & parameter,
                                  ArduinoJson::JsonVariant & json_value);
  long getSerialNumber();
  void initializeEeprom();
  void incrementServerStream();
  void propertyHelp(Property & property,
                    bool verbose);
  void parameterHelp(Parameter & parameter,
                     bool property=false);
  void functionHelp(Function & function,
                    bool verbose);
  void callbackHelp(Callback & callback,
                    bool verbose);
  void help(bool verbose);
  void writeDeviceIdToResponse();
  void writeFirmwareInfoToResponse();
  void writeHardwareInfoToResponse();
  void writeDeviceInfoToResponse();
  void interruptHelp(Interrupt & interrupt,
                     bool verbose);
  void writeInterruptInfoToResponse();
  void writeApiToResponse(bool verbose,
                          ArduinoJson::JsonArray & firmware_name_array);
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
  Interrupt * findInterruptPtrByChars(const char * interrupt_name);
  Interrupt * findInterruptPtrByConstantString(const ConstantString & interrupt_name);

  // Handlers
  void getMethodIdsHandler();
  void helpHandler();
  void verboseHelpHandler();
  void getDeviceIdHandler();
  void getDeviceInfoHandler();
  void getInterruptInfoHandler();
  void detachAllInterruptsHandler();
  void getApiHandler();
  void getApiVerboseHandler();
  void getMemoryFreeHandler();
  void getPropertyDefaultValuesHandler();
  void getAllPropertyDefaultValuesHandler();
  void getPropertyValuesHandler();
  void getAllPropertyValuesHandler();
  void setPropertiesToDefaultsHandler();
  void setAllPropertiesToDefaultsHandler();

};
}
#include "ServerDefinitions.h"

#endif
