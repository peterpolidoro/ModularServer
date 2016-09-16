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
const size_t led_pin = 13;

const size_t baudrate = 9600;

const size_t model_number = 1001;

// Use semantic versioning http://semver.org/
const long firmware_major = 0;
const long firmware_minor = 1;
const long firmware_patch = 0;

HardwareSerial & serial2 = Serial2;
const size_t serial2_rx_pin = 17;

CONSTANT_STRING(device_name,"board_led_controller");

// Fields

// Parameters
CONSTANT_STRING(duration_on_parameter_name,"duration_on");
const double duration_min = 0.1;
const double duration_max = 2.5;

CONSTANT_STRING(duration_off_parameter_name,"duration_off");

CONSTANT_STRING(count_parameter_name,"count");
const long count_min = 1;
const long count_max = 100;

CONSTANT_STRING(seconds_unit,"seconds");

// Methods
CONSTANT_STRING(led_on_method_name,"setLedOn");
CONSTANT_STRING(led_off_method_name,"setLedOff");
CONSTANT_STRING(get_led_pin_method_name,"getLedPin");
CONSTANT_STRING(blink_led_method_name,"blinkLed");

// Errors
}
