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
CONSTANT_STRING(device_name,"board_led_controller");

CONSTANT_STRING(firmware_name,"BoardLedController");
// Use semantic versioning http://semver.org/
const ModularDevice::FirmwareInfo firmware_info =
  {
    .name_ptr=&firmware_name,
    .version_major=1,
    .version_minor=0,
    .version_patch=0,
  };

const size_t baudrate = 9600;

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

const size_t led_pin = 13;

// Fields

// Parameters
CONSTANT_STRING(duration_on_parameter_name,"duration_on");
const double duration_min = 0.1;
const double duration_max = 2.5;

CONSTANT_STRING(duration_off_parameter_name,"duration_off");

CONSTANT_STRING(count_parameter_name,"count");
const long count_min = 1;
const long count_max = 100;

CONSTANT_STRING(seconds_unit,"seconds");

// Methods
CONSTANT_STRING(led_on_method_name,"setLedOn");
CONSTANT_STRING(led_off_method_name,"setLedOff");
CONSTANT_STRING(get_led_pin_method_name,"getLedPin");
CONSTANT_STRING(blink_led_method_name,"blinkLed");

// Errors
}
