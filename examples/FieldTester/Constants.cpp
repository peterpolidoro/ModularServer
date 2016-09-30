// ----------------------------------------------------------------------------
// Constants.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Constants.h"


namespace constants
{
const size_t baudrate = 9600;

CONSTANT_STRING(device_name,"field_tester");

CONSTANT_STRING(firmware_name,"FieldTester");
// Use semantic versioning http://semver.org/
const ModularDevice::FirmwareInfo firmware_info =
  {
    .name_ptr=&firmware_name,
    .version_major=1,
    .version_minor=0,
    .version_patch=0,
  };

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

CONSTANT_STRING(form_factor,"mega");
CONSTANT_STRING(hardware_name,"mega");
const ModularDevice::HardwareInfo hardware_info =
  {
    .name_ptr=&hardware_name,
    .model_number=0,
    .version_major=0,
    .version_minor=0,
  };

#elif defined(__MK20DX128__) || defined(__MK20DX256__)

CONSTANT_STRING(form_factor,"teensy");
CONSTANT_STRING(hardware_name,"teensy");
const ModularDevice::HardwareInfo hardware_info =
  {
    .name_ptr=&hardware_name,
    .model_number=0,
    .version_major=0,
    .version_minor=0,
  };

#else

CONSTANT_STRING(form_factor,"");
CONSTANT_STRING(hardware_name,"");
const ModularDevice::HardwareInfo hardware_info =
  {
    .name_ptr=&hardware_name,
    .model_number=0,
    .version_major=0,
    .version_minor=0,
  };

#endif

// Fields
CONSTANT_STRING(double_field_name,"double");
const double double_default = 3.14159;

CONSTANT_STRING(bool_field_name,"bool");
const bool bool_default = false;

CONSTANT_STRING(long_array_field_name,"long_array");
const long long_array_default[LONG_ARRAY_LENGTH] = {5,4,3,2};
const long long_array_element_min = -3;
const long long_array_element_max = 10;

CONSTANT_STRING(double_array_field_name,"double_array");
const double double_array_default[DOUBLE_ARRAY_LENGTH] = {-1.1,2.2,3.3};
const double double_array_element_min = -33.333;
const double double_array_element_max = 100.0;

CONSTANT_STRING(bool_array_field_name,"bool_array");
const bool bool_array_default[BOOL_ARRAY_LENGTH] = {false,true};

CONSTANT_STRING(string_field_name,"string");
const char string_default[STRING_LENGTH_MAX] = "abcdef";

CONSTANT_STRING(odd_field_name,"odd");
const ModularDevice::SubsetMemberType odd_subset[ODD_SUBSET_LENGTH] =
  {
    {.l=1},
    {.l=3},
    {.l=5},
    {.l=7},
    {.l=9},
  };
const long & odd_default = odd_subset[2].l;

CONSTANT_STRING(mode_field_name,"mode");
CONSTANT_STRING(mode_rising,"RISING");
CONSTANT_STRING(mode_falling,"FALLING");
CONSTANT_STRING(mode_change,"CHANGE");
const ModularDevice::SubsetMemberType mode_ptr_subset[MODE_SUBSET_LENGTH] =
  {
    {.cs_ptr=&mode_rising},
    {.cs_ptr=&mode_falling},
    {.cs_ptr=&mode_change},
  };
const ConstantString * const mode_ptr_default = &mode_rising;

CONSTANT_STRING(odd_array_field_name,"odd_array");
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
const ModularDevice::SubsetMemberType count_subset[COUNT_SUBSET_LENGTH] =
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
const ModularDevice::SubsetMemberType direction_ptr_subset[DIRECTION_SUBSET_LENGTH] =
  {
    {.cs_ptr=&direction_up},
    {.cs_ptr=&direction_down},
    {.cs_ptr=&direction_left},
    {.cs_ptr=&direction_right},
  };

CONSTANT_STRING(direction_array_parameter_name,"direction_array");
const size_t direction_array_length_min = 2;
const size_t direction_array_length_max = 4;

// Methods
CONSTANT_STRING(get_doubled_method_name,"getDoubled");
CONSTANT_STRING(get_bool_method_name,"getBool");
CONSTANT_STRING(get_long_array_fixed_method_name,"getLongArrayFixed");
CONSTANT_STRING(get_long_array_variable_method_name,"getLongArrayVariable");
CONSTANT_STRING(set_long_array_fixed_method_name,"setLongArrayFixed");
CONSTANT_STRING(set_long_array_variable_method_name,"setLongArrayVariable");
CONSTANT_STRING(set_long_array_parameter_method_name,"setLongArrayParameter");
CONSTANT_STRING(get_string_all_method_name,"getStringAll");
CONSTANT_STRING(get_string_some_method_name,"getStringSome");
CONSTANT_STRING(get_count_method_name,"getCount");
CONSTANT_STRING(get_count_array_method_name,"getCountArray");
CONSTANT_STRING(get_direction_method_name,"getDirection");
CONSTANT_STRING(get_direction_array_method_name,"getDirectionArray");
CONSTANT_STRING(check_mode_method_name,"checkMode");
CONSTANT_STRING(increment_mode_method_name,"incrementMode");

// Errors
CONSTANT_STRING(index_error,"Invalid index.");

}
