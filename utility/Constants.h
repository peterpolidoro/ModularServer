// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_DEVICE_CONSTANTS_H_
#define _MODULAR_DEVICE_CONSTANTS_H_
#include "ConstantVariable.h"


namespace ModularDevice
{
namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{INTERNAL_FIELD_COUNT_MAX=1};
enum{INTERNAL_PARAMETER_COUNT_MAX=3};
enum{INTERNAL_METHOD_COUNT_MAX=15};

enum {METHOD_PARAMETER_COUNT_MAX=6};

enum{SERVER_STREAM_COUNT_MAX=4};

enum{STRING_LENGTH_REQUEST=257};
enum{STRING_LENGTH_ERROR=257};
enum{STRING_LENGTH_DEVICE_NAME=32};
enum{STRING_LENGTH_PARAMETER_NAME=24};
enum{STRING_LENGTH_PARAMETER_UNITS=8};
enum{STRING_LENGTH_PARAMETER_COUNT=2};
enum{STRING_LENGTH_METHOD_NAME=32};
enum{STRING_LENGTH_SUBSET=257};
enum{STRING_LENGTH_SUBSET_ELEMENT=12};
enum{SUBSET_ELEMENT_COUNT_MAX=20};

enum {JSON_TOKEN_MAX=32};

union NumberType
{
  long l;
  double d;
};

union SubsetMemberType
{
  const long l;
  ConstantString * const cs_ptr;
};

// Fields
extern ConstantString serial_number_field_name;

extern const long serial_number_min;
extern const long serial_number_max;
extern const long serial_number_default;

// Parameters
extern ConstantString field_name_parameter_name;
extern ConstantString field_value_parameter_name;
extern ConstantString field_element_index_parameter_name;

// Methods
extern ConstantString get_device_info_method_name;
extern ConstantString get_method_ids_method_name;
extern ConstantString get_parameters_method_name;
extern ConstantString help_method_name;
extern ConstantString verbose_help_method_name;
extern ConstantString get_memory_free_method_name;
extern ConstantString get_field_default_values_method_name;
extern ConstantString set_fields_to_defaults_method_name;
extern ConstantString set_field_to_default_method_name;
extern ConstantString get_field_values_method_name;
extern ConstantString get_field_value_method_name;
extern ConstantString get_field_element_value_method_name;
extern ConstantString set_field_value_method_name;
extern ConstantString set_field_element_value_method_name;
extern ConstantString set_all_field_element_values_method_name;

// Errors
extern ConstantString parse_error_message;
extern ConstantString invalid_request_error_message;
extern ConstantString method_not_found_error_message;
extern ConstantString invalid_params_error_message;
extern ConstantString internal_error_error_message;
extern ConstantString server_error_error_message;

extern ConstantString object_request_error_data;
extern ConstantString request_length_error_data;
extern ConstantString parameter_not_found_error_data;
extern ConstantString field_not_found_error_data;
extern ConstantString field_not_array_type_error_data;
extern ConstantString field_element_index_out_of_bounds_error_data;
extern ConstantString incorrect_parameter_number_error_data;
extern ConstantString invalid_json_object_error_data;
extern ConstantString invalid_json_array_error_data;
extern ConstantString parameter_error_error_data;
extern ConstantString array_parameter_error_error_data;
extern ConstantString array_parameter_length_error_error_data;
extern ConstantString value_not_in_subset_error_data;
extern ConstantString value_not_in_range_error_data;

extern const int parse_error_code;
extern const int invalid_request_error_code;
extern const int method_not_found_error_code;
extern const int invalid_params_error_code;
extern const int internal_error_error_code;
extern const int server_error_error_code;

// Saved Variables
extern const size_t eeprom_initialized_default_value;

// Constant Strings
extern ConstantString empty_constant_string;
extern ConstantString id_constant_string;
extern ConstantString error_constant_string;
extern ConstantString message_constant_string;
extern ConstantString data_constant_string;
extern ConstantString code_constant_string;
extern ConstantString device_name_constant_string;
extern ConstantString firmware_name_constant_string;
extern ConstantString hardware_name_constant_string;
extern ConstantString name_constant_string;
extern ConstantString type_constant_string;
extern ConstantString units_constant_string;
extern ConstantString result_constant_string;
extern ConstantString result_type_constant_string;
extern ConstantString array_element_type_constant_string;
extern ConstantString methods_constant_string;
extern ConstantString parameters_constant_string;
extern ConstantString fields_constant_string;
extern ConstantString min_constant_string;
extern ConstantString max_constant_string;
extern ConstantString array_element_min_constant_string;
extern ConstantString array_element_max_constant_string;
extern ConstantString array_length_min_constant_string;
extern ConstantString array_length_max_constant_string;
extern ConstantString model_number_constant_string;
extern ConstantString firmware_version_constant_string;
extern ConstantString major_constant_string;
extern ConstantString minor_constant_string;
extern ConstantString patch_constant_string;
extern ConstantString device_info_constant_string;
extern ConstantString value_constant_string;
extern ConstantString default_value_constant_string;
extern ConstantString question_constant_string;
extern ConstantString question_double_constant_string;
extern ConstantString zero_constant_string;
extern ConstantString given_constant_string;
extern ConstantString needed_constant_string;
extern ConstantString less_than_equal_constant_string;
extern ConstantString element_constant_string;
extern ConstantString array_length_constant_string;
extern ConstantString array_open_constant_string;
extern ConstantString array_close_constant_string;
extern ConstantString array_separator_constant_string;
extern ConstantString quote_constant_string;
extern ConstantString subset_constant_string;
extern ConstantString array_element_subset_constant_string;
extern ConstantString processor_constant_string;
extern ConstantString processor_type_constant_string;
}
}
#endif
