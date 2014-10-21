// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef CONSTANTS_H
#define CONSTANTS_H

// RemoteDevice
const char EOL = '\n';
const char START_CHAR_JSON_OBJECT = '{';
const char START_CHAR_JSON_ARRAY = '[';
const int STRING_LENGTH_REQUEST = 257;

const int STRING_LENGTH_DEVICE_NAME = 64;

const int JSON_PARSER_SIZE = 32;
const int JSON_OBJECT_SIZE_RESPONSE = 32;
const int JSON_OBJECT_SIZE_DEVICE_INFO = 4;
const int METHOD_COUNT_MAX = 32;

const int STRING_LENGTH_PARAMETERS = 257;
const int STRING_LENGTH_ERROR = 257;

enum ResponseCodes
  {
    ERROR=0,
    SUCCESS=1,
  };

enum RequestType
  {
    OBJECT_REQUEST,
    ARRAY_REQUEST,
    COMMAND_LINE_REQUEST,
  };

enum ParameterType
  {
    LONG_PARAMETER,
    DOUBLE_PARAMETER,
    STRING_PARAMETER,
  };

union NumberType
{
  long l;
  double d;
};

// Method
const int STRING_LENGTH_METHOD_NAME = 32;

// const int JSON_ARRAY_SIZE_METHOD_HELP = 16;
// const int JSON_OBJECT_SIZE_METHOD_HELP_FULL = 32;
const int PARAMETER_COUNT_MAX = 8;

// Parameter
const int STRING_LENGTH_PARAMETER_NAME = 16;
const int STRING_LENGTH_PARAMETER_UNITS = 8;
const int STRING_LENGTH_PARAMETER_HELP = 64;

const int JSON_OBJECT_SIZE_PARAMETER_HELP = 8;

#endif
