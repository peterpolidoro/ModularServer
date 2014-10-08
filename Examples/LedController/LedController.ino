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

  Serial.begin(9600);
  delay(1000);
}

void loop()
{
  device_interface.processMessage();
}
