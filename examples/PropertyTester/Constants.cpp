// ----------------------------------------------------------------------------
// Constants.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "Constants.h"


namespace constants
{
const long baud = 115200;

CONSTANT_STRING(device_name,"property_tester");

CONSTANT_STRING(firmware_name,"PropertyTester");
// Use semantic versioning http://semver.org/
const modular_server::FirmwareInfo firmware_info =
{
  .name_ptr=&firmware_name,
  .version_major=2,
  .version_minor=0,
  .version_patch=0,
};

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

CONSTANT_STRING(form_factor,"5x3");
CONSTANT_STRING(hardware_name,"Mega2560");
const modular_server::HardwareInfo hardware_info =
{
  .name_ptr=&hardware_name,
  .part_number=0,
  .version_major=0,
  .version_minor=0,
};

#elif defined(__MK20DX128__) || defined(__MK20DX256__)

CONSTANT_STRING(form_factor,"3x2");
CONSTANT_STRING(hardware_name,"Teensy");
const modular_server::HardwareInfo hardware_info =
{
  .name_ptr=&hardware_name,
  .part_number=0,
  .version_major=3,
  .version_minor=2,
};

#else

CONSTANT_STRING(form_factor,"");
CONSTANT_STRING(hardware_name,"");
const modular_server::HardwareInfo hardware_info =
{
  .name_ptr=&hardware_name,
  .part_number=0,
  .version_major=0,
  .version_minor=0,
};

#endif

// Pins

// Units

// Properties
CONSTANT_STRING(double_property_name,"double");
const double double_min = 1.0;
const double double_max = 5.0;
const double double_default = 3.14159;
const double double_min_new = 5.0;
const double double_max_new = 10.0;

CONSTANT_STRING(bool_property_name,"bool");
const bool bool_default = true;
const bool bool_default_new = false;

CONSTANT_STRING(long_array_property_name,"longArray");
const long long_array_default[LONG_ARRAY_LENGTH] = {5,4,3,2};
const long long_array_element_min = -3;
const long long_array_element_max = 10;

CONSTANT_STRING(double_array_property_name,"doubleArray");
const double double_array_default[DOUBLE_ARRAY_LENGTH] = {-1.1,2.2,3.3};
const double double_array_element_min = -33.333;
const double double_array_element_max = 100.0;

CONSTANT_STRING(bool_array_property_name,"boolArray");
const bool bool_array_default[BOOL_ARRAY_LENGTH] = {true,false,false,true};
const bool bool_array_default_new[BOOL_ARRAY_LENGTH_NEW] = {false,true};

CONSTANT_STRING(string_property_name,"string");
const char string_default[STRING_LENGTH_MAX] = "abcdef";

CONSTANT_STRING(odd_property_name,"odd");
modular_server::SubsetMemberType odd_subset[ODD_SUBSET_LENGTH] =
{
  {.l=1},
  {.l=3},
  {.l=5},
  {.l=7},
  {.l=9},
};
const long & odd_default = odd_subset[2].l;
modular_server::SubsetMemberType odd_subset_new[ODD_SUBSET_LENGTH_NEW] =
{
  {.l=11},
  {.l=13},
  {.l=15},
  {.l=17},
};

CONSTANT_STRING(mode_property_name,"mode");
CONSTANT_STRING(mode_rising,"RISING");
CONSTANT_STRING(mode_falling,"FALLING");
CONSTANT_STRING(mode_change,"CHANGE");
modular_server::SubsetMemberType mode_ptr_subset[MODE_SUBSET_LENGTH] =
{
  {.cs_ptr=&mode_rising},
  {.cs_ptr=&mode_falling},
  {.cs_ptr=&mode_change},
};
const ConstantString * const mode_ptr_default = &mode_rising;

CONSTANT_STRING(mode_array_property_name,"modeArray");
const ConstantString * const mode_array_default[MODE_ARRAY_LENGTH] =
{
  &mode_rising,
  &mode_falling,
};

CONSTANT_STRING(odd_array_property_name,"oddArray");
const long odd_array_default[ODD_ARRAY_LENGTH] =
{
  odd_subset[0].l,
  odd_subset[2].l,
};

// Parameters
CONSTANT_STRING(long_array_parameter_name,"long_array_parameter");

CONSTANT_STRING(length_parameter_name,"length_parameter");
const long length_min = 0;
const long length_max = 20;

CONSTANT_STRING(count_parameter_name,"count");
modular_server::SubsetMemberType count_subset[COUNT_SUBSET_LENGTH] =
{
  {.l=10},
  {.l=20},
  {.l=30},
  {.l=40},
  {.l=50},
  {.l=60},
  {.l=70},
};

CONSTANT_STRING(count_array_parameter_name,"count_array");
const size_t count_array_element_min = 30;
const size_t count_array_element_max = 60;
const size_t count_array_length_min = 1;
const size_t count_array_length_max = 3;

CONSTANT_STRING(direction_parameter_name,"direction");
CONSTANT_STRING(direction_up,"UP");
CONSTANT_STRING(direction_down,"DOWN");
CONSTANT_STRING(direction_left,"LEFT");
CONSTANT_STRING(direction_right,"RIGHT");
modular_server::SubsetMemberType direction_ptr_subset[DIRECTION_SUBSET_LENGTH] =
{
  {.cs_ptr=&direction_up},
  {.cs_ptr=&direction_down},
  {.cs_ptr=&direction_left},
  {.cs_ptr=&direction_right},
};

CONSTANT_STRING(direction_array_parameter_name,"direction_array");
const size_t direction_array_length_min = 2;
const size_t direction_array_length_max = 4;

CONSTANT_STRING(subset_index_parameter_name,"subset_index");

// Functions
CONSTANT_STRING(get_doubled_function_name,"getDoubled");
CONSTANT_STRING(get_bool_function_name,"getBool");
CONSTANT_STRING(get_long_array_fixed_function_name,"getLongArrayFixed");
CONSTANT_STRING(get_long_array_variable_function_name,"getLongArrayVariable");
CONSTANT_STRING(set_long_array_fixed_function_name,"setLongArrayFixed");
CONSTANT_STRING(set_long_array_variable_function_name,"setLongArrayVariable");
CONSTANT_STRING(set_long_array_parameter_function_name,"setLongArrayParameter");
CONSTANT_STRING(get_string_all_function_name,"getStringAll");
CONSTANT_STRING(get_string_some_function_name,"getStringSome");
CONSTANT_STRING(get_count_function_name,"getCount");
CONSTANT_STRING(get_count_array_function_name,"getCountArray");
CONSTANT_STRING(get_direction_function_name,"getDirection");
CONSTANT_STRING(get_direction_array_function_name,"getDirectionArray");
CONSTANT_STRING(check_mode_function_name,"checkMode");
CONSTANT_STRING(increment_mode_function_name,"incrementMode");
CONSTANT_STRING(set_new_double_range_function_name,"setNewDoubleRange");
CONSTANT_STRING(set_new_odd_subset_function_name,"setNewOddSubset");
CONSTANT_STRING(set_new_odd_default_function_name,"setNewOddDefault");

// Callbacks

// Errors
CONSTANT_STRING(index_error,"Invalid index.");

}
