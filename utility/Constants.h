// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_DEVICE_CONSTANTS_H_
#define _MODULAR_DEVICE_CONSTANTS_H_


namespace ModularDevice
{
namespace constants
{
enum{METHOD_COUNT_MAX=40}; //9 used internally
enum{PARAMETER_COUNT_MAX=17}; // 1 used internally
enum{SAVED_VARIABLE_COUNT_MAX=12}; // 2 used internally

enum{STRING_LENGTH_REQUEST=257};
enum{STRING_LENGTH_ERROR=257};
enum{STRING_LENGTH_DEVICE_NAME=32};
enum{STRING_LENGTH_PARAMETER_NAME=24};
enum{STRING_LENGTH_PARAMETER_UNITS=8};
enum{STRING_LENGTH_PARAMETER_COUNT=8};
enum{STRING_LENGTH_METHOD_NAME=32};

enum {JSON_PARSER_SIZE=32};
enum {METHOD_PARAMETER_COUNT_MAX=6};

enum ParameterType
  {
    LONG_PARAMETER,
    DOUBLE_PARAMETER,
    BOOL_PARAMETER,
    STRING_PARAMETER,
    OBJECT_PARAMETER,
    ARRAY_PARAMETER,
  };

union NumberType
{
  long l;
  double d;
};

extern const char eol;
extern const char start_char_json_object;
extern const char start_char_json_array;

extern const unsigned char method_count_max;

extern const unsigned char eeprom_initialized_value;

extern const unsigned int serial_number_min;
extern const unsigned int serial_number_max;
extern const unsigned int serial_number_default;
}
}
#endif
