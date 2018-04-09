#include "CallbackTester.h"


CallbackTester dev;

void setup()
{
  dev.setup();
  dev.startServer();
}

void loop()
{
  dev.update();
}
