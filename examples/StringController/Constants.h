// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "ConstantVariable.h"

namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{FIELD_COUNT_MAX=4};
enum{PARAMETER_COUNT_MAX=6};
enum{METHOD_COUNT_MAX=12};

extern const unsigned int baudrate;

extern const unsigned int model_number;

extern const unsigned char firmware_major;
extern const unsigned char firmware_minor;
extern const unsigned char firmware_patch;

extern HardwareSerial& serial2;
extern const unsigned char serial2_rx_pin;

extern ConstantString device_name;

// Fields
// Field values must be long, bool, long[], or bool[]
extern ConstantString starting_chars_count_field_name;
extern const long starting_chars_count_min;
extern const long starting_chars_count_max;
extern const long starting_chars_count_default;

extern ConstantString bool_field_name;
extern const bool bool_default;

extern ConstantString long_array_field_name;
enum{LONG_ARRAY_LENGTH=4};
extern const long long_array_default[LONG_ARRAY_LENGTH];
extern const long long_array_element_min;
extern const long long_array_element_max;

// Parameters
extern ConstantString string_parameter_name;

extern ConstantString string2_parameter_name;

extern ConstantString count_parameter_name;
extern const int count_min;
extern const int count_max;

extern ConstantString index_array_parameter_name;
extern const int index_array_element_min;
extern const int index_array_element_max;
extern const unsigned char index_array_length_min;
extern const unsigned char index_array_length_max;

extern ConstantString double_echo_parameter_name;

extern ConstantString long_array_parameter_name;

// Methods
extern ConstantString echo_method_name;
extern ConstantString length_method_name;
extern ConstantString starts_with_method_name;
extern ConstantString repeat_method_name;
extern ConstantString chars_at_method_name;
extern ConstantString starting_chars_method_name;
extern ConstantString set_stored_string_method_name;
extern ConstantString get_stored_string_method_name;
extern ConstantString get_bool_method_name;
extern ConstantString get_long_array_method_name;
extern ConstantString set_long_array_method_name;
extern ConstantString get_long_array_variable_method_name;

// Errors
extern ConstantString index_error;

}
#endif
