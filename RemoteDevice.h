// ----------------------------------------------------------------------------
// RemoteDevice.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef REMOTE_DEVICE_H
#define REMOTE_DEVICE_H
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
#include "utility/Parameter.h"
#include "utility/Method.h"
#include "utility/Constants.h"


namespace RemoteDevice
{
class RemoteDevice
{
public:
  RemoteDevice(Stream &stream);
  void setRequestStream(Stream &stream);
  void processRequest();
  void addMethod(Method method);
  void setName(char *name);
  void setModelNumber(int model_number);
  void setFirmwareNumber(int firmware_number);
  ArduinoJson::Generator::JsonObject<JSON_OBJECT_SIZE_RESPONSE> response;
  ArduinoJson::Parser::JsonObject parameters;
private:
  Stream *stream_ptr_;
  char request_[STRING_LENGTH_REQUEST];
  ArduinoJson::Parser::JsonParser<JSON_PARSER_SIZE> parser_;
  RequestType request_type_;
  std::vector<Method> method_vector_;
  char name_[STRING_LENGTH_DEVICE_NAME];
  int model_number_;
  int serial_number_;
  int firmware_number_;

  ArduinoJson::Generator::JsonArray<PARAMETER_COUNT_MAX> method_help_array_;
  ArduinoJson::Generator::JsonObject<JSON_OBJECT_SIZE_PARAMETER_HELP> parameter_help_object_;
  int parameter_count_;

  void processObjectRequest(ArduinoJson::Parser::JsonObject &json_object);
  void processArrayRequest(ArduinoJson::Parser::JsonArray &json_array);
  int processMethodString(char *method_string);
  int getMethodIndexByName(char *method_name);
  int countJsonArrayElements(ArduinoJson::Parser::JsonArray &json_array);
  void createParametersObject(int method_index, ArduinoJson::Parser::JsonArray &json_array);
  void executeMethod(int method_index);
  void methodHelp(int method_index);
  int processParameterString(int method_index, char *parameter_string);
  int getParameterIndexByName(int method_index, char *parameter_name);
  void parameterHelp(int method_index, int parameter_index);

  // reserved methods
  void getDeviceInfoCallback();
  void getMethodIdsCallback();
  void getResponseCodesCallback();
  void help();
};
}
extern RemoteDevice::RemoteDevice remote_device;
#endif
