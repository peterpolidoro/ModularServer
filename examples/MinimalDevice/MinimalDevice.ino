#include <EEPROM.h>
#include "Streaming.h"
#include "JsonParser.h"
#include "StandardCplusplus.h"
#include "MemoryFree.h"
#include "Flash.h"
#include "RemoteDevice.h"

// See README.md for more information

using namespace RemoteDevice;

const int baudrate = 9600;

const int model_number = 1000;
const int firmware_number = 1;

FLASH_STRING(device_name,"minimal_device");

void setup()
{
  remote_device.setName(device_name);
  remote_device.setModelNumber(model_number);
  remote_device.setFirmwareNumber(firmware_number);

  Serial.begin(baudrate);
}

void loop()
{
  remote_device.handleServerRequests();
}
