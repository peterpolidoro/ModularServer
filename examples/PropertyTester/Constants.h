// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef PROPERTY_TESTER_CONSTANTS_H
#define PROPERTY_TESTER_CONSTANTS_H
#include "ConstantVariable.h"
#include "ModularServer.h"


namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{PROPERTY_COUNT_MAX=10};
enum{PARAMETER_COUNT_MAX=6};
enum{FUNCTION_COUNT_MAX=15};
enum{CALLBACK_COUNT_MAX=1};

enum{INTERRUPT_COUNT_MAX=1};

extern const long baudrate;

extern ConstantString device_name;

extern ConstantString firmware_name;
extern const modular_server::FirmwareInfo firmware_info;

extern ConstantString form_factor;
extern ConstantString hardware_name;
extern const modular_server::HardwareInfo hardware_info;

// Interrupts

// Units

// Properties
// Property values must be long, double, bool, long[], double[], bool[], char[], ConstantString *, (ConstantString *)[]
extern ConstantString double_property_name;
extern const double double_default;

extern ConstantString bool_property_name;
extern const bool bool_default;

extern ConstantString long_array_property_name;
enum{LONG_ARRAY_LENGTH=4};
extern const long long_array_default[LONG_ARRAY_LENGTH];
extern const long long_array_element_min;
extern const long long_array_element_max;

extern ConstantString double_array_property_name;
enum{DOUBLE_ARRAY_LENGTH=3};
extern const double double_array_default[DOUBLE_ARRAY_LENGTH];
extern const double double_array_element_min;
extern const double double_array_element_max;

extern ConstantString bool_array_property_name;
enum{BOOL_ARRAY_LENGTH=2};
extern const bool bool_array_default[BOOL_ARRAY_LENGTH];

extern ConstantString string_property_name;
enum{STRING_LENGTH_MAX=10};
extern const char string_default[STRING_LENGTH_MAX];

extern ConstantString odd_property_name;
enum{ODD_SUBSET_LENGTH=5};
extern modular_server::SubsetMemberType odd_subset[ODD_SUBSET_LENGTH];
extern const long & odd_default;

extern ConstantString mode_property_name;
enum{MODE_SUBSET_LENGTH=3};
extern ConstantString mode_rising;
extern ConstantString mode_falling;
extern ConstantString mode_change;
extern modular_server::SubsetMemberType mode_ptr_subset[MODE_SUBSET_LENGTH];
extern const ConstantString * const mode_ptr_default;

extern ConstantString mode_array_property_name;
enum{MODE_ARRAY_LENGTH=2};
extern const ConstantString * const mode_array_default[MODE_ARRAY_LENGTH];

extern ConstantString odd_array_property_name;
enum{ODD_ARRAY_LENGTH=2};
extern const long odd_array_default[ODD_ARRAY_LENGTH];

// Parameters
extern ConstantString long_array_parameter_name;

extern ConstantString length_parameter_name;
extern const long length_min;
extern const long length_max;

extern ConstantString count_parameter_name;
enum{COUNT_SUBSET_LENGTH=7};
extern modular_server::SubsetMemberType count_subset[COUNT_SUBSET_LENGTH];

extern ConstantString count_array_parameter_name;
extern const size_t count_array_element_min;
extern const size_t count_array_element_max;
extern const size_t count_array_length_min;
extern const size_t count_array_length_max;

extern ConstantString direction_parameter_name;
enum{DIRECTION_SUBSET_LENGTH=4};
extern ConstantString direction_up;
extern ConstantString direction_down;
extern ConstantString direction_left;
extern ConstantString direction_right;
extern modular_server::SubsetMemberType direction_ptr_subset[DIRECTION_SUBSET_LENGTH];

extern ConstantString direction_array_parameter_name;
extern const size_t direction_array_length_min;
extern const size_t direction_array_length_max;

// Functions
extern ConstantString get_doubled_function_name;
extern ConstantString get_bool_function_name;
extern ConstantString get_long_array_fixed_function_name;
extern ConstantString get_long_array_variable_function_name;
extern ConstantString set_long_array_fixed_function_name;
extern ConstantString set_long_array_variable_function_name;
extern ConstantString set_long_array_parameter_function_name;
extern ConstantString get_string_all_function_name;
extern ConstantString get_string_some_function_name;
extern ConstantString get_count_function_name;
extern ConstantString get_count_array_function_name;
extern ConstantString get_direction_function_name;
extern ConstantString get_direction_array_function_name;
extern ConstantString check_mode_function_name;
extern ConstantString increment_mode_function_name;

// Callbacks

// Errors
extern ConstantString index_error;

}
#endif
