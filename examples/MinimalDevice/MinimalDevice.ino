#include <EEPROM.h>
#include "Streaming.h"
#include "JsonParser.h"
#include "Array.h"
#include "MemoryFree.h"
#include "Flash.h"
#include "ConstantVariable.h"
#include "ModularDevice.h"
#include "Constants.h"
#include "Callbacks.h"
#include "Controller.h"


// See README.md for more information

void setup()
{
  controller.setup();
}

void loop()
{
  controller.update();
}
