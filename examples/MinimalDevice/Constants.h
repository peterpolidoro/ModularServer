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
// enum{METHOD_COUNT_MAX=31};
// enum{PARAMETER_COUNT_MAX=16};
// enum{SAVED_VARIABLE_COUNT_MAX=10};
enum{METHOD_COUNT_MAX=1};
enum{PARAMETER_COUNT_MAX=1};
enum{SAVED_VARIABLE_COUNT_MAX=1};

extern const unsigned int baudrate;

extern const unsigned int model_number;

extern const unsigned char firmware_major;
extern const unsigned char firmware_minor;
extern const unsigned char firmware_patch;

extern ConstantString device_name;
}
#endif
