#include <EEPROM.h>
#include "Streaming.h"
#include "JsonParser.h"
#include "StandardCplusplus.h"
#include "MemoryFree.h"
#include "Flash.h"
#include "RemoteDevice.h"

// See README.md for more information

using namespace RemoteDevice;
using namespace ArduinoJson::Parser;

const int baudrate = 9600;

const int model_number = 1235;
const int firmware_number = 1;

const int count_min = 1;
const int count_max = 100;

const int starting_chars_count_min = 1;
const int starting_chars_count_max = 10;
const int starting_chars_count_default = 2;

FLASH_STRING(device_name,"string_controller");
FLASH_STRING(echo_method_name,"echo");
FLASH_STRING(length_method_name,"length");
FLASH_STRING(starts_with_method_name,"startsWith");
FLASH_STRING(repeat_method_name,"repeat");
FLASH_STRING(chars_at_method_name,"charsAt");
FLASH_STRING(starting_chars_method_name,"startingChars");
FLASH_STRING(set_starting_chars_count_method_name,"setStartingCharsCount");
FLASH_STRING(get_starting_chars_count_method_name,"getStartingCharsCount");
FLASH_STRING(string_parameter_name,"string");
FLASH_STRING(string2_parameter_name,"string2");
FLASH_STRING(count_parameter_name,"count");
FLASH_STRING(index_array_parameter_name,"index_array");
FLASH_STRING(starting_chars_count_name,"starting_chars_count");

// Callbacks must be non-blocking (avoid 'delay')

void echoCallback()
{
  // remote_device.getParameterValue must be cast to either:
  // char*
  // long
  // double
  // JsonArray
  // JsonObject
  //
  // For more info read about ArduinoJson JsonParser JsonValues
  char* string = remote_device.getParameterValue(string_parameter_name);
  remote_device.addToResponse("echo", string);
}

void lengthCallback()
{
  char* string = remote_device.getParameterValue(string_parameter_name);
  remote_device.addToResponse("length", String(string).length());
}

void startsWithCallback()
{
  char* string = remote_device.getParameterValue(string_parameter_name);
  char* string2 = remote_device.getParameterValue(string2_parameter_name);
  remote_device.addBooleanToResponse("starts_with", String(string).startsWith(string2));
}

void repeatCallback()
{
  char* string = remote_device.getParameterValue(string_parameter_name);
  long count = remote_device.getParameterValue(count_parameter_name);
  remote_device.addKeyToResponse("strings");
  remote_device.startResponseArray();
  for (int i=0; i < count; i++)
  {
    remote_device.addToResponse(string);
  }
  remote_device.stopResponseArray();
}

void charsAtCallback()
{
  char* string = remote_device.getParameterValue(string_parameter_name);
  JsonArray index_array = remote_device.getParameterValue(index_array_parameter_name);
  remote_device.addKeyToResponse("result");
  remote_device.startResponseArray();
  for (JsonArrayIterator index_it=index_array.begin();
       index_it != index_array.end();
       ++index_it)
  {
    remote_device.startResponseObject();
    long index = *index_it;
    remote_device.addToResponse("index",index);
    char c = String(string).charAt(index);
    remote_device.addToResponse("char",c);
    remote_device.stopResponseObject();
  }
  remote_device.stopResponseArray();
}

void startingCharsCallback()
{
  char* string = remote_device.getParameterValue(string_parameter_name);
  int starting_chars_count;
  // remote_device.getSavedVariableValue type must match the saved variable default type
  remote_device.getSavedVariableValue(starting_chars_count_name,starting_chars_count);
  remote_device.addToResponse("starting_chars", String(string).substring(0,starting_chars_count));
}

void setStartingCharsCountCallback()
{
  // remote_device.setSavedVariableValue type must match the saved variable default type
  // remote_device.getParameterValue type might not match saved variable type
  // so recast type if necessary to make all types match
  int starting_chars_count = (long)remote_device.getParameterValue(starting_chars_count_name);
  remote_device.setSavedVariableValue(starting_chars_count_name,starting_chars_count);
}

void getStartingCharsCountCallback()
{
  int starting_chars_count;
  // remote_device.getSavedVariableValue type must match the saved variable default type
  remote_device.getSavedVariableValue(starting_chars_count_name,starting_chars_count);
  remote_device.addToResponse("starting_chars_count",starting_chars_count);
}


void setup()
{
  remote_device.setName(device_name);
  remote_device.setModelNumber(model_number);
  remote_device.setFirmwareNumber(firmware_number);

  Method& echo_method = remote_device.createMethod(echo_method_name);
  echo_method.attachCallback(echoCallback);
  Parameter& string_parameter = remote_device.createParameter(string_parameter_name);
  string_parameter.setTypeString();
  echo_method.addParameter(string_parameter);

  Method& length_method = remote_device.createMethod(length_method_name);
  length_method.attachCallback(lengthCallback);
  length_method.addParameter(string_parameter);

  Method& starts_with_method = remote_device.createMethod(starts_with_method_name);
  starts_with_method.attachCallback(startsWithCallback);
  starts_with_method.addParameter(string_parameter);
  Parameter& string2_parameter = remote_device.copyParameter(string_parameter,string2_parameter_name);
  starts_with_method.addParameter(string2_parameter);

  Method& repeat_method = remote_device.createMethod(repeat_method_name);
  repeat_method.attachCallback(repeatCallback);
  repeat_method.addParameter(string_parameter);
  Parameter& count_parameter = remote_device.createParameter(count_parameter_name);
  count_parameter.setRange(count_min,count_max);
  repeat_method.addParameter(count_parameter);

  Method& chars_at_method = remote_device.createMethod(chars_at_method_name);
  chars_at_method.attachCallback(charsAtCallback);
  chars_at_method.addParameter(string_parameter);
  Parameter& index_array_parameter = remote_device.createParameter(index_array_parameter_name);
  index_array_parameter.setTypeArray();
  chars_at_method.addParameter(index_array_parameter);

  Method& starting_chars_method = remote_device.createMethod(starting_chars_method_name);
  starting_chars_method.attachCallback(startingCharsCallback);
  starting_chars_method.addParameter(string_parameter);

  remote_device.createSavedVariable(starting_chars_count_name,starting_chars_count_default);

  Method& set_starting_chars_count_method = remote_device.createMethod(set_starting_chars_count_method_name);
  set_starting_chars_count_method.attachCallback(setStartingCharsCountCallback);
  Parameter& starting_chars_count_parameter = remote_device.createParameter(starting_chars_count_name);
  starting_chars_count_parameter.setRange(starting_chars_count_min,starting_chars_count_max);
  set_starting_chars_count_method.addParameter(starting_chars_count_parameter);

  Method& get_starting_chars_count_method = remote_device.createMethod(get_starting_chars_count_method_name);
  get_starting_chars_count_method.attachCallback(getStartingCharsCountCallback);

  Serial.begin(baudrate);
}

void loop()
{
  remote_device.handleServerRequests();
}
