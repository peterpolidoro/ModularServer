// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "ConstantVariable.h"
#include "ModularServer.h"


namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{FIELD_COUNT_MAX=1};
enum{PARAMETER_COUNT_MAX=1};
enum{METHOD_COUNT_MAX=1};

extern ConstantString device_name;
extern ConstantString firmware_name;
extern const long firmware_major;
extern const long firmware_minor;
extern const long firmware_patch;

extern const size_t baudrate;

extern const size_t model_number;
extern ConstantString hardware_name;
extern const long hardware_major;
extern const long hardware_minor;

// Fields
// Field values must be long, double, bool, long[], double[], bool[], char[], ConstantString *

// Parameters

// Methods

// Errors
}
#endif
