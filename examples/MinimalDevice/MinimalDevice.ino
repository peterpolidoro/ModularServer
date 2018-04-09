#include "MinimalDevice.h"


MinimalDevice dev;

void setup()
{
  dev.setup();
  dev.startServer();
}

void loop()
{
  dev.update();
}
