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
const size_t baudrate = 9600;

CONSTANT_STRING(device_name,"minimal_device");

CONSTANT_STRING(firmware_name,"MinimalDevice");
// Use semantic versioning http://semver.org/
const Modular::FirmwareInfo firmware_info =
  {
    .name_ptr=&firmware_name,
    .version_major=1,
    .version_minor=0,
    .version_patch=0,
  };

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

CONSTANT_STRING(form_factor,"mega");
CONSTANT_STRING(hardware_name,"mega");
const Modular::HardwareInfo hardware_info =
  {
    .name_ptr=&hardware_name,
    .model_number=0,
    .version_major=0,
    .version_minor=0,
  };

#elif defined(__MK20DX128__) || defined(__MK20DX256__)

CONSTANT_STRING(form_factor,"teensy");
CONSTANT_STRING(hardware_name,"teensy");
const Modular::HardwareInfo hardware_info =
  {
    .name_ptr=&hardware_name,
    .model_number=0,
    .version_major=0,
    .version_minor=0,
  };

#else

CONSTANT_STRING(form_factor,"");
CONSTANT_STRING(hardware_name,"");
const Modular::HardwareInfo hardware_info =
  {
    .name_ptr=&hardware_name,
    .model_number=0,
    .version_major=0,
    .version_minor=0,
  };

#endif

// Fields

// Parameters

// Methods

// Errors
}
