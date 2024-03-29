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

CONSTANT_STRING(device_name,"string_controller");

CONSTANT_STRING(firmware_name,"StringController");
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

// Pins

// Units
CONSTANT_STRING(characters_units,"characters");

// Properties
const long serial_number_default_new = 77;

CONSTANT_STRING(starting_chars_count_property_name,"startingCharsCount");
const long starting_chars_count_min = 1;
const long starting_chars_count_max = 10;
const long starting_chars_count_default = 2;

CONSTANT_STRING(stored_string_property_name,"storedString");
const char stored_string_default[STORED_STRING_LENGTH_MAX] = "I am a stored string.";

// Parameters
CONSTANT_STRING(string_parameter_name,"string");

CONSTANT_STRING(string2_parameter_name,"string2");

CONSTANT_STRING(count_parameter_name,"count");
const int count_min = 1;
const int count_max = 100;

CONSTANT_STRING(index_array_parameter_name,"index_array");
const int index_array_element_min = 0;
const int index_array_element_max = 10;
const size_t index_array_length_min = 1;
const size_t index_array_length_max = 3;

CONSTANT_STRING(double_echo_parameter_name,"double_echo");

CONSTANT_STRING(array_to_echo_parameter_name,"array_to_echo");

// Functions
CONSTANT_STRING(echo_function_name,"echo");
CONSTANT_STRING(length_function_name,"length");
CONSTANT_STRING(starts_with_function_name,"startsWith");
CONSTANT_STRING(repeat_function_name,"repeat");
CONSTANT_STRING(chars_at_function_name,"charsAt");
CONSTANT_STRING(starting_chars_function_name,"startingChars");
CONSTANT_STRING(set_stored_string_function_name,"setStoredString");
CONSTANT_STRING(get_stored_string_function_name,"getStoredString");
CONSTANT_STRING(echo_array_function_name,"echoArray");

// Callbacks

// Errors
CONSTANT_STRING(index_error,"Invalid index.");

}
