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

FLASH_STRING(device_name,"led_controller");
FLASH_STRING(led_on_method_name,"setLedOn");
FLASH_STRING(led_off_method_name,"setLedOff");
FLASH_STRING(get_led_pin_method_name,"getLedPin");
FLASH_STRING(blink_led_method_name,"blinkLed");
FLASH_STRING(duration_on_parameter_name,"duration_on");
FLASH_STRING(duration_off_parameter_name,"duration_off");
FLASH_STRING(count_parameter_name,"count");
FLASH_STRING(seconds_unit,"seconds");
}
