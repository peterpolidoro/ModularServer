// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef MINIMAL_DEVICE_CONSTANTS_H
#define MINIMAL_DEVICE_CONSTANTS_H
#include "ConstantVariable.h"
#include "ModularServer.h"


namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{PROPERTY_COUNT_MAX=1};
enum{PARAMETER_COUNT_MAX=1};
enum{FUNCTION_COUNT_MAX=1};
enum{CALLBACK_COUNT_MAX=2};

enum{INTERRUPT_COUNT_MAX=2};

extern const long baudrate;

extern ConstantString device_name;

extern ConstantString firmware_name;
extern const modular_server::FirmwareInfo firmware_info;

extern ConstantString form_factor;
extern ConstantString hardware_name;
extern const modular_server::HardwareInfo hardware_info;

extern size_t led_pin;

// Interrupts
extern ConstantString bnc_a_interrupt_name;
extern const size_t bnc_a_pin;

extern ConstantString bnc_b_interrupt_name;
extern const size_t bnc_b_pin;

// Units

// Properties
// Property values must be long, double, bool, long[], double[], bool[], char[], ConstantString *

// Parameters

// Functions

// Callbacks
extern ConstantString set_led_on_callback_name;
extern ConstantString set_led_off_callback_name;

// Errors
}
#endif
