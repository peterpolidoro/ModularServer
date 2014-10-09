// ----------------------------------------------------------------------------
// DeviceInterface.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _DEVICE_INTERFACE_H_
#define _DEVICE_INTERFACE_H_
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
#include "Parameter.h"
#include "Command.h"

using namespace ArduinoJson;

const char EOL = '\n';
const char JSON_OBJECT_START_CHAR = '{';
const char JSON_ARRAY_START_CHAR = '[';
const int MESSAGE_LENGTH_MAX = 257;

const int DEVICE_NAME_LENGTH_MAX = 32;
const int ERROR_MESSAGE_LENGTH_MAX = 32;

const int JSON_PARSER_SIZE = 32;
const int JSON_RESPONSE_SIZE = 32;
const int JSON_ARGUMENTS_SIZE = 32;
const int JSON_COMMANDS_COUNT_MAX = 32;

enum ResponseCodes
  {
    ERROR=0,
    SUCCESS=1,
  };

enum MessageType
  {
    OBJECT_MESSAGE,
    ARRAY_MESSAGE,
    COMMAND_LINE_MESSAGE,
  };

class DeviceInterface
{
public:
  DeviceInterface(Stream &stream);
  void setMessageStream(Stream &stream);
  void processMessage();
  void addCommand(Command command);
  void setName(char *name);
  void setModelNumber(int model_number);
  void setFirmwareNumber(int firmware_number);
  Generator::JsonObject<JSON_RESPONSE_SIZE> response;
  Generator::JsonObject<JSON_ARGUMENTS_SIZE> arguments;
private:
  Stream *stream_;
  char message_[MESSAGE_LENGTH_MAX];
  Parser::JsonParser<JSON_PARSER_SIZE> parser_;
  MessageType message_type_;
  std::vector<Command> command_vector_;
  char name_[DEVICE_NAME_LENGTH_MAX];
  int model_number_;
  int serial_number_;
  int firmware_number_;

  void processObjectMessage(Parser::JsonObject &json_object);
  void processArrayMessage(Parser::JsonArray &json_array);
  int processCommandString(char *command_str);
  int getCommandIndexByName(char *command_name);
  int countJsonArrayElements(Parser::JsonArray &json_array);
  void createArgumentsObjectFromArrayMessage(int command_index, Parser::JsonArray &json_array);
  void executeCommand(int command_index);
  // reserved commands
  void getDeviceInfoCallback();
  void getCommandsCallback();
  void getResponseCodesCallback();
  void help();
};

extern DeviceInterface device_interface;
#endif
