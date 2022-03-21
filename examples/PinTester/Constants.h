// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef PIN_TESTER_CONSTANTS_H
#define PIN_TESTER_CONSTANTS_H
#include <ConstantVariable.h>
#include <ModularServer.h>


namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{PROPERTY_COUNT_MAX=1};
enum{PARAMETER_COUNT_MAX=1};
enum{FUNCTION_COUNT_MAX=1};
enum{CALLBACK_COUNT_MAX=1};

extern const long baud;

extern ConstantString device_name;

extern ConstantString firmware_name;
extern const modular_server::FirmwareInfo firmware_info;

extern ConstantString form_factor;
extern ConstantString hardware_name;
extern const modular_server::HardwareInfo hardware_info;

extern const long pwm_period_ms;
extern const long pwm_on_duration_ms;
extern const long pwm_count;

// Pins
enum{PIN_COUNT_MAX=4};

extern ConstantString pin_a_pin_name;
extern const size_t pin_a_pin_number;

extern ConstantString pin_b_pin_name;
extern const size_t pin_b_pin_number;

extern ConstantString pin_c_pin_name;
extern const size_t pin_c_pin_number;

extern ConstantString pin_d_pin_name;
extern const size_t pin_d_pin_number;

// Units

// Properties
// Property values must be long, double, bool, long[], double[], bool[], char[], ConstantString *, (ConstantString *)[]

// Parameters

// Functions

// Callbacks
extern ConstantString test_pin_callback_name;

// Errors
}
#endif
