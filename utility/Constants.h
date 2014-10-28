// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace RemoteDevice
{
// RemoteDevice
const char EOL = '\n';
const char START_CHAR_JSON_OBJECT = '{';
const char START_CHAR_JSON_ARRAY = '[';
const int STRING_LENGTH_REQUEST = 257;

const int STRING_LENGTH_DEVICE_NAME = 32;

const int JSON_PARSER_SIZE = 32;
const int METHOD_COUNT_MAX = 16;

const int STRING_LENGTH_ERROR = 257;

const int DOUBLE_DIGITS = 4;
const int STRING_LENGTH_DOUBLE = 36;

const int RESPONSE_INDENT = 2;
const int RESPONSE_DEPTH_MAX = 8;

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
const int PARAMETER_COUNT_MAX = 5;

// Parameter
const int STRING_LENGTH_PARAMETER_NAME = 24;
const int STRING_LENGTH_PARAMETER_UNITS = 8;

}
#endif
