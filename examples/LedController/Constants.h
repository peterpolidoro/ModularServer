// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "Flash.h"

namespace constants
{
extern const int led_pin;
extern const int baudrate;

extern const int model_number;
extern const int firmware_number;

extern const double duration_min;
extern const double duration_max;
extern const long count_min;
extern const long count_max;

extern _FLASH_STRING device_name;
extern _FLASH_STRING led_on_method_name;
extern _FLASH_STRING led_off_method_name;
extern _FLASH_STRING get_led_pin_method_name;
extern _FLASH_STRING blink_led_method_name;
extern _FLASH_STRING duration_on_parameter_name;
extern _FLASH_STRING duration_off_parameter_name;
extern _FLASH_STRING count_parameter_name;
extern _FLASH_STRING seconds_unit;
}
#endif
