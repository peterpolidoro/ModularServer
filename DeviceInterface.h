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

using namespace ArduinoJson;

const char EOL = '\n';
const char JSON_OBJECT_START_CHAR = '{';
const char JSON_ARRAY_START_CHAR = '[';
const int MESSAGE_LENGTH_MAX = 257;

const int CMD_NAME_LENGTH_MAX = 32;

const int JSON_PARSER_SIZE = 32;
const int JSON_RESPONSE_SIZE = 32;

enum ResponseCodes
  {
    ERROR=0,
    SUCCESS=1,
  };

enum ReservedCommands
  {
    CMD_GET_DEVICE_INFO=0,
    CMD_GET_COMMANDS=1,
    CMD_GET_RESPONSE_CODES=2,
    RESERVED_COMMAND_COUNT=3,
  };

enum MessageType
  {
    OBJECT_MESSAGE,
    ARRAY_MESSAGE,
    COMMAND_LINE_MESSAGE,
  };

extern "C" {
  typedef void (*Callback)(void);
}

class Command
{
public:
  Command(char *name);
  void setName(char *name);
  boolean compareName(char *name_to_compare);
  char* getName();
  void printName();
  void attachCallback(Callback callback);
  void callback();
private:
  char name_[CMD_NAME_LENGTH_MAX];
  Callback callback_;
  boolean callback_attached_;
};

class DeviceInterface
{
public:
  DeviceInterface(Stream &stream);
  void setMessageStream(Stream &stream);
  void processMessage();
  void addCommand(Command cmd);
  void setModelNumber(int model_number);
  void setFirmwareNumber(int firmware_number);
  // Command getCommandByName(char *name);
  void printAllCommandNames();
  Generator::JsonObject<JSON_RESPONSE_SIZE> response;
private:
  Stream *stream_;
  char message_[MESSAGE_LENGTH_MAX];
  Parser::JsonParser<JSON_PARSER_SIZE> parser_;
  MessageType message_type_;
  std::vector<Command> command_vector_;
  int model_number_;
  int serial_number_;
  int firmware_number_;

  void processObjectMessage(Parser::JsonObject &json_object);
  void processArrayMessage(Parser::JsonArray &json_array);
  void processCommand(char *command_str);
  int getCommandIndex(char *command_name);
  // reserved commands
  void getDeviceInfoCallback();
  void getCommandsCallback();
  void getResponseCodesCallback();
};

extern DeviceInterface device_interface;
#endif
