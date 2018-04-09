#include "PropertyTester.h"


PropertyTester dev;

void setup()
{
  dev.setup();
  dev.startServer();
}

void loop()
{
  dev.update();
}
