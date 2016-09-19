// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "ConstantVariable.h"

namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{FIELD_COUNT_MAX=1};
enum{PARAMETER_COUNT_MAX=3};
enum{METHOD_COUNT_MAX=4};

extern ConstantString device_name;
extern ConstantString firmware_name;
extern const long firmware_major;
extern const long firmware_minor;
extern const long firmware_patch;

extern const size_t baudrate;

extern const size_t model_number;
extern ConstantString hardware_name;

extern const size_t led_pin;

extern HardwareSerial & serial2;
extern const size_t serial2_rx_pin;

// Fields
// Field values must be long, double, bool, long[], double[], bool[], char[], ConstantString *

// Parameters
extern ConstantString duration_on_parameter_name;
extern const double duration_min;
extern const double duration_max;

extern ConstantString duration_off_parameter_name;

extern ConstantString count_parameter_name;
extern const long count_min;
extern const long count_max;

extern ConstantString seconds_unit;

// Methods
extern ConstantString led_on_method_name;
extern ConstantString led_off_method_name;
extern ConstantString get_led_pin_method_name;
extern ConstantString blink_led_method_name;

// Errors
}
#endif
