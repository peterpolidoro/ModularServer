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
enum{FIELD_COUNT_MAX=2};
enum{PARAMETER_COUNT_MAX=1};
enum{METHOD_COUNT_MAX=6};

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
extern ConstantString bool_field_name;
extern const bool bool_default;

extern ConstantString long_array_field_name;
enum{LONG_ARRAY_LENGTH=4};
extern const long long_array_default[LONG_ARRAY_LENGTH];
extern const long long_array_element_min;
extern const long long_array_element_max;

// Parameters
extern ConstantString long_array_parameter_name;

// Methods
extern ConstantString get_bool_method_name;
extern ConstantString get_long_array_fixed_method_name;
extern ConstantString get_long_array_variable_method_name;
extern ConstantString set_long_array_fixed_method_name;
extern ConstantString set_long_array_variable_method_name;
extern ConstantString set_long_array_parameter_method_name;

// Errors
extern ConstantString index_error;

}
#endif
