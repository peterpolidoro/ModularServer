// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef BOARD_LED_CONTROLLER_CONSTANTS_H
#define BOARD_LED_CONTROLLER_CONSTANTS_H
#include <ConstantVariable.h>
#include <ModularServer.h>


namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{PROPERTY_COUNT_MAX=1};
enum{PARAMETER_COUNT_MAX=3};
enum{FUNCTION_COUNT_MAX=4};
enum{CALLBACK_COUNT_MAX=1};

enum{PIN_COUNT_MAX=1};

extern const long baud;

extern ConstantString device_name;

extern ConstantString firmware_name;
extern const modular_server::FirmwareInfo firmware_info;

extern ConstantString form_factor;
extern ConstantString hardware_name;
extern const modular_server::HardwareInfo hardware_info;

// Pins
extern ConstantString led_pin_name;
extern const size_t led_pin_number;

// Units
extern ConstantString seconds_unit;

// Properties
// Property values must be long, double, bool, long[], double[], bool[], char[], ConstantString *, (ConstantString *)[]

// Parameters
extern ConstantString duration_on_parameter_name;
extern const double duration_min;
extern const double duration_max;

extern ConstantString duration_off_parameter_name;

extern ConstantString count_parameter_name;
extern const long count_min;
extern const long count_max;

// Functions
extern ConstantString led_on_function_name;
extern ConstantString led_off_function_name;
extern ConstantString get_led_pin_number_function_name;
extern ConstantString blink_led_function_name;

// Callbacks

// Errors
}
#endif
