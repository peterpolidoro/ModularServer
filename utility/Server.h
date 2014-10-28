// ----------------------------------------------------------------------------
// Server.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef SERVER_H
#define SERVER_H
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Stream.h>
#include "Streaming.h"
#include "JsonParser.h"
#include "StandardCplusplus.h"
#include "vector"
#include "MemoryFree.h"
#include "Flash.h"
#include "Parameter.h"
#include "Method.h"
#include "Constants.h"
#include "JsonPrinter.h"


namespace RemoteDevice
{
class Server
{
public:
  Server(Stream &stream=Serial);
  void setStream(Stream &stream);
  void handleRequest();
  void setName(_FLASH_STRING &name);
  void setModelNumber(int model_number);
  void setFirmwareNumber(int firmware_number);
  Method& createMethod(_FLASH_STRING &method_name);
  Method& copyMethod(Method method,_FLASH_STRING &method_name);
  Parameter& createParameter(_FLASH_STRING &parameter_name);
  Parameter& copyParameter(Parameter parameter,_FLASH_STRING &parameter_name);
  ArduinoJson::Parser::JsonValue getParameterValue(_FLASH_STRING &name);
  template<typename T>
  void addToResponse(const char* key, T value)
  {
    response_.add(key,value);
  }
  template<typename T>
  void addToResponse(T value)
  {
    response_.add(value);
  }
  void addNullToResponse(const char* key);
  void addNullToResponse();
  void addBooleanToResponse(const char* key, boolean value);
  void addBooleanToResponse(boolean value);
  void addKeyToResponse(const char* key);
  void startResponseObject();
  void stopResponseObject();
  void startResponseArray();
  void stopResponseArray();
private:
  Stream *stream_ptr_;
  char request_[STRING_LENGTH_REQUEST];
  ArduinoJson::Parser::JsonParser<JSON_PARSER_SIZE> parser_;
  ArduinoJson::Parser::JsonArray request_json_array_;
  std::vector<Method> method_vector_;
  std::vector<Parameter> parameter_vector_;
  _FLASH_STRING *name_ptr_;
  int model_number_;
  int serial_number_;
  int firmware_number_;
  int request_method_index_;
  int parameter_count_;
  JsonPrinter response_;
  boolean error_;

  void processRequestArray();
  int processMethodString(char *method_string);
  int findMethodIndex(char *method_name);
  int findMethodIndex(_FLASH_STRING &method_name);
  int countJsonArrayElements(ArduinoJson::Parser::JsonArray &json_array);
  void executeMethod();
  void methodHelp(int method_index);
  void verboseMethodHelp(int method_index);
  int processParameterString(char *parameter_string);
  int findParameterIndex(const char *parameter_name);
  int findParameterIndex(_FLASH_STRING &parameter_name);
  void parameterHelp(Parameter &parameter);
  boolean checkParameters();
  boolean checkParameter(int parameter_index, ArduinoJson::Parser::JsonValue json_value);

  // reserved methods
  void getDeviceInfoCallback();
  void getMethodIdsCallback();
  void getResponseCodesCallback();
  void getParametersCallback();
  void help();
  void verboseHelp();
};
}

#endif
