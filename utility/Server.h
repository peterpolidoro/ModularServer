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
#include "JsonGenerator.h"
#include "StandardCplusplus.h"
#include "vector"
#include "MemoryFree.h"
#include "Flash.h"
#include "Parameter.h"
#include "Method.h"
#include "Constants.h"


namespace RemoteDevice
{
class Server
{
public:
  Server(Stream &stream=Serial);
  void setRequestStream(Stream &stream);
  void handleRequest();
  void setName(_FLASH_STRING &name);
  void setModelNumber(int model_number);
  void setFirmwareNumber(int firmware_number);
  Method& createMethod(_FLASH_STRING &method_name);
  Method& createMethod(Method method);
  Parameter& createParameter(_FLASH_STRING &parameter_name);
  Parameter& createParameter(Parameter parameter);
  ArduinoJson::Parser::JsonValue getParameterValue(_FLASH_STRING &name);
  ArduinoJson::Generator::JsonObject<JSON_OBJECT_SIZE_RESPONSE> response;
private:
  Stream *stream_ptr_;
  char request_[STRING_LENGTH_REQUEST];
  ArduinoJson::Parser::JsonParser<JSON_PARSER_SIZE> parser_;
  ArduinoJson::Parser::JsonArray request_json_array_;
  RequestType request_type_;
  std::vector<Method> method_vector_;
  std::vector<Parameter> parameter_vector_;
  _FLASH_STRING *name_ptr_;
  int model_number_;
  int serial_number_;
  int firmware_number_;
  int request_method_index_;

  ArduinoJson::Generator::JsonArray<PARAMETER_COUNT_MAX> method_help_array_;
  ArduinoJson::Generator::JsonObject<JSON_OBJECT_SIZE_PARAMETER_HELP> parameter_help_object_;
  char parameter_name_array_[PARAMETER_COUNT_MAX][STRING_LENGTH_PARAMETER_NAME];
  int parameter_count_;

  void processRequestArray();
  int processMethodString(char *method_string);
  int findMethodIndexByName(char *method_name);
  int findMethodIndexByName(_FLASH_STRING &parameter_name);
  int countJsonArrayElements(ArduinoJson::Parser::JsonArray &json_array);
  void executeMethod();
  void methodHelp();
  int processParameterString(char *parameter_string);
  int findParameterIndexByName(const char *parameter_name);
  int findParameterIndexByName(_FLASH_STRING &parameter_name);
  void parameterHelp(int parameter_index);
  boolean checkParameters();
  boolean checkParameter(int parameter_index, ArduinoJson::Parser::JsonValue json_value);

  // reserved methods
  void getDeviceInfoCallback();
  void getMethodIdsCallback();
  void getResponseCodesCallback();
  void help();
};
}

#endif
