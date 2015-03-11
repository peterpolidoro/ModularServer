// ----------------------------------------------------------------------------
// Constants.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Constants.h"

namespace ModularDevice
{
namespace constants
{
const char eol = '\n';
const char start_char_json_object = '{';
const char start_char_json_array = '[';

const int method_count_max = 24;

const int double_digits = 4;

const int response_indent = 2;

const unsigned char eeprom_initialized_value = 123;

const unsigned int serial_number_min = 0;
const unsigned int serial_number_max = 65535;
const unsigned int serial_number_default = 0;
}
}
