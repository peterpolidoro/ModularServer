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
const long baud = 115200;

CONSTANT_STRING(device_name,"callback_tester");

CONSTANT_STRING(firmware_name,"CallbackTester");
// Use semantic versioning http://semver.org/
const modular_server::FirmwareInfo firmware_info =
{
  .name_ptr=&firmware_name,
  .version_major=2,
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

const size_t bnc_a_pin_number = 2;
const size_t bnc_b_pin_number = 3;
const size_t bnc_c_pin_number = 18;
const size_t bnc_d_pin_number = 19;
const size_t bnc_e_pin_number = 20;
const size_t bnc_f_pin_number = 21;

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

const size_t bnc_a_pin_number = 33;
const size_t bnc_b_pin_number = 32;
const size_t bnc_c_pin_number = 2;
const size_t bnc_d_pin_number = 3;
const size_t bnc_e_pin_number = 4;
const size_t bnc_f_pin_number = 5;

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

const size_t bnc_a_pin_number = 0;
const size_t bnc_b_pin_number = 1;
const size_t bnc_c_pin_number = 2;
const size_t bnc_d_pin_number = 3;
const size_t bnc_e_pin_number = 4;
const size_t bnc_f_pin_number = 5;

#endif

const size_t led_pin_number = 13;

// Pins
CONSTANT_STRING(bnc_a_pin_name,"bnc_a");
CONSTANT_STRING(bnc_b_pin_name,"bnc_b");
CONSTANT_STRING(bnc_c_pin_name,"bnc_c");
CONSTANT_STRING(bnc_d_pin_name,"bnc_d");
CONSTANT_STRING(bnc_e_pin_name,"bnc_e");
CONSTANT_STRING(bnc_f_pin_name,"bnc_f");

// Units
CONSTANT_STRING(seconds_unit,"seconds");

// Properties
CONSTANT_STRING(duration_on_property_name,"durationOn");
const double duration_min = 0.1;
const double duration_max = 2.5;
const double duration_on_default = duration_min;

CONSTANT_STRING(duration_off_property_name,"durationOff");
const double duration_off_default = duration_min;

CONSTANT_STRING(count_property_name,"count");
const long count_min = 1;
const long count_max = 20;
const long count_default = 10;

// Parameters

// Functions

// Callbacks
CONSTANT_STRING(set_led_on_callback_name,"setLedOn");
CONSTANT_STRING(set_led_off_callback_name,"setLedOff");
CONSTANT_STRING(blink_led_callback_name,"blinkLed");

// Errors
}
