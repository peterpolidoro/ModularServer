// ----------------------------------------------------------------------------
// Constants.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Constants.h"

namespace ModularDevice
{
namespace constants
{
// Fields
CONSTANT_STRING(serial_number_field_name,"serial_number");

const long serial_number_min = 0;
const long serial_number_max = 65535;
const long serial_number_default = serial_number_min;

// Parameters
CONSTANT_STRING(field_name_parameter_name,"field_name");
CONSTANT_STRING(field_value_parameter_name,"field_value");
CONSTANT_STRING(field_element_index_parameter_name,"field_element_index");

// Methods
CONSTANT_STRING(get_device_info_method_name,"getDeviceInfo");
CONSTANT_STRING(get_method_ids_method_name,"getMethodIds");
CONSTANT_STRING(get_parameters_method_name,"getParameters");
CONSTANT_STRING(help_method_name,"?");
CONSTANT_STRING(verbose_help_method_name,"??");
CONSTANT_STRING(get_memory_free_method_name,"getMemoryFree");
CONSTANT_STRING(get_field_default_values_method_name,"getFieldDefaultValues");
CONSTANT_STRING(set_fields_to_defaults_method_name,"setFieldsToDefaults");
CONSTANT_STRING(set_field_to_default_method_name,"setFieldToDefault");
CONSTANT_STRING(get_field_values_method_name,"getFieldValues");
CONSTANT_STRING(get_field_value_method_name,"getFieldValue");
CONSTANT_STRING(get_field_element_value_method_name,"getFieldElementValue");
CONSTANT_STRING(set_field_value_method_name,"setFieldValue");
CONSTANT_STRING(set_field_element_value_method_name,"setFieldElementValue");
CONSTANT_STRING(set_all_field_element_values_method_name,"setAllFieldElementValues");

// Errors
CONSTANT_STRING(parse_error_message,"Parse error");
CONSTANT_STRING(invalid_request_error_message,"Invalid Request");
CONSTANT_STRING(method_not_found_error_message,"Method not found");
CONSTANT_STRING(invalid_params_error_message,"Invalid params");
CONSTANT_STRING(server_error_error_message,"Server error");

CONSTANT_STRING(object_request_error_data,"JSON object requests not supported. Must use compact JSON array format for requests.");
CONSTANT_STRING(request_length_error_data,"Request length too long.");
CONSTANT_STRING(parameter_not_found_error_data,"Parameter not found");
CONSTANT_STRING(field_not_found_error_data,"Field not found");
CONSTANT_STRING(field_not_array_type_error_data,"Field not array type");
CONSTANT_STRING(field_element_index_out_of_bounds_error_data,"field_element_index out of bounds");
CONSTANT_STRING(incorrect_parameter_number_error_data,"Incorrect number of parameters. ")
CONSTANT_STRING(invalid_json_object_error_data," is not a valid JSON object.")
CONSTANT_STRING(invalid_json_array_error_data," is not a valid JSON array.")
CONSTANT_STRING(parameter_error_error_data,"Parameter value out of range: ");
CONSTANT_STRING(array_parameter_error_error_data,"Array parameter element value out of range: ");
CONSTANT_STRING(array_parameter_length_error_error_data,"Array parameter length out of range: ");

const int parse_error_code = -32700;
const int invalid_request_error_code = -32600;
const int method_not_found_error_code = -32601;
const int invalid_params_error_code = -32602;
const int internal_error_error_code = -32603;
const int server_error_error_code = -32000;

// Saved Variables
const size_t eeprom_initialized_default_value = 123;

// Constant Strings
CONSTANT_STRING(empty_constant_string,"");
CONSTANT_STRING(id_constant_string,"id");
CONSTANT_STRING(error_constant_string,"error");
CONSTANT_STRING(message_constant_string,"message");
CONSTANT_STRING(data_constant_string,"data");
CONSTANT_STRING(code_constant_string,"code");
CONSTANT_STRING(name_constant_string,"name");
CONSTANT_STRING(type_constant_string,"type");
CONSTANT_STRING(units_constant_string,"units");
CONSTANT_STRING(result_constant_string,"result");
CONSTANT_STRING(result_type_constant_string,"result_type");
CONSTANT_STRING(array_element_type_constant_string,"array_element_type");
CONSTANT_STRING(methods_constant_string,"methods");
CONSTANT_STRING(parameters_constant_string,"parameters");
CONSTANT_STRING(fields_constant_string,"fields");
CONSTANT_STRING(min_constant_string,"min");
CONSTANT_STRING(max_constant_string,"max");
CONSTANT_STRING(array_element_min_constant_string,"array_element_min");
CONSTANT_STRING(array_element_max_constant_string,"array_element_max");
CONSTANT_STRING(array_length_min_constant_string,"array_length_min");
CONSTANT_STRING(array_length_max_constant_string,"array_length_max");
CONSTANT_STRING(model_number_constant_string,"model_number");
CONSTANT_STRING(firmware_version_constant_string,"firmware_version");
CONSTANT_STRING(major_constant_string,"major");
CONSTANT_STRING(minor_constant_string,"minor");
CONSTANT_STRING(patch_constant_string,"patch");
CONSTANT_STRING(device_info_constant_string,"device_info");
CONSTANT_STRING(value_constant_string,"value");
CONSTANT_STRING(default_value_constant_string,"default_value");
CONSTANT_STRING(question_constant_string,"?");
CONSTANT_STRING(question_double_constant_string,"??");
CONSTANT_STRING(zero_constant_string,"0");
CONSTANT_STRING(given_constant_string," given. ");
CONSTANT_STRING(needed_constant_string," needed.");
CONSTANT_STRING(less_than_equal_constant_string," <= ");
CONSTANT_STRING(element_constant_string," element");
CONSTANT_STRING(array_length_constant_string," array length");
CONSTANT_STRING(board_constant_string,"board");

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
CONSTANT_STRING(board_type_constant_string,"mega");
#elif defined(__MK20DX128__) || defined(__MK20DX256__)
CONSTANT_STRING(board_type_constant_string,"teensy");
#else
CONSTANT_STRING(board_type_constant_string,"");
#endif
}
}
