// ----------------------------------------------------------------------------
// Pin.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_PIN_H_
#define _MODULAR_SERVER_PIN_H_
#include <Streaming.h>
#include <Array.h>
#include <ConstantVariable.h>
#include <Functor.h>
#include <FunctorCallbacks.h>
#include <EventController.h>

#include "HardwareElement.h"
#include "Callback.h"
#include "Response.h"
#include "Constants.h"


namespace modular_server
{
class Callback;

class Pin : private HardwareElement
{
public:
  Pin();

  void setModeDigitalInput();
  void setModeDigitalInputPullup();
  void setModeDigitalOutput();
  void setModeAnalogInput();
  void setModeAnalogOutput();
  void setModePulseRising();
  void setModePulseFalling();
  void setModePwmRising();
  void setModePwmFalling();

  int getValue();
  void setValue(int value);
  void addPwm(uint32_t period_ms,
    uint32_t on_duration_ms,
    int32_t count);

  size_t getPinNumber();
  int getInterruptNumber();

private:
  int interrupt_number_;
  size_t pin_number_;
  Callback * callback_ptr_;
  const ConstantString * mode_ptr_;
  FunctorCallbacks::Callback isr_;
  static EventController<modular_server::constants::PIN_PWM_EVENT_COUNT_MAX> pin_pwm_event_controller_;

  Pin(const ConstantString & name,
    size_t pin_number);
  void setup(const ConstantString & name);
  Callback * getCallbackPtr();
  const ConstantString & getMode();
  void setMode(const ConstantString & pin_mode);

  void writeApi(Response & response,
    bool write_name_only,
    bool write_details);
  void setPinNumber(size_t pin_number);
  void enablePullup();
  void disablePullup();
  void setCallback(Callback & callback);
  void removeCallback();
  void reattach();
  void attach(Callback & callback,
    const ConstantString & mode);
  void detach();
  void resetIsr();
  static void setupPinPulseEventController();

  // Handlers
  void isrHandler();
  void setPinHighHandler(int index);
  void setPinLowHandler(int index);

  friend class Server;
  friend class Callback;
};
}
#endif
