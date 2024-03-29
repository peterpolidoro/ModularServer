// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_CONSTANTS_H_
#define _MODULAR_SERVER_CONSTANTS_H_
#include <ConstantVariable.h>
#include <Array.h>

// #include "Pin.h"


namespace modular_server
{
namespace constants
{
enum {FIRMWARE_COUNT_MAX=8};
enum {HARDWARE_COUNT_MAX=4};

//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{SERVER_PROPERTY_COUNT_MAX=1};
enum{SERVER_PARAMETER_COUNT_MAX=5};
enum{SERVER_FUNCTION_COUNT_MAX=14};
enum{SERVER_CALLBACK_COUNT_MAX=1};

enum {FUNCTION_PARAMETER_COUNT_MAX=8};
enum {CALLBACK_PROPERTY_COUNT_MAX=8};
enum {CALLBACK_PIN_COUNT_MAX=8};
enum {PIN_COUNT_MAX=64};

enum{SERVER_STREAM_COUNT_MAX=4};

enum{JSON_DOCUMENT_SIZE=1024};

enum{STRING_LENGTH_REQUEST=257};
enum{STRING_LENGTH_ERROR=257};
enum{STRING_LENGTH_PARAMETER_COUNT=3};
enum{STRING_LENGTH_SUBSET=257};
enum{STRING_LENGTH_SUBSET_ELEMENT=32};
enum{STRING_LENGTH_VERSION=18};
enum{STRING_LENGTH_VERSION_PROPERTY=6};
enum{SUBSET_ELEMENT_COUNT_MAX=20};

enum {JSON_TOKEN_MAX=32};

enum {FIRMWARE_NAME_JSON_DOCUMENT_SIZE=128};

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

extern const long response_pipe_read_max;

extern const double epsilon;

// Pins
enum{PIN_PWM_EVENT_COUNT_MAX=16};
extern const size_t pin_pulse_timer_number;
extern const uint32_t pin_pulse_delay;
extern const uint32_t pin_pulse_count;
extern ConstantString pin_mode_digital_input;
extern ConstantString pin_mode_digital_input_pullup;
extern ConstantString pin_mode_digital_output;
extern ConstantString pin_mode_analog_input;
extern ConstantString pin_mode_analog_output;
extern ConstantString pin_mode_pulse_rising;
extern ConstantString pin_mode_pulse_falling;
extern ConstantString pin_mode_pwm_rising;
extern ConstantString pin_mode_pwm_falling;
extern ConstantString pin_mode_interrupt_low;
extern ConstantString pin_mode_interrupt_change;
extern ConstantString pin_mode_interrupt_rising;
extern ConstantString pin_mode_interrupt_falling;

// Properties
extern ConstantString serial_number_property_name;
extern const long serial_number_min;
extern const long serial_number_max;
extern const long serial_number_default;

// Parameters
enum{VERBOSITY_SUBSET_LENGTH=3};
extern ConstantString verbosity_names;
extern ConstantString verbosity_general;
extern ConstantString verbosity_detailed;
extern SubsetMemberType verbosity_ptr_subset[VERBOSITY_SUBSET_LENGTH];

extern ConstantString pin_name_parameter_name;

enum{PIN_MODE_SUBSET_LENGTH=9};
extern SubsetMemberType pin_mode_ptr_subset[PIN_MODE_SUBSET_LENGTH];

extern ConstantString pin_value_parameter_name;
extern const long pin_value_min;
extern const long pin_value_max;

// Functions
extern ConstantString get_method_ids_function_name;
extern ConstantString help_function_name;
extern ConstantString verbose_help_function_name;
extern ConstantString get_device_id_function_name;
extern ConstantString get_device_info_function_name;
extern ConstantString get_api_function_name;
extern ConstantString get_property_default_values_function_name;
extern ConstantString set_properties_to_defaults_function_name;
extern ConstantString get_pin_info_function_name;
extern ConstantString get_property_values_function_name;
extern ConstantString set_pin_mode_function_name;
extern ConstantString get_pin_value_function_name;
extern ConstantString set_pin_value_function_name;
extern ConstantString get_memory_free_function_name;

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
extern ConstantString parameter_incorrect_type_error_data;
extern ConstantString property_not_found_error_data;
extern ConstantString property_not_array_type_error_data;
extern ConstantString property_element_index_out_of_bounds_error_data;
extern ConstantString cannot_set_element_in_string_property_with_subset_error_data;
extern ConstantString incorrect_parameter_number_error_data;
extern ConstantString parameter_error_error_data;
extern ConstantString array_parameter_error_error_data;
extern ConstantString array_parameter_length_error_error_data;
extern ConstantString value_not_in_subset_error_data;
extern ConstantString value_not_in_range_error_data;
extern ConstantString property_function_not_found_error_data;
extern ConstantString incorrect_property_parameter_number_error_data;
extern ConstantString callback_function_not_found_error_data;
extern ConstantString incorrect_callback_parameter_number_error_data;

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
extern ConstantString serial_number_constant_string;
extern ConstantString ancestors_constant_string;
extern ConstantString firmware_constant_string;
extern ConstantString hardware_constant_string;
extern ConstantString name_constant_string;
extern ConstantString type_constant_string;
extern ConstantString method_type_constant_string;
extern ConstantString units_constant_string;
extern ConstantString result_constant_string;
extern ConstantString result_info_constant_string;
extern ConstantString array_element_type_constant_string;
extern ConstantString property_constant_string;
extern ConstantString properties_constant_string;
extern ConstantString parameters_constant_string;
extern ConstantString function_constant_string;
extern ConstantString functions_constant_string;
extern ConstantString callback_constant_string;
extern ConstantString callbacks_constant_string;
extern ConstantString min_constant_string;
extern ConstantString max_constant_string;
extern ConstantString array_element_min_constant_string;
extern ConstantString array_element_max_constant_string;
extern ConstantString array_length_min_constant_string;
extern ConstantString array_length_max_constant_string;
extern ConstantString string_length_max_constant_string;
extern ConstantString version_constant_string;
extern ConstantString part_number_constant_string;
extern ConstantString device_id_constant_string;
extern ConstantString device_info_constant_string;
extern ConstantString api_constant_string;
extern ConstantString verbosity_constant_string;
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
extern ConstantString array_length_default_constant_string;
extern ConstantString array_length_spaces_constant_string;
extern ConstantString string_length_constant_string;
extern ConstantString array_open_constant_string;
extern ConstantString array_close_constant_string;
extern ConstantString array_separator_constant_string;
extern ConstantString version_property_separator_constant_string;
extern ConstantString subset_constant_string;
extern ConstantString all_constant_string;
extern ConstantString array_element_subset_constant_string;
extern ConstantString pins_constant_string;
extern ConstantString interrupt_number_constant_string;
extern ConstantString pin_number_constant_string;
extern ConstantString pin_mode_constant_string;
extern ConstantString processor_constant_string;
extern ConstantString processor_name_constant_string;

enum {ALL_ARRAY_SIZE=1};
extern ConstantString * all_c_style_array[ALL_ARRAY_SIZE];
extern Array<ConstantString *,ALL_ARRAY_SIZE> all_array;

}
}
#endif
