// LedController
#ifdef __AVR__
#include "Flash.h"
#include <EEPROM.h>
#endif
#include "Streaming.h"
#include "JsonParser.h"
#include "Array.h"
#include "MemoryFree.h"
#include "ConstantVariable.h"
#include "SavedVariable.h"
#include "ModularDevice.h"
#include "NonBlockBlink.h"
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
