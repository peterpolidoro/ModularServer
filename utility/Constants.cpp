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

// Methods
CONSTANT_STRING(get_device_info_method_name,"getDeviceInfo");
CONSTANT_STRING(get_method_ids_method_name,"getMethodIds");
CONSTANT_STRING(get_parameters_method_name,"getParameters");
CONSTANT_STRING(help_method_name,"?");
CONSTANT_STRING(verbose_help_method_name,"??");
CONSTANT_STRING(get_memory_free_method_name,"getMemoryFree");
CONSTANT_STRING(get_field_values_method_name,"getFieldValues");
CONSTANT_STRING(set_fields_to_defaults_method_name,"setFieldsToDefaults");
CONSTANT_STRING(set_serial_number_method_name,"setSerialNumber");

// Errors
CONSTANT_STRING(parse_error_message,"Parse error");
CONSTANT_STRING(invalid_request_error_message,"Invalid Request");
CONSTANT_STRING(method_not_found_error_message,"Method not found");
CONSTANT_STRING(invalid_params_error_message,"Invalid params");
CONSTANT_STRING(server_error_error_message,"Server error");
CONSTANT_STRING(object_request_error_data,"JSON object requests not supported. Must use compact JSON array format for requests.");
CONSTANT_STRING(request_length_error_data,"Request length too long.");
CONSTANT_STRING(parameter_not_found_error_data,"Parameter not found");
CONSTANT_STRING(incorrect_parameter_number_error_data,"Incorrect number of parameters. ")
CONSTANT_STRING(invalid_json_object_error_data," is not a valid JSON object.")
CONSTANT_STRING(invalid_json_array_error_data," is not a valid JSON array.")
CONSTANT_STRING(parameter_error_error_data,"Parameter value out of range: ");
CONSTANT_STRING(array_parameter_error_error_data,"Array parameter element value out of range: ");

const int parse_error_code = -32700;
const int invalid_request_error_code = -32600;
const int method_not_found_error_code = -32601;
const int invalid_params_error_code = -32602;
const int internal_error_error_code = -32603;
const int server_error_error_code = -32000;

// Saved Variables
const unsigned char eeprom_initialized_default_value = 123;

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
CONSTANT_STRING(model_number_constant_string,"model_number");
CONSTANT_STRING(firmware_version_constant_string,"firmware_version");
CONSTANT_STRING(major_constant_string,"major");
CONSTANT_STRING(minor_constant_string,"minor");
CONSTANT_STRING(patch_constant_string,"patch");
CONSTANT_STRING(device_info_constant_string,"device_info");
CONSTANT_STRING(value_constant_string,"value");
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
