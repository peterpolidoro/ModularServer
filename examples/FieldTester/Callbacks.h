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
void getDoubledCallback();

void getBoolCallback();

void getLongArrayFixedCallback();

void getLongArrayVariableCallback();

void setLongArrayFixedCallback();

void setLongArrayVariableCallback();

void setLongArrayParameterCallback();

void getStringAllCallback();

void getStringSomeCallback();

void getCountCallback();

void getCountArrayCallback();

void getDirectionCallback();

void getDirectionArrayCallback();

void checkModeCallback();

}
#endif
