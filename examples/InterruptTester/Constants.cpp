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
const long baudrate = 115200;

CONSTANT_STRING(device_name,"minimal_device");

CONSTANT_STRING(firmware_name,"InterruptTester");
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

const size_t bnc_a_pin = 2;
const size_t bnc_b_pin = 3;

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

const size_t bnc_a_pin = 33;
const size_t bnc_b_pin = 32;

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

const size_t bnc_a_pin = 0;
const size_t bnc_b_pin = 1;

#endif

const size_t led_pin = 13;

// Interrupts
CONSTANT_STRING(bnc_a_interrupt_name,"bnc_a");
CONSTANT_STRING(bnc_b_interrupt_name,"bnc_b");

// Units

// Properties

// Parameters

// Functions

// Callbacks
CONSTANT_STRING(set_led_on_callback_name,"setLedOn");
CONSTANT_STRING(set_led_off_callback_name,"setLedOff");

// Errors
}
