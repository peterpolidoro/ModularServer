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
extern const int baudrate;

extern const int model_number;
extern const int firmware_number;

extern const int count_min;
extern const int count_max;

extern const int starting_chars_count_min;
extern const int starting_chars_count_max;
extern const int starting_chars_count_default;

extern _FLASH_STRING device_name;
extern _FLASH_STRING echo_method_name;
extern _FLASH_STRING length_method_name;
extern _FLASH_STRING starts_with_method_name;
extern _FLASH_STRING repeat_method_name;
extern _FLASH_STRING chars_at_method_name;
extern _FLASH_STRING starting_chars_method_name;
extern _FLASH_STRING set_starting_chars_count_method_name;
extern _FLASH_STRING get_starting_chars_count_method_name;
extern _FLASH_STRING string_parameter_name;
extern _FLASH_STRING string2_parameter_name;
extern _FLASH_STRING count_parameter_name;
extern _FLASH_STRING index_array_parameter_name;
extern _FLASH_STRING starting_chars_count_name;
}
#endif
