#include <EEPROM.h>
#include <iterator>
#include <vector>
#include "Streaming.h"
#include "ArduinoJson.h"
#include "MemoryFree.h"
#include "Flash.h"
#include "RemoteDevice.h"
#include "NonBlockBlink.h"
#include "Constants.h"
#include "Callbacks.h"

// See README.md for more information

using namespace RemoteDevice;

void setup()
{
  pinMode(constants::led_pin, OUTPUT);

  remote_device.setName(constants::device_name);
  remote_device.setModelNumber(constants::model_number);
  remote_device.setFirmwareNumber(constants::firmware_number);

  Method& led_on_method = remote_device.createMethod(constants::led_on_method_name);
  led_on_method.attachCallback(callbacks::setLedOnCallback);

  Method& led_off_method = remote_device.createMethod(constants::led_off_method_name);
  led_off_method.attachCallback(callbacks::setLedOffCallback);

  Method& get_led_pin_method = remote_device.createMethod(constants::get_led_pin_method_name);
  get_led_pin_method.attachCallback(callbacks::getLedPinCallback);

  Method& blink_led_method = remote_device.createMethod(constants::blink_led_method_name);
  blink_led_method.attachCallback(callbacks::blinkLedCallback);
  Parameter& duration_on_parameter = remote_device.createParameter(constants::duration_on_parameter_name);
  duration_on_parameter.setUnits(constants::seconds_unit);
  duration_on_parameter.setRange(constants::duration_min,constants::duration_max);
  blink_led_method.addParameter(duration_on_parameter);
  Parameter& duration_off_parameter = remote_device.copyParameter(duration_on_parameter,constants::duration_off_parameter_name);
  blink_led_method.addParameter(duration_off_parameter);
  Parameter& count_parameter = remote_device.createParameter(constants::count_parameter_name);
  count_parameter.setRange(constants::count_min,constants::count_max);
  blink_led_method.addParameter(count_parameter);

  remote_device.startServer(constants::baudrate);
}

void loop()
{
  remote_device.handleServerRequests();
  non_block_blink.update();
}
