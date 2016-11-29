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
enum{PROPERTY_COUNT_MAX=3};
enum{PARAMETER_COUNT_MAX=1};
enum{FUNCTION_COUNT_MAX=1};
enum{CALLBACK_COUNT_MAX=3};

enum{INTERRUPT_COUNT_MAX=6};

extern const long baudrate;

extern ConstantString device_name;

extern ConstantString firmware_name;
extern const modular_server::FirmwareInfo firmware_info;

extern ConstantString form_factor;
extern ConstantString hardware_name;
extern const modular_server::HardwareInfo hardware_info;

extern const size_t led_pin;

// Interrupts
extern ConstantString bnc_a_interrupt_name;
extern const size_t bnc_a_pin;

extern ConstantString bnc_b_interrupt_name;
extern const size_t bnc_b_pin;

extern ConstantString bnc_c_interrupt_name;
extern const size_t bnc_c_pin;

extern ConstantString bnc_d_interrupt_name;
extern const size_t bnc_d_pin;

extern ConstantString bnc_e_interrupt_name;
extern const size_t bnc_e_pin;

extern ConstantString bnc_f_interrupt_name;
extern const size_t bnc_f_pin;

// Units
extern ConstantString seconds_unit;

// Properties
// Property values must be long, double, bool, long[], double[], bool[], char[], ConstantString *
extern ConstantString duration_on_property_name;
extern const double duration_min;
extern const double duration_max;
extern const double duration_on_default;

extern ConstantString duration_off_property_name;
extern const double duration_off_default;

extern ConstantString count_property_name;
extern const long count_min;
extern const long count_max;
extern const long count_default;

// Parameters

// Functions

// Callbacks
extern ConstantString set_led_on_callback_name;
extern ConstantString set_led_off_callback_name;
extern ConstantString blink_led_callback_name;

// Errors
}
#endif
