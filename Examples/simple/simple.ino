#include "Streaming.h"
#include "JsonParser.h"
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

void setup()
{
  pinMode(led_pin, OUTPUT);

  Command led_on_cmd("setLedOn");
  led_on_cmd.attachCallback(setLedOnCallback);
  deviceInterface.addCommand(led_on_cmd);

  Command led_off_cmd("setLedOff");
  led_off_cmd.attachCallback(setLedOffCallback);
  deviceInterface.addCommand(led_off_cmd);

  Serial.begin(9600);
  delay(1000);
  deviceInterface.printAllCommandNames();
}

void loop()
{
  deviceInterface.processMessage();
}
