// ----------------------------------------------------------------------------
// Constants.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Callbacks.h"

namespace callbacks
{
// Callbacks must be non-blocking (avoid 'delay')
void echoCallback()
{
  // remote_device.getParameterValue must be cast to either:
  // const char*
  // long
  // double
  // JsonArray&
  // JsonObject&
  //
  // For more info read about ArduinoJson Decoding/Parsing
  // https://github.com/bblanchon/ArduinoJson
  const char* string = remote_device.getParameterValue(constants::string_parameter_name);
  remote_device.addToResponse("echo", string);
}

void lengthCallback()
{
  const char* string = remote_device.getParameterValue(constants::string_parameter_name);
  remote_device.addToResponse("length", String(string).length());
}

void startsWithCallback()
{
  const char* string = remote_device.getParameterValue(constants::string_parameter_name);
  const char* string2 = remote_device.getParameterValue(constants::string2_parameter_name);
  remote_device.addBooleanToResponse("starts_with", String(string).startsWith(string2));
}

void repeatCallback()
{
  const char* string = remote_device.getParameterValue(constants::string_parameter_name);
  long count = remote_device.getParameterValue(constants::count_parameter_name);
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
  const char* string = remote_device.getParameterValue(constants::string_parameter_name);
  JsonArray& index_array = remote_device.getParameterValue(constants::index_array_parameter_name);
  remote_device.addKeyToResponse("result");
  remote_device.startResponseArray();
  for (JsonArray::iterator index_it=index_array.begin();
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
  const char* string = remote_device.getParameterValue(constants::string_parameter_name);
  int starting_chars_count;
  // remote_device.getSavedVariableValue type must match the saved variable default type
  remote_device.getSavedVariableValue(constants::starting_chars_count_name,starting_chars_count);
  remote_device.addToResponse("starting_chars", String(string).substring(0,starting_chars_count));
}

void setStartingCharsCountCallback()
{
  // remote_device.setSavedVariableValue type must match the saved variable default type
  // remote_device.getParameterValue type might not match saved variable type
  // so recast type if necessary to make all types match
  int starting_chars_count = (long)remote_device.getParameterValue(constants::starting_chars_count_name);
  remote_device.setSavedVariableValue(constants::starting_chars_count_name,starting_chars_count);
}

void getStartingCharsCountCallback()
{
  int starting_chars_count;
  // remote_device.getSavedVariableValue type must match the saved variable default type
  remote_device.getSavedVariableValue(constants::starting_chars_count_name,starting_chars_count);
  remote_device.addToResponse("starting_chars_count",starting_chars_count);
}
}
