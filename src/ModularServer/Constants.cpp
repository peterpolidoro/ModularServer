// ----------------------------------------------------------------------------
// Constants.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
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
  .version_major=7,
  .version_minor=2,
  .version_patch=0,
};

const long response_pipe_read_max = 100000;

const double epsilon = 0.000000001;

// Pins
const size_t pin_pulse_timer_number = 3;
const uint32_t pin_pulse_delay = 0;
const uint32_t pin_pulse_count = 1;
CONSTANT_STRING(pin_mode_digital_input,"DIGITAL_INPUT");
CONSTANT_STRING(pin_mode_digital_input_pullup,"DIGITAL_INPUT_PULLUP");
CONSTANT_STRING(pin_mode_digital_output,"DIGITAL_OUTPUT");
CONSTANT_STRING(pin_mode_analog_input,"ANALOG_INPUT");
CONSTANT_STRING(pin_mode_analog_output,"ANALOG_OUTPUT");
CONSTANT_STRING(pin_mode_pulse_rising,"PULSE_RISING");
CONSTANT_STRING(pin_mode_pulse_falling,"PULSE_FALLING");
CONSTANT_STRING(pin_mode_interrupt_low,"INTERRUPT_LOW");
CONSTANT_STRING(pin_mode_interrupt_change,"INTERRUPT_CHANGE");
CONSTANT_STRING(pin_mode_interrupt_rising,"INTERRUPT_RISING");
CONSTANT_STRING(pin_mode_interrupt_falling,"INTERRUPT_FALLING");

// Properties
CONSTANT_STRING(serial_number_property_name,"serialNumber");
const long serial_number_min = 0;
const long serial_number_max = 65535;
const long serial_number_default = serial_number_min;

// Parameters
CONSTANT_STRING(verbosity_names,"NAMES");
CONSTANT_STRING(verbosity_general,"GENERAL");
CONSTANT_STRING(verbosity_detailed,"DETAILED");
SubsetMemberType verbosity_ptr_subset[VERBOSITY_SUBSET_LENGTH] =
{
  {.cs_ptr=&verbosity_names},
  {.cs_ptr=&verbosity_general},
  {.cs_ptr=&verbosity_detailed},
};

CONSTANT_STRING(pin_name_parameter_name,"pin_name");

SubsetMemberType pin_mode_ptr_subset[PIN_MODE_SUBSET_LENGTH] =
{
  {.cs_ptr=&pin_mode_digital_input},
  {.cs_ptr=&pin_mode_digital_input_pullup},
  {.cs_ptr=&pin_mode_digital_output},
  {.cs_ptr=&pin_mode_analog_input},
  {.cs_ptr=&pin_mode_analog_output},
  {.cs_ptr=&pin_mode_pulse_rising},
  {.cs_ptr=&pin_mode_pulse_falling},
};

CONSTANT_STRING(pin_value_parameter_name,"pin_value");
const long pin_value_min = 0;
const long pin_value_max = 255;

