// ----------------------------------------------------------------------------
// Constants.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Constants.h"

namespace modular_server
{
namespace constants
{
CONSTANT_STRING(firmware_name,"ModularServer");
// Use semantic versioning http://semver.org/
const FirmwareInfo firmware_info =
  {
    .name_ptr=&firmware_name,
    .version_major=1,
    .version_minor=0,
    .version_patch=0,
  };

// Properties
CONSTANT_STRING(serial_number_property_name,"serial_number");

const long serial_number_min = 0;
const long serial_number_max = 65535;
const long serial_number_default = serial_number_min;

// Parameters
CONSTANT_STRING(property_name_parameter_name,"property_name");
CONSTANT_STRING(property_value_parameter_name,"property_value");
CONSTANT_STRING(property_element_index_parameter_name,"property_element_index");

// Methods
CONSTANT_STRING(get_procedure_ids_method_name,"getProcedureIds");
CONSTANT_STRING(help_method_name,"?");
CONSTANT_STRING(verbose_help_method_name,"??");
CONSTANT_STRING(get_device_id_method_name,"getDeviceId");
CONSTANT_STRING(get_device_info_method_name,"getDeviceInfo");
CONSTANT_STRING(get_api_method_name,"getApi");
CONSTANT_STRING(get_api_verbose_method_name,"getApiVerbose");
CONSTANT_STRING(get_memory_free_method_name,"getMemoryFree");
CONSTANT_STRING(get_property_default_values_method_name,"getPropertyDefaultValues");
CONSTANT_STRING(set_properties_to_defaults_method_name,"setPropertiesToDefaults");
CONSTANT_STRING(set_property_to_default_method_name,"setPropertyToDefault");
CONSTANT_STRING(get_property_values_method_name,"getPropertyValues");
CONSTANT_STRING(get_property_value_method_name,"getPropertyValue");
CONSTANT_STRING(get_property_element_value_method_name,"getPropertyElementValue");
CONSTANT_STRING(set_property_value_method_name,"setPropertyValue");
CONSTANT_STRING(set_property_element_value_method_name,"setPropertyElementValue");
CONSTANT_STRING(set_all_property_element_values_method_name,"setAllPropertyElementValues");

// Callbacks

// Errors
CONSTANT_STRING(parse_error_message,"Parse error");
CONSTANT_STRING(invalid_request_error_message,"Invalid Request");
CONSTANT_STRING(method_not_found_error_message,"Method not found");
CONSTANT_STRING(invalid_params_error_message,"Invalid params");
CONSTANT_STRING(server_error_error_message,"Server error");

CONSTANT_STRING(object_request_error_data,"JSON object requests not supported. Must use compact JSON array format for requests.");
CONSTANT_STRING(request_length_error_data,"Request length too long.");
CONSTANT_STRING(parameter_not_found_error_data,"Parameter not found");
CONSTANT_STRING(property_not_found_error_data,"Property not found");
CONSTANT_STRING(property_not_array_type_error_data,"Property not array type");
CONSTANT_STRING(property_element_index_out_of_bounds_error_data,"property_element_index out of bounds");
CONSTANT_STRING(cannot_set_element_in_string_property_with_subset_error_data,"Cannot set element in string property with subset.");
CONSTANT_STRING(incorrect_parameter_number_error_data,"Incorrect number of parameters. ")
CONSTANT_STRING(invalid_json_object_error_data," is not a valid JSON object.")
CONSTANT_STRING(invalid_json_array_error_data," is not a valid JSON array.")
CONSTANT_STRING(parameter_error_error_data,"Parameter value not valid. ");
CONSTANT_STRING(array_parameter_error_error_data,"Array parameter element value not valid. ");
CONSTANT_STRING(array_parameter_length_error_error_data,"Array parameter length not valid. ");
CONSTANT_STRING(value_not_in_subset_error_data,"Value not in subset: ");
CONSTANT_STRING(value_not_in_range_error_data,"Value not in range: ");

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
CONSTANT_STRING(form_factor_constant_string,"form_factor");
CONSTANT_STRING(firmware_constant_string,"firmware");
CONSTANT_STRING(hardware_constant_string,"hardware");
CONSTANT_STRING(name_constant_string,"name");
CONSTANT_STRING(type_constant_string,"type");
CONSTANT_STRING(units_constant_string,"units");
CONSTANT_STRING(result_constant_string,"result");
CONSTANT_STRING(result_type_constant_string,"result_type");
CONSTANT_STRING(array_element_type_constant_string,"array_element_type");
CONSTANT_STRING(properties_constant_string,"properties");
CONSTANT_STRING(parameters_constant_string,"parameters");
CONSTANT_STRING(methods_constant_string,"methods");
CONSTANT_STRING(callbacks_constant_string,"callbacks");
CONSTANT_STRING(min_constant_string,"min");
CONSTANT_STRING(max_constant_string,"max");
CONSTANT_STRING(array_element_min_constant_string,"array_element_min");
CONSTANT_STRING(array_element_max_constant_string,"array_element_max");
CONSTANT_STRING(array_length_min_constant_string,"array_length_min");
CONSTANT_STRING(array_length_max_constant_string,"array_length_max");
CONSTANT_STRING(version_constant_string,"version");
CONSTANT_STRING(part_number_constant_string,"part_number");
CONSTANT_STRING(device_id_constant_string,"device_id");
CONSTANT_STRING(device_info_constant_string,"device_info");
CONSTANT_STRING(api_constant_string,"API");
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
CONSTANT_STRING(array_open_constant_string,"[");
CONSTANT_STRING(array_close_constant_string,"]");
CONSTANT_STRING(array_separator_constant_string,",");
CONSTANT_STRING(version_property_separator_constant_string,".");
CONSTANT_STRING(subset_constant_string,"subset");
CONSTANT_STRING(all_constant_string,"All");
CONSTANT_STRING(array_element_subset_constant_string,"array_element_subset");
CONSTANT_STRING(processor_constant_string,"processor");

#if defined(__AVR_ATmega1280__)
CONSTANT_STRING(processor_name_constant_string,"ATmega1280");
#elif defined(__AVR_ATmega2560__)
CONSTANT_STRING(processor_name_constant_string,"ATmega2560");
#elif defined(__MK20DX128__)
CONSTANT_STRING(processor_name_constant_string,"MK20DX128");
#elif defined(__MK20DX256__)
CONSTANT_STRING(processor_name_constant_string,"MK20DX256");
#else
CONSTANT_STRING(processor_name_constant_string,"");
#endif
}
}
