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

using namespace ArduinoJson;

const char EOL = '\n';
const char JSON_OBJECT_START_CHAR = '{';
const char JSON_ARRAY_START_CHAR = '[';
const int MESSAGE_LENGTH_MAX = 257;

const int CMD_NAME_LENGTH_MAX = 32;
const int CMD_COUNT_MAX = 32;

enum Responses {
  ERROR_RESPONSE=0,
  SUCCESS_RESPONSE=1,
};

enum ReservedCommands {
  CMD_GET_DEVICE_INFO=0,
  CMD_GET_COMMAND_IDS=1,
  CMD_GET_RESPONSE_CODES=2,
  RESERVED_CMD_COUNT,
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
  Command getCommandByName(char *name);
  void printAllCommandNames();
private:
  Stream *stream_;
  char message_[MESSAGE_LENGTH_MAX];
  Parser::JsonParser<32> parser_;
  Command command_array_[CMD_COUNT_MAX];
  int command_count_;

  void processObjectMessage(Parser::JsonObject &json_object);
  void processArrayMessage(Parser::JsonArray &json_array);
  int getCommandIndexByName(char *name);
};

extern DeviceInterface deviceInterface;

#endif
