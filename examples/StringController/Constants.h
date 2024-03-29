// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef STRING_CONTROLLER_CONSTANTS_H
#define STRING_CONTROLLER_CONSTANTS_H
#include <ConstantVariable.h>
#include <ModularServer.h>


namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{PROPERTY_COUNT_MAX=2};
enum{PARAMETER_COUNT_MAX=6};
enum{FUNCTION_COUNT_MAX=9};
enum{CALLBACK_COUNT_MAX=1};

enum{PIN_COUNT_MAX=1};

extern const long baud;

extern ConstantString device_name;

extern ConstantString firmware_name;
extern const modular_server::FirmwareInfo firmware_info;

extern ConstantString form_factor;
extern ConstantString hardware_name;
extern const modular_server::HardwareInfo hardware_info;

// Units
extern ConstantString characters_units;

// Properties
// Property values must be long, double, bool, long[], double[], bool[], char[], ConstantString *, (ConstantString *)[]
extern const long serial_number_default_new;

extern ConstantString starting_chars_count_property_name;
extern const long starting_chars_count_min;
extern const long starting_chars_count_max;
extern const long starting_chars_count_default;

extern ConstantString stored_string_property_name;
enum{STORED_STRING_LENGTH_MAX=30};
extern const char stored_string_default[STORED_STRING_LENGTH_MAX];

// Parameters
extern ConstantString string_parameter_name;

extern ConstantString string2_parameter_name;

extern ConstantString count_parameter_name;
extern const int count_min;
extern const int count_max;

extern ConstantString index_array_parameter_name;
extern const int index_array_element_min;
extern const int index_array_element_max;
extern const size_t index_array_length_min;
extern const size_t index_array_length_max;

extern ConstantString double_echo_parameter_name;

extern ConstantString array_to_echo_parameter_name;
constexpr size_t array_to_echo_length_min = 1;
constexpr size_t array_to_echo_length_max = 20;

// Functions
extern ConstantString echo_function_name;
extern ConstantString length_function_name;
extern ConstantString starts_with_function_name;
extern ConstantString repeat_function_name;
extern ConstantString chars_at_function_name;
extern ConstantString starting_chars_function_name;
extern ConstantString set_stored_string_function_name;
extern ConstantString get_stored_string_function_name;
extern ConstantString echo_array_function_name;

// Errors
extern ConstantString index_error;

}
#endif
