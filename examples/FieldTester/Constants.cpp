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
const size_t baudrate = 9600;

const size_t model_number = 1003;

// Use semantic versioning http://semver.org/
const long firmware_major = 0;
const long firmware_minor = 1;
const long firmware_patch = 0;

HardwareSerial& serial2 = Serial2;
const size_t serial2_rx_pin = 17;

CONSTANT_STRING(device_name,"field_tester");

// Fields
CONSTANT_STRING(double_field_name,"double");
const double double_default = 3.14159;

CONSTANT_STRING(bool_field_name,"bool");
const bool bool_default = false;

CONSTANT_STRING(long_array_field_name,"long_array");
const long long_array_default[LONG_ARRAY_LENGTH] = {5,4,3,2};
const long long_array_element_min = -3;
const long long_array_element_max = 10;

CONSTANT_STRING(double_array_field_name,"double_array");
const double double_array_default[DOUBLE_ARRAY_LENGTH] = {-1.1,2.2,3.3};
const double double_array_element_min = -33.333;
const double double_array_element_max = 100.0;

CONSTANT_STRING(bool_array_field_name,"bool_array");
const bool bool_array_default[BOOL_ARRAY_LENGTH] = {false,true};

CONSTANT_STRING(string_field_name,"string");
const char string_default[STRING_LENGTH_MAX] = "abcdef";

CONSTANT_STRING(odd_field_name,"odd");
const long odd_members[ODD_MEMBER_COUNT] = {1,3,5,7,9};
const long &odd_default = odd_members[2];

CONSTANT_STRING(mode_field_name,"mode");
CONSTANT_STRING(mode_rising,"RISING");
CONSTANT_STRING(mode_falling,"FALLING");
CONSTANT_STRING(mode_change,"CHANGE");
const ConstantString * const mode_ptr_members[MODE_MEMBER_COUNT] =
  {
    &mode_rising,
    &mode_falling,
    &mode_change,
  };
ConstantString * const mode_ptr_default = &mode_rising;

// Parameters
CONSTANT_STRING(long_array_parameter_name,"long_array_parameter");

CONSTANT_STRING(length_parameter_name,"length_parameter");
const long length_min = 0;
const long length_max = 20;

// Methods
CONSTANT_STRING(get_doubled_method_name,"getDoubled");
CONSTANT_STRING(get_bool_method_name,"getBool");
CONSTANT_STRING(get_long_array_fixed_method_name,"getLongArrayFixed");
CONSTANT_STRING(get_long_array_variable_method_name,"getLongArrayVariable");
CONSTANT_STRING(set_long_array_fixed_method_name,"setLongArrayFixed");
CONSTANT_STRING(set_long_array_variable_method_name,"setLongArrayVariable");
CONSTANT_STRING(set_long_array_parameter_method_name,"setLongArrayParameter");
CONSTANT_STRING(get_string_all_method_name,"getStringAll");
CONSTANT_STRING(get_string_some_method_name,"getStringSome");

// Errors
CONSTANT_STRING(index_error,"Invalid index.");

}
