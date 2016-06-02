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
enum{FIELD_COUNT_MAX=6};
enum{PARAMETER_COUNT_MAX=2};
enum{METHOD_COUNT_MAX=9};

extern const size_t baudrate;

extern const size_t model_number;

extern const long firmware_major;
extern const long firmware_minor;
extern const long firmware_patch;

extern HardwareSerial& serial2;
extern const size_t serial2_rx_pin;

extern ConstantString device_name;

// Fields
// Field values must be long, double, bool, long[], double[], bool[], char[]
extern ConstantString double_field_name;
extern const double double_default;

extern ConstantString bool_field_name;
extern const bool bool_default;

extern ConstantString long_array_field_name;
enum{LONG_ARRAY_LENGTH=4};
extern const long long_array_default[LONG_ARRAY_LENGTH];
extern const long long_array_element_min;
extern const long long_array_element_max;

extern ConstantString double_array_field_name;
enum{DOUBLE_ARRAY_LENGTH=3};
extern const double double_array_default[DOUBLE_ARRAY_LENGTH];
extern const double double_array_element_min;
extern const double double_array_element_max;

extern ConstantString bool_array_field_name;
enum{BOOL_ARRAY_LENGTH=2};
extern const bool bool_array_default[BOOL_ARRAY_LENGTH];

extern ConstantString string_field_name;
enum{STRING_LENGTH_MAX=10};
extern const char string_default[STRING_LENGTH_MAX];

// Parameters
extern ConstantString long_array_parameter_name;

extern ConstantString length_parameter_name;
extern const long length_min;
extern const long length_max;

// Methods
extern ConstantString get_doubled_method_name;
extern ConstantString get_bool_method_name;
extern ConstantString get_long_array_fixed_method_name;
extern ConstantString get_long_array_variable_method_name;
extern ConstantString set_long_array_fixed_method_name;
extern ConstantString set_long_array_variable_method_name;
extern ConstantString set_long_array_parameter_method_name;
extern ConstantString get_string_all_method_name;
extern ConstantString get_string_some_method_name;

// Errors
extern ConstantString index_error;

}
#endif