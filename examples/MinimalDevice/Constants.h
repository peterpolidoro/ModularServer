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


namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{FIELD_COUNT_MAX=1};
enum{PARAMETER_COUNT_MAX=1};
enum{METHOD_COUNT_MAX=1};

extern const unsigned int baudrate;

extern const unsigned int model_number;

extern const unsigned char firmware_major;
extern const unsigned char firmware_minor;
extern const unsigned char firmware_patch;

extern ConstantString device_name;

// Fields

// Parameters

// Methods

// Errors
}
#endif
