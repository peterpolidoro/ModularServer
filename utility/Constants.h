// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

namespace RemoteDevice
{
// RemoteDevice
const char EOL = '\n';
const char START_CHAR_JSON_OBJECT = '{';
const char START_CHAR_JSON_ARRAY = '[';
const int STRING_LENGTH_REQUEST = 286;

const int STRING_LENGTH_DEVICE_NAME = 32;

const int JSON_STATIC_BUFFER_SIZE = 254;
const int METHOD_COUNT_MAX = 24;

const int STRING_LENGTH_ERROR = 257;

const int DOUBLE_DIGITS = 4;
const int STRING_LENGTH_DOUBLE = 36;

const int RESPONSE_INDENT = 2;
const int RESPONSE_DEPTH_MAX = 8;

const unsigned char EEPROM_INITIALIZED_VALUE = 123;

const unsigned int SERIAL_NUMBER_MIN = 0;
const unsigned int SERIAL_NUMBER_MAX = 65535;
const unsigned int SERIAL_NUMBER_DEFAULT = 0;

enum ResponseCodes
  {
    ERROR=0,
    SUCCESS=1,
  };

enum ParameterType
  {
    LONG_PARAMETER,
    DOUBLE_PARAMETER,
    STRING_PARAMETER,
    OBJECT_PARAMETER,
    ARRAY_PARAMETER,
  };

union NumberType
{
  long l;
  double d;
};

// Method
const int STRING_LENGTH_METHOD_NAME = 32;
const int PARAMETER_COUNT_MAX = 6;

// Parameter
const int STRING_LENGTH_PARAMETER_NAME = 24;
const int STRING_LENGTH_PARAMETER_UNITS = 8;

}
#endif
