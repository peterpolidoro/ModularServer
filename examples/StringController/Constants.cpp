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
const int baudrate = 9600;

const int model_number = 1002;
const int firmware_number = 1;

const int count_min = 1;
const int count_max = 100;

const int starting_chars_count_min = 1;
const int starting_chars_count_max = 10;
const int starting_chars_count_default = 2;

FLASH_STRING(device_name,"string_controller");
FLASH_STRING(echo_method_name,"echo");
FLASH_STRING(length_method_name,"length");
FLASH_STRING(starts_with_method_name,"startsWith");
FLASH_STRING(repeat_method_name,"repeat");
FLASH_STRING(chars_at_method_name,"charsAt");
FLASH_STRING(starting_chars_method_name,"startingChars");
FLASH_STRING(set_starting_chars_count_method_name,"setStartingCharsCount");
FLASH_STRING(get_starting_chars_count_method_name,"getStartingCharsCount");
FLASH_STRING(set_stored_string_method_name,"setStoredString");
FLASH_STRING(get_stored_string_method_name,"getStoredString");
FLASH_STRING(string_parameter_name,"string");
FLASH_STRING(string2_parameter_name,"string2");
FLASH_STRING(count_parameter_name,"count");
FLASH_STRING(index_array_parameter_name,"index_array");
FLASH_STRING(starting_chars_count_name,"starting_chars_count");
}