// Functions
CONSTANT_STRING(get_method_ids_function_name,"getMethodIds");
CONSTANT_STRING(help_function_name,"?");
CONSTANT_STRING(verbose_help_function_name,"??");
CONSTANT_STRING(get_device_id_function_name,"getDeviceId");
CONSTANT_STRING(get_device_info_function_name,"getDeviceInfo");
CONSTANT_STRING(get_api_function_name,"getApi");
CONSTANT_STRING(get_property_default_values_function_name,"getPropertyDefaultValues");
CONSTANT_STRING(set_properties_to_defaults_function_name,"setPropertiesToDefaults");
CONSTANT_STRING(get_property_values_function_name,"getPropertyValues");
CONSTANT_STRING(get_pin_info_function_name,"getPinInfo");
CONSTANT_STRING(set_pin_mode_function_name,"setPinMode");
CONSTANT_STRING(get_pin_value_function_name,"getPinValue");
CONSTANT_STRING(set_pin_value_function_name,"setPinValue");
CONSTANT_STRING(get_memory_free_function_name,"getMemoryFree");

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
CONSTANT_STRING(parameter_incorrect_type_error_data," parameter has incorrect type.");
CONSTANT_STRING(property_not_found_error_data,"Property not found");
CONSTANT_STRING(property_not_array_type_error_data,"Property not array type");
CONSTANT_STRING(property_element_index_out_of_bounds_error_data,"property_element_index out of bounds");
CONSTANT_STRING(cannot_set_element_in_string_property_with_subset_error_data,"Cannot set element in string property with subset.");
CONSTANT_STRING(incorrect_parameter_number_error_data,"Incorrect number of parameters. ")
CONSTANT_STRING(parameter_error_error_data,"Parameter value not valid. ");
CONSTANT_STRING(array_parameter_error_error_data,"Array parameter element value not valid. ");
CONSTANT_STRING(array_parameter_length_error_error_data,"Array parameter length not valid. ");
CONSTANT_STRING(value_not_in_subset_error_data,"Value not in subset: ");
CONSTANT_STRING(value_not_in_range_error_data,"Value not in range: ");
CONSTANT_STRING(property_function_not_found_error_data,"Property function not found");
CONSTANT_STRING(incorrect_property_parameter_number_error_data,"Incorrect number of property parameters. ")
CONSTANT_STRING(callback_function_not_found_error_data,"Callback function not found");
CONSTANT_STRING(incorrect_callback_parameter_number_error_data,"Incorrect number of callback parameters. ")

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
CONSTANT_STRING(serial_number_constant_string,"serial_number");
CONSTANT_STRING(ancestors_constant_string,"ancestors");
CONSTANT_STRING(firmware_constant_string,"firmware");
CONSTANT_STRING(hardware_constant_string,"hardware");
CONSTANT_STRING(name_constant_string,"name");
CONSTANT_STRING(type_constant_string,"type");
CONSTANT_STRING(method_type_constant_string,"method_type");
CONSTANT_STRING(units_constant_string,"units");
CONSTANT_STRING(result_constant_string,"result");
CONSTANT_STRING(result_info_constant_string,"result_info");
CONSTANT_STRING(array_element_type_constant_string,"array_element_type");
CONSTANT_STRING(property_constant_string,"property");
CONSTANT_STRING(properties_constant_string,"properties");
CONSTANT_STRING(parameters_constant_string,"parameters");
CONSTANT_STRING(function_constant_string,"function");
CONSTANT_STRING(functions_constant_string,"functions");
CONSTANT_STRING(callback_constant_string,"callback");
CONSTANT_STRING(callbacks_constant_string,"callbacks");
CONSTANT_STRING(min_constant_string,"min");
CONSTANT_STRING(max_constant_string,"max");
CONSTANT_STRING(array_element_min_constant_string,"array_element_min");
CONSTANT_STRING(array_element_max_constant_string,"array_element_max");
CONSTANT_STRING(array_length_min_constant_string,"array_length_min");
CONSTANT_STRING(array_length_max_constant_string,"array_length_max");
CONSTANT_STRING(string_length_max_constant_string,"string_length_max");
CONSTANT_STRING(version_constant_string,"version");
CONSTANT_STRING(part_number_constant_string,"part_number");
CONSTANT_STRING(device_id_constant_string,"device_id");
CONSTANT_STRING(device_info_constant_string,"device_info");
CONSTANT_STRING(api_constant_string,"api");
CONSTANT_STRING(verbosity_constant_string,"verbosity");
CONSTANT_STRING(value_constant_string,"value");
CONSTANT_STRING(default_value_constant_string,"default_value");
CONSTANT_STRING(question_constant_string,"?");
CONSTANT_STRING(question_double_constant_string,"??");
CONSTANT_STRING(zero_constant_string,"0");
CONSTANT_STRING(given_constant_string," given. ");
CONSTANT_STRING(needed_constant_string," needed.");
CONSTANT_STRING(less_than_equal_constant_string," <= ");
CONSTANT_STRING(element_constant_string," element");
CONSTANT_STRING(array_length_constant_string,"array_length");
CONSTANT_STRING(array_length_default_constant_string,"array_length_default");
CONSTANT_STRING(array_length_spaces_constant_string," array length");
CONSTANT_STRING(string_length_constant_string,"string_length");
CONSTANT_STRING(array_open_constant_string,"[");
CONSTANT_STRING(array_close_constant_string,"]");
CONSTANT_STRING(array_separator_constant_string,",");
CONSTANT_STRING(version_property_separator_constant_string,".");
CONSTANT_STRING(subset_constant_string,"subset");
CONSTANT_STRING(all_constant_string,"ALL");
CONSTANT_STRING(array_element_subset_constant_string,"array_element_subset");
CONSTANT_STRING(pins_constant_string,"pins");
CONSTANT_STRING(interrupt_number_constant_string,"interrupt_number");
CONSTANT_STRING(pin_number_constant_string,"pin_number");
CONSTANT_STRING(pin_mode_constant_string,"pin_mode");
CONSTANT_STRING(processor_constant_string,"processor");

#if defined(__AVR_ATmega1280__)
CONSTANT_STRING(processor_name_constant_string,"ATmega1280");
#elif defined(__AVR_ATmega2560__)
CONSTANT_STRING(processor_name_constant_string,"ATmega2560");
#elif defined(__MK20DX128__)
CONSTANT_STRING(processor_name_constant_string,"MK20DX128");
#elif defined(__MK20DX256__)
CONSTANT_STRING(processor_name_constant_string,"MK20DX256");
#elif defined(__MK64FX512__)
CONSTANT_STRING(processor_name_constant_string,"MK64FX512");
#elif defined(__MK66FX1M0__)
CONSTANT_STRING(processor_name_constant_string,"MK66FX1M0");
#elif defined(__IMXRT1062__)
CONSTANT_STRING(processor_name_constant_string,"IMXRT1062");
#else
CONSTANT_STRING(processor_name_constant_string,"");
#endif

ConstantString * all_c_style_array[ALL_ARRAY_SIZE] =
{
  &all_constant_string,
};
Array<ConstantString *,ALL_ARRAY_SIZE> all_array(all_c_style_array);

}
}
