// ----------------------------------------------------------------------------
// Constants.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "Constants.h"


namespace constants
{
const long baud = 115200;

CONSTANT_STRING(device_name,"pin_tester");

CONSTANT_STRING(firmware_name,"PinTester");
// Use semantic versioning http://semver.org/
const modular_server::FirmwareInfo firmware_info =
{
  .name_ptr=&firmware_name,
  .version_major=1,
  .version_minor=0,
  .version_patch=0,
};

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

CONSTANT_STRING(form_factor,"5x3");
CONSTANT_STRING(hardware_name,"Mega2560");
const modular_server::HardwareInfo hardware_info =
{
  .name_ptr=&hardware_name,
  .part_number=0,
  .version_major=0,
  .version_minor=0,
};

#elif defined(__MK20DX128__) || defined(__MK20DX256__)

CONSTANT_STRING(form_factor,"3x2");
CONSTANT_STRING(hardware_name,"Teensy");
const modular_server::HardwareInfo hardware_info =
{
  .name_ptr=&hardware_name,
  .part_number=0,
  .version_major=3,
  .version_minor=2,
};

#else

CONSTANT_STRING(form_factor,"");
CONSTANT_STRING(hardware_name,"");
const modular_server::HardwareInfo hardware_info =
{
  .name_ptr=&hardware_name,
  .part_number=0,
  .version_major=0,
  .version_minor=0,
};

#endif

const long pwm_period_ms = 1000;
const long pwm_on_duration_ms = 200;
const long pwm_count = 1;

// Pins
CONSTANT_STRING(pin_a_pin_name,"pin_a");
const size_t pin_a_pin_number = 22;

CONSTANT_STRING(pin_b_pin_name,"pin_b");
const size_t pin_b_pin_number = 4;

CONSTANT_STRING(pin_c_pin_name,"pin_c");
const size_t pin_c_pin_number = 3;

CONSTANT_STRING(pin_d_pin_name,"pin_d");
const size_t pin_d_pin_number = 2;

// Units

// Properties

// Parameters

// Functions

// Callbacks
CONSTANT_STRING(test_pin_callback_name,"testPin");

// Errors
}
