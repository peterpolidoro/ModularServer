#include <EEPROM.h>
#include "Streaming.h"
#include "JsonParser.h"
#include "StandardCplusplus.h"
#include "MemoryFree.h"
#include "Flash.h"
#include "RemoteDevice.h"
#include "NonBlockBlink.h"

// See README.md for more information

using namespace RemoteDevice;

const int led_pin = 13;
const int baudrate = 9600;

const int model_number = 1234;
const int firmware_number = 1;

const double duration_min = 0.1;
const double duration_max = 2.5;
const long count_min = 1;
const long count_max = 100;

FLASH_STRING(device_name,"led_controller");
FLASH_STRING(led_on_method_name,"setLedOn");
FLASH_STRING(led_off_method_name,"setLedOff");
FLASH_STRING(get_led_pin_method_name,"getLedPin");
FLASH_STRING(blink_led_method_name,"blinkLed");
FLASH_STRING(duration_on_parameter_name,"duration_on");
FLASH_STRING(duration_off_parameter_name,"duration_off");
FLASH_STRING(count_parameter_name,"count");
FLASH_STRING(seconds_unit,"seconds");

// Callbacks must be non-blocking (avoid 'delay')
NonBlockBlink non_block_blink(led_pin);

void setLedOnCallback()
{
  non_block_blink.stop();
  digitalWrite(led_pin, HIGH);
}

void setLedOffCallback()
{
  non_block_blink.stop();
  digitalWrite(led_pin, LOW);
}

void getLedPinCallback()
{
  remote_device.addToResponse("led_pin", led_pin);
}

void blinkLedCallback()
{
  double duration_on = remote_device.getParameterValue(duration_on_parameter_name);
  double duration_off = remote_device.getParameterValue(duration_off_parameter_name);
  long count = remote_device.getParameterValue(count_parameter_name);
  non_block_blink.setDurationOn(duration_on);
  non_block_blink.setDurationOff(duration_off);
  non_block_blink.setCount(count);
  non_block_blink.start();
}


void setup()
{
  pinMode(led_pin, OUTPUT);

  remote_device.setName(device_name);
  remote_device.setModelNumber(model_number);
  remote_device.setFirmwareNumber(firmware_number);

  Method& led_on_method = remote_device.createMethod(led_on_method_name);
  led_on_method.attachCallback(setLedOnCallback);

  Method& led_off_method = remote_device.createMethod(led_off_method_name);
  led_off_method.attachCallback(setLedOffCallback);

  Method& get_led_pin_method = remote_device.createMethod(get_led_pin_method_name);
  get_led_pin_method.attachCallback(getLedPinCallback);

  Method& blink_led_method = remote_device.createMethod(blink_led_method_name);
  blink_led_method.attachCallback(blinkLedCallback);
  Parameter& duration_on_parameter = remote_device.createParameter(duration_on_parameter_name);
  duration_on_parameter.setUnits(seconds_unit);
  duration_on_parameter.setRange(duration_min,duration_max);
  blink_led_method.addParameter(duration_on_parameter);
  Parameter& duration_off_parameter = remote_device.copyParameter(duration_on_parameter,duration_off_parameter_name);
  blink_led_method.addParameter(duration_off_parameter);
  Parameter& count_parameter = remote_device.createParameter(count_parameter_name);
  count_parameter.setRange(count_min,count_max);
  blink_led_method.addParameter(count_parameter);

  Serial.begin(baudrate);
}

void loop()
{
  remote_device.handleServerRequests();
  non_block_blink.update();
}
