#include "Streaming.h"
#include "JsonParser.h"
#include "StandardCplusplus.h"
#include "DeviceInterface.h"
#include "NonBlockBlink.h"

// See README.md for more information

int led_pin = 13;

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
  device_interface.response["led_pin"] = led_pin;
}

void blinkLedCallback()
{
  non_block_blink.duration_on = (long)device_interface.arguments["duration_on"];
  non_block_blink.duration_off = (long)device_interface.arguments["duration_off"];
  non_block_blink.count = (long)device_interface.arguments["count"];
  non_block_blink.start();
}

void setup()
{
  pinMode(led_pin, OUTPUT);

  device_interface.setName("arduino_led_controller");

  Command led_on_cmd("setLedOn");
  led_on_cmd.attachCallback(setLedOnCallback);
  device_interface.addCommand(led_on_cmd);

  Command led_off_cmd("setLedOff");
  led_off_cmd.attachCallback(setLedOffCallback);
  device_interface.addCommand(led_off_cmd);

  Command get_led_pin_cmd("getLedPin");
  get_led_pin_cmd.attachCallback(getLedPinCallback);
  device_interface.addCommand(get_led_pin_cmd);

  Command blink_led_cmd("blinkLed");
  blink_led_cmd.attachCallback(blinkLedCallback);
  Parameter duration_on_prm("duration_on");
  duration_on_prm.setUnits("ms");
  blink_led_cmd.addParameter(duration_on_prm);
  Parameter duration_off_prm = duration_on_prm;
  duration_off_prm.setName("duration_off");
  blink_led_cmd.addParameter(duration_off_prm);
  Parameter count_prm("count");
  blink_led_cmd.addParameter(count_prm);
  device_interface.addCommand(blink_led_cmd);

  Serial.begin(9600);
}

void loop()
{
  device_interface.processMessage();
  non_block_blink.update();
}
