// ----------------------------------------------------------------------------
// Callbacks.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef CALLBACKS_H
#define CALLBACKS_H
#include "JsonParser.h"
#include "ModularDevice.h"
#include "Constants.h"


namespace callbacks
{
void echoCallback();

void lengthCallback();

void startsWithCallback();

void repeatCallback();

void charsAtCallback();

void startingCharsCallback();

void setStartingCharsCountCallback();

void getStartingCharsCountCallback();
}
#endif
