#include "Streaming.h"
#include "JsonParser.h"
#include "StandardCplusplus.h"
#include "RemoteDevice.h"
#include "NonBlockBlink.h"

// See README.md for more information

const int led_pin = 13;
const int baudrate = 9600;

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
  remote_device.response["result"] = led_pin;
}

void blinkLedCallback()
{
  non_block_blink.duration_on = (long)remote_device.parameters["duration_on"];
  non_block_blink.duration_off = (long)remote_device.parameters["duration_off"];
  non_block_blink.count = (long)remote_device.parameters["count"];
  non_block_blink.start();
}

void setup()
{
  pinMode(led_pin, OUTPUT);

  remote_device.setName("led_controller");
  remote_device.setModelNumber(1234);
  remote_device.setFirmwareNumber(1);

  Method led_on_method("setLedOn");
  led_on_method.attachCallback(setLedOnCallback);
  remote_device.addMethod(led_on_method);

  Method led_off_method("setLedOff");
  led_off_method.attachCallback(setLedOffCallback);
  remote_device.addMethod(led_off_method);

  Method get_led_pin_method("getLedPin");
  get_led_pin_method.attachCallback(getLedPinCallback);
  remote_device.addMethod(get_led_pin_method);

  Method blink_led_method("blinkLed");
  blink_led_method.attachCallback(blinkLedCallback);
  Parameter duration_on_parameter("duration_on");
  duration_on_parameter.setUnits("ms");
  blink_led_method.addParameter(duration_on_parameter);
  Parameter duration_off_parameter = duration_on_parameter;
  duration_off_parameter.setName("duration_off");
  blink_led_method.addParameter(duration_off_parameter);
  Parameter count_parameter("count");
  blink_led_method.addParameter(count_parameter);
  remote_device.addMethod(blink_led_method);

  Serial.begin(baudrate);
}

void loop()
{
  remote_device.processRequest();
  non_block_blink.update();
}
