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

FLASH_STRING(device_name,"string_controller");
FLASH_STRING(echo_method_name,"echo");
FLASH_STRING(length_method_name,"length");
FLASH_STRING(starts_with_method_name,"startsWith");
FLASH_STRING(repeat_method_name,"repeat");
FLASH_STRING(chars_at_method_name,"charsAt");
FLASH_STRING(string_parameter_name,"string");
FLASH_STRING(string2_parameter_name,"string2");
FLASH_STRING(count_parameter_name,"count");
FLASH_STRING(index_array_parameter_name,"index_array");

// Callbacks must be non-blocking (avoid 'delay')

void echoCallback()
{
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


void setup()
{
  remote_device.setName(device_name);
  remote_device.setModelNumber(1235);
  remote_device.setFirmwareNumber(1);

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
  count_parameter.setRange(1,10);
  repeat_method.addParameter(count_parameter);

  Method& chars_at_method = remote_device.createMethod(chars_at_method_name);
  chars_at_method.attachCallback(charsAtCallback);
  chars_at_method.addParameter(string_parameter);
  Parameter& index_array_parameter = remote_device.createParameter(index_array_parameter_name);
  index_array_parameter.setTypeArray();
  chars_at_method.addParameter(index_array_parameter);

  Serial.begin(baudrate);
}

void loop()
{
  remote_device.handleServerRequests();
}
