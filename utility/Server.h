// ----------------------------------------------------------------------------
// Server.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _SERVER_H_
#define _SERVER_H_
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
#include "SavedVariable.h"
#include "Constants.h"
#include "JsonPrinter.h"


namespace RemoteDevice
{
class Server
{
public:
  Server(HardwareSerial &serial=Serial);
  void setSerial(HardwareSerial &serial);
  void setName(const _FLASH_STRING &name);
  void setModelNumber(const unsigned int model_number);
  void setSerialNumber(const unsigned int serial_number);
  void setFirmwareNumber(const unsigned int firmware_number);
  Method& createMethod(const _FLASH_STRING &method_name);
  Method& copyMethod(Method method,const _FLASH_STRING &method_name);
  Parameter& createParameter(const _FLASH_STRING &parameter_name);
  Parameter& copyParameter(Parameter parameter,const _FLASH_STRING &parameter_name);
  ArduinoJson::Parser::JsonValue getParameterValue(const _FLASH_STRING &name);
  template<typename T>
  SavedVariable& createSavedVariable(const _FLASH_STRING &saved_variable_name, const T &default_value)
  {
    int saved_variable_index = findSavedVariableIndex(saved_variable_name);
    if (saved_variable_index < 0)
    {
      saved_variable_vector_.push_back(SavedVariable(saved_variable_name,eeprom_index_,default_value));
      unsigned char eeprom_init_value;
      getSavedVariableValue(*eeprom_init_name_ptr_,eeprom_init_value);
      if (eeprom_init_value != EEPROM_INITIALIZED_VALUE)
      {
        saved_variable_vector_.back().setDefaultValue();
      }
      eeprom_index_ += sizeof(default_value);
      return saved_variable_vector_.back();
    }
  }
  template<typename T>
  void setSavedVariableValue(const _FLASH_STRING &saved_variable_name, const T &value)
  {
    int saved_variable_index = findSavedVariableIndex(saved_variable_name);
    if (saved_variable_index >= 0)
    {
      saved_variable_vector_[saved_variable_index].setValue(value);
    }
  }
  template<typename T>
  void getSavedVariableValue(const _FLASH_STRING &saved_variable_name, T &value)
  {
    int saved_variable_index = findSavedVariableIndex(saved_variable_name);
    if (saved_variable_index >= 0)
    {
      saved_variable_vector_[saved_variable_index].getValue(value);
    }
  }
  template<typename T>
  void addToResponse(const char *key, T value)
  {
    response_.add(key,value);
  }
  template<typename T>
  void addToResponse(T value)
  {
    response_.add(value);
  }
  void addNullToResponse(const char *key);
  void addNullToResponse();
  void addBooleanToResponse(const char *key, const boolean value);
  void addBooleanToResponse(const boolean value);
  void addKeyToResponse(const char *key);
  void startResponseObject();
  void stopResponseObject();
  void startResponseArray();
  void stopResponseArray();
  void resetDefaults();
  void startServer(const int baudrate);
  void handleRequest();
private:
  HardwareSerial *serial_ptr_;
  char request_[STRING_LENGTH_REQUEST];
  ArduinoJson::Parser::JsonParser<JSON_PARSER_SIZE> parser_;
  ArduinoJson::Parser::JsonArray request_json_array_;
  std::vector<Method> method_vector_;
  std::vector<Parameter> parameter_vector_;
  std::vector<SavedVariable> saved_variable_vector_;
  const _FLASH_STRING *name_ptr_;
  unsigned int model_number_;
  unsigned int firmware_number_;
  int request_method_index_;
  int parameter_count_;
  JsonPrinter response_;
  boolean error_;
  unsigned int eeprom_index_;
  const _FLASH_STRING *eeprom_init_name_ptr_;
  boolean eeprom_uninitialized_;
  unsigned int eeprom_initialized_index_;

  void processRequestArray();
  int processMethodString(char *method_string);
  int findMethodIndex(char *method_name);
  int findMethodIndex(const _FLASH_STRING &method_name);
  int countJsonArrayElements(ArduinoJson::Parser::JsonArray &json_array);
  void executeMethod();
  void methodHelp(int method_index);
  void verboseMethodHelp(int method_index);
  int processParameterString(char *parameter_string);
  int findParameterIndex(const char *parameter_name);
  int findParameterIndex(const _FLASH_STRING &parameter_name);
  void parameterHelp(Parameter &parameter);
  boolean checkParameters();
  boolean checkParameter(int parameter_index, ArduinoJson::Parser::JsonValue json_value);
  int findSavedVariableIndex(const _FLASH_STRING &saved_variable_name);
  unsigned int getSerialNumber();
  void initializeEeprom();

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
