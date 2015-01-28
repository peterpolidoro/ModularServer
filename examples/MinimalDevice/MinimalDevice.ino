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

  // Parameters

  // Methods

  // Start Server
  modular_device.startServer(constants::baudrate);
}

void loop()
{
  modular_device.handleServerRequests();
}
