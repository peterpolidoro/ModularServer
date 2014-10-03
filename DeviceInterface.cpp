// ----------------------------------------------------------------------------
// DeviceInterface.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "DeviceInterface.h"

Command::Command(char *name="")
{
  setName(name);
  callback_attached_ = false;
}

void Command::setName(char *name)
{
  strcpy(name_,name);
}

boolean Command::compareName(char *name_to_compare)
{
  // return (strcmp(name_to_compare,name_) == 0);
  return String(name_).equalsIgnoreCase(name_to_compare);
}

char* Command::getName()
{
  return name_;
}

void Command::printName()
{
  Serial << name_ << endl;
}

void Command::attachCallback(Callback callback)
{
  callback_ = callback;
  callback_attached_ = true;
}

void Command::callback()
{
  if (callback_attached_)
  {
    (*callback_)();
  }
}

DeviceInterface::DeviceInterface(Stream &stream)
{
  stream_ = &stream;
  command_count_ = 0;
}

void DeviceInterface::setMessageStream(Stream &stream)
{
  stream_ = &stream;
}

void DeviceInterface::processMessage()
{
  while (stream_->available() > 0)
  {
    int message_length = stream_->readBytesUntil(EOL,message_,MESSAGE_LENGTH_MAX);
    message_[message_length] = '\0';
    // *stream_ << message_ << endl;
    if (message_[0] == JSON_OBJECT_START_CHAR)
    {
      Parser::JsonObject json_object = parser_.parse(message_);
      if (json_object.success())
      {
        // for (Parser::JsonObjectIterator i=json_object.begin(); i!=json_object.end(); ++i)
        // {
        //   *stream_ << i.key() << " : " << (char*)i.value() << endl;
        // }
        processObjectMessage(json_object);
      }
      else
      {
        // *stream_ << "Parsing JSON object failed!" << endl;
      }
    }
    else
    {
      if (message_[0] != JSON_ARRAY_START_CHAR)
      {
        String msg_str = String("[") + String(message_) + String("]");
        msg_str.toCharArray(message_,MESSAGE_LENGTH_MAX);
        // *stream_ << message_ << endl;
      }
      Parser::JsonArray json_array = parser_.parse(message_);
      if (json_array.success())
      {
        // for (Parser::JsonArrayIterator i=json_array.begin(); i!=json_array.end(); ++i)
        // {
        //   *stream_ << (char*)*i << endl;
        // }
        processArrayMessage(json_array);
      }
      else
      {
        // *stream_ << "Parsing JSON array failed!" << endl;
      }
    }
  }
}

void DeviceInterface::addCommand(Command cmd)
{
  char* name = cmd.getName();
  if (String(name).length() > 0)
  {
    int command_index = getCommandIndexByName(name);
    if (command_index < 0)
    {
      command_array_[command_count_++] = cmd;
    }
    else
    {
      command_array_[command_index] = Command(name);
    }
  }
}

Command DeviceInterface::getCommandByName(char *name)
{
  int command_index = getCommandIndexByName(name);
  if (command_index >= 0)
  {
    return command_array_[command_index];
  }
  else
  {
    return Command(name);
  }
}

void DeviceInterface::processObjectMessage(Parser::JsonObject &json_object)
{
  // *stream_ << "Processing object!" << endl;
}

void DeviceInterface::processArrayMessage(Parser::JsonArray &json_array)
{
  // *stream_ << "Processing array!" << endl;
  char* cmd = json_array[0];
  // *stream_ << "Command: " << cmd << endl;
  int command_index = getCommandIndexByName(cmd);
  if (command_index >= 0)
  {
    command_array_[command_index].callback();
    // *stream_ << cmd << " Found!" << endl;
  }
  else
  {
    // *stream_ << cmd << " Not Found!" << endl;
  }
}

int DeviceInterface::getCommandIndexByName(char *name)
{
  boolean found = false;
  int command_index = -1;
  int i = 0;
  while ((!found) && (i < command_count_))
  {
    if (command_array_[i].compareName(name))
    {
      found = true;
      command_index = i;
    }
    i++;
  }
  return command_index;
}

void DeviceInterface::printAllCommandNames()
{
  for (int command_index; command_index<command_count_; command_index++)
  {
    command_array_[command_index].printName();
  }
}

DeviceInterface deviceInterface(Serial);
