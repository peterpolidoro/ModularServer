// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_CONSTANTS_H_
#define _MODULAR_SERVER_CONSTANTS_H_
#include "ConstantVariable.h"


namespace modular_server
{
namespace constants
{
enum {FIRMWARE_COUNT_MAX=8};
enum {HARDWARE_INFO_ARRAY_COUNT_MAX=4};

//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{SERVER_PROPERTY_COUNT_MAX=1};
enum{SERVER_PARAMETER_COUNT_MAX=4};
enum{SERVER_METHOD_COUNT_MAX=17};
enum{SERVER_CALLBACK_COUNT_MAX=1};

enum {METHOD_PARAMETER_COUNT_MAX=6};
enum {CALLBACK_PROPERTY_COUNT_MAX=6};

enum{SERVER_STREAM_COUNT_MAX=4};

enum{STRING_LENGTH_REQUEST=257};
enum{STRING_LENGTH_ERROR=257};
enum{STRING_LENGTH_PARAMETER_COUNT=3};
enum{STRING_LENGTH_SUBSET=257};
enum{STRING_LENGTH_SUBSET_ELEMENT=32};
enum{STRING_LENGTH_VERSION=18};
enum{STRING_LENGTH_VERSION_PROPERTY=6};
enum{SUBSET_ELEMENT_COUNT_MAX=20};

enum {JSON_TOKEN_MAX=32};
enum {JSON_BUFFER_SIZE=32};

struct FirmwareInfo
{
  const ConstantString * const name_ptr;
  const size_t version_major;
  const size_t version_minor;
  const size_t version_patch;
};

struct HardwareInfo
{
  const ConstantString * const name_ptr;
  const size_t part_number;
  const size_t version_major;
  const size_t version_minor;
};

union NumberType
{
  long l;
  double d;
};

union SubsetMemberType
{
  long l;
  const ConstantString * cs_ptr;
};

extern ConstantString firmware_name;
extern const FirmwareInfo firmware_info;

// Properties
extern ConstantString serial_number_property_name;

extern const long serial_number_min;
extern const long serial_number_max;
extern const long serial_number_default;

// Parameters
extern ConstantString property_name_parameter_name;
extern ConstantString property_value_parameter_name;
extern ConstantString property_element_index_parameter_name;

// Methods
extern ConstantString get_procedure_ids_method_name;
extern ConstantString help_method_name;
extern ConstantString verbose_help_method_name;
extern ConstantString get_device_id_method_name;
extern ConstantString get_device_info_method_name;
extern ConstantString get_api_method_name;
extern ConstantString get_api_verbose_method_name;
extern ConstantString get_memory_free_method_name;
extern ConstantString get_property_default_values_method_name;
extern ConstantString set_properties_to_defaults_method_name;
extern ConstantString set_property_to_default_method_name;
extern ConstantString get_property_values_method_name;
extern ConstantString get_property_value_method_name;
extern ConstantString get_property_element_value_method_name;
extern ConstantString set_property_value_method_name;
extern ConstantString set_property_element_value_method_name;
extern ConstantString set_all_property_element_values_method_name;

// Callbacks

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
extern ConstantString property_not_found_error_data;
extern ConstantString property_not_array_type_error_data;
extern ConstantString property_element_index_out_of_bounds_error_data;
extern ConstantString cannot_set_element_in_string_property_with_subset_error_data;
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
extern ConstantString form_factor_constant_string;
extern ConstantString firmware_constant_string;
extern ConstantString hardware_constant_string;
extern ConstantString name_constant_string;
extern ConstantString type_constant_string;
extern ConstantString units_constant_string;
extern ConstantString result_constant_string;
extern ConstantString result_type_constant_string;
extern ConstantString array_element_type_constant_string;
extern ConstantString properties_constant_string;
extern ConstantString parameters_constant_string;
extern ConstantString methods_constant_string;
extern ConstantString callbacks_constant_string;
extern ConstantString min_constant_string;
extern ConstantString max_constant_string;
extern ConstantString array_element_min_constant_string;
extern ConstantString array_element_max_constant_string;
extern ConstantString array_length_min_constant_string;
extern ConstantString array_length_max_constant_string;
extern ConstantString version_constant_string;
extern ConstantString part_number_constant_string;
extern ConstantString device_id_constant_string;
extern ConstantString device_info_constant_string;
extern ConstantString api_constant_string;
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
extern ConstantString version_property_separator_constant_string;
extern ConstantString subset_constant_string;
extern ConstantString all_constant_string;
extern ConstantString array_element_subset_constant_string;
extern ConstantString processor_constant_string;
extern ConstantString processor_name_constant_string;
}
}
#endif
