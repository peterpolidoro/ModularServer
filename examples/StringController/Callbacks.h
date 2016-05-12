// ----------------------------------------------------------------------------
// Callbacks.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef CALLBACKS_H
#define CALLBACKS_H
#include "ArduinoJson.h"
#include "ModularServer.h"
#include "Constants.h"
#include "Controller.h"


namespace callbacks
{
void echoCallback();

void lengthCallback();

void startsWithCallback();

void repeatCallback();

void charsAtCallback();

void startingCharsCallback();

void setStoredStringCallback();

void getStoredStringCallback();

void testBoolCallback();
}
#endif
