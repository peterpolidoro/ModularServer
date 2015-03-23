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
const int led_pin = 13;
const int baudrate = 9600;

const int model_number = 1001;
const int firmware_number = 1;

const double duration_min = 0.1;
const double duration_max = 2.5;
const long count_min = 1;
const long count_max = 100;

CONSTANT_STRING(device_name,"led_controller");
CONSTANT_STRING(led_on_method_name,"setLedOn");
CONSTANT_STRING(led_off_method_name,"setLedOff");
CONSTANT_STRING(get_led_pin_method_name,"getLedPin");
CONSTANT_STRING(blink_led_method_name,"blinkLed");
CONSTANT_STRING(duration_on_parameter_name,"duration_on");
CONSTANT_STRING(duration_off_parameter_name,"duration_off");
CONSTANT_STRING(count_parameter_name,"count");
CONSTANT_STRING(seconds_unit,"seconds");
}
