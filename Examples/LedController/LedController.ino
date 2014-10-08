#include "Streaming.h"
#include "JsonParser.h"
#include "StandardCplusplus.h"
#include "DeviceInterface.h"

int led_pin = 13;

void setLedOnCallback()
{
  digitalWrite(led_pin, HIGH);
}

void setLedOffCallback()
{
  digitalWrite(led_pin, LOW);
}

void getLedPinCallback()
{
  device_interface.response["led_pin"] = led_pin;
}

void blinkLedCallback()
{
  int count = 10;
  for (int i=0;i<count;i++)
  {
    digitalWrite(led_pin, HIGH);
    delay(500);
    digitalWrite(led_pin, LOW);
    delay(500);
  }
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
  device_interface.addCommand(blink_led_cmd);

  Serial.begin(9600);
  delay(1000);
}

void loop()
{
  device_interface.processMessage();
}
