// ----------------------------------------------------------------------------
// Constants.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Constants.h"


namespace constants
{
CONSTANT_STRING(device_name,"modular_server");
CONSTANT_STRING(firmware_name,"minimal_device");
// Use semantic versioning http://semver.org/
const long firmware_major = 0;
const long firmware_minor = 1;
const long firmware_patch = 0;

const size_t baudrate = 9600;

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

const size_t model_number = 0;
CONSTANT_STRING(hardware_name,"");

#elif defined(__MK20DX128__) || defined(__MK20DX256__)

const size_t model_number = 0;
CONSTANT_STRING(hardware_name,"");

#else

const size_t model_number = 0;
CONSTANT_STRING(hardware_name,"");

#endif

// Fields

// Parameters

// Methods

// Errors
}
