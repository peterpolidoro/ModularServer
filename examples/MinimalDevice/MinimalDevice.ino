#include <EEPROM.h>
#include <iterator>
#include <vector>
#include "Streaming.h"
#include "ArduinoJson.h"
#include "MemoryFree.h"
#include "Flash.h"
#include "RemoteDevice.h"
#include "Constants.h"

// See README.md for more information

using namespace RemoteDevice;

void setup()
{
  remote_device.setName(constants::device_name);
  remote_device.setModelNumber(constants::model_number);
  remote_device.setFirmwareNumber(constants::firmware_number);

  remote_device.startServer(constants::baudrate);
}

void loop()
{
  remote_device.handleServerRequests();
}
