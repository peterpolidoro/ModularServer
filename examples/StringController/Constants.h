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
enum{FIELD_COUNT_MAX=1};
enum{PARAMETER_COUNT_MAX=6};
enum{METHOD_COUNT_MAX=11};

extern const unsigned int baudrate;

extern const unsigned int model_number;

extern const unsigned char firmware_major;
extern const unsigned char firmware_minor;
extern const unsigned char firmware_patch;

extern HardwareSerial& serial2;
extern const unsigned char serial2_rx_pin;

extern const int count_min;
extern const int count_max;

extern const int starting_chars_count_min;
extern const int starting_chars_count_max;
extern const int starting_chars_count_default;

extern const int index_array_element_min;
extern const int index_array_element_max;

extern ConstantString index_error;

extern ConstantString device_name;

extern ConstantString string_parameter_name;
extern ConstantString string2_parameter_name;
extern ConstantString count_parameter_name;
extern ConstantString index_array_parameter_name;
extern ConstantString double_echo_parameter_name;
extern ConstantString starting_chars_count_name;

extern ConstantString echo_method_name;
extern ConstantString length_method_name;
extern ConstantString starts_with_method_name;
extern ConstantString repeat_method_name;
extern ConstantString chars_at_method_name;
extern ConstantString starting_chars_method_name;
extern ConstantString set_starting_chars_count_method_name;
extern ConstantString get_starting_chars_count_method_name;
extern ConstantString set_stored_string_method_name;
extern ConstantString get_stored_string_method_name;

}
#endif
