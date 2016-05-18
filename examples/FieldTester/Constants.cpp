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
const unsigned int baudrate = 9600;

const unsigned int model_number = 1003;

// Use semantic versioning http://semver.org/
const unsigned char firmware_major = 0;
const unsigned char firmware_minor = 1;
const unsigned char firmware_patch = 0;

HardwareSerial& serial2 = Serial2;
const unsigned char serial2_rx_pin = 17;

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

CONSTANT_STRING(char_array_field_name,"char_array");
const char char_array_default[CHAR_ARRAY_LENGTH_MAX] = "abcdef";

// Parameters
CONSTANT_STRING(long_array_parameter_name,"long_array_parameter");

// Methods
CONSTANT_STRING(get_doubled_method_name,"getDoubled");
CONSTANT_STRING(get_bool_method_name,"getBool");
CONSTANT_STRING(get_long_array_fixed_method_name,"getLongArrayFixed");
CONSTANT_STRING(get_long_array_variable_method_name,"getLongArrayVariable");
CONSTANT_STRING(set_long_array_fixed_method_name,"setLongArrayFixed");
CONSTANT_STRING(set_long_array_variable_method_name,"setLongArrayVariable");
CONSTANT_STRING(set_long_array_parameter_method_name,"setLongArrayParameter");
CONSTANT_STRING(get_char_array_method_name,"getCharArray");

// Errors
CONSTANT_STRING(index_error,"Invalid index.");

}
