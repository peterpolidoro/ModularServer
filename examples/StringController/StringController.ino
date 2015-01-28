#include <EEPROM.h>
#include "Streaming.h"
#include "JsonParser.h"
#include "StandardCplusplus.h"
#include "MemoryFree.h"
#include "Flash.h"
#include "ModularDevice.h"
#include "Constants.h"
#include "Callbacks.h"

// See README.md for more information

using namespace ModularDevice;

void setup()
{
  // Pin Setup

  // Device Info
  modular_device.setName(constants::device_name);
  modular_device.setModelNumber(constants::model_number);
  modular_device.setFirmwareNumber(constants::firmware_number);

  // Saved Variables
  modular_device.createSavedVariable(constants::starting_chars_count_name,constants::starting_chars_count_default);

  // Parameters
  Parameter& string_parameter = modular_device.createParameter(constants::string_parameter_name);
  string_parameter.setTypeString();
  Parameter& string2_parameter = modular_device.copyParameter(string_parameter,constants::string2_parameter_name);
  Parameter& count_parameter = modular_device.createParameter(constants::count_parameter_name);
  count_parameter.setRange(constants::count_min,constants::count_max);
  Parameter& index_array_parameter = modular_device.createParameter(constants::index_array_parameter_name);
  index_array_parameter.setTypeArray();
  Parameter& starting_chars_count_parameter = modular_device.createParameter(constants::starting_chars_count_name);
  starting_chars_count_parameter.setRange(constants::starting_chars_count_min,constants::starting_chars_count_max);

  // Methods
  Method& echo_method = modular_device.createMethod(constants::echo_method_name);
  echo_method.attachCallback(callbacks::echoCallback);
  echo_method.addParameter(string_parameter);

  Method& length_method = modular_device.createMethod(constants::length_method_name);
  length_method.attachCallback(callbacks::lengthCallback);
  length_method.addParameter(string_parameter);

  Method& starts_with_method = modular_device.createMethod(constants::starts_with_method_name);
  starts_with_method.attachCallback(callbacks::startsWithCallback);
  starts_with_method.addParameter(string_parameter);
  starts_with_method.addParameter(string2_parameter);

  Method& repeat_method = modular_device.createMethod(constants::repeat_method_name);
  repeat_method.attachCallback(callbacks::repeatCallback);
  repeat_method.addParameter(string_parameter);
  repeat_method.addParameter(count_parameter);

  Method& chars_at_method = modular_device.createMethod(constants::chars_at_method_name);
  chars_at_method.attachCallback(callbacks::charsAtCallback);
  chars_at_method.addParameter(string_parameter);
  chars_at_method.addParameter(index_array_parameter);

  Method& starting_chars_method = modular_device.createMethod(constants::starting_chars_method_name);
  starting_chars_method.attachCallback(callbacks::startingCharsCallback);
  starting_chars_method.addParameter(string_parameter);

  Method& set_starting_chars_count_method = modular_device.createMethod(constants::set_starting_chars_count_method_name);
  set_starting_chars_count_method.attachCallback(callbacks::setStartingCharsCountCallback);
  set_starting_chars_count_method.addParameter(starting_chars_count_parameter);

  Method& get_starting_chars_count_method = modular_device.createMethod(constants::get_starting_chars_count_method_name);
  get_starting_chars_count_method.attachCallback(callbacks::getStartingCharsCountCallback);

  // Start Server
  modular_device.startServer(constants::baudrate);
}

void loop()
{
  modular_device.handleServerRequests();
}
