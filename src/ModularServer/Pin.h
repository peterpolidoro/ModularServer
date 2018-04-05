// ----------------------------------------------------------------------------
// Pin.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_PIN_H_
#define _MODULAR_SERVER_PIN_H_
#include <Streaming.h>
#include <Array.h>
#include <ConstantVariable.h>
#include <Functor.h>
#include <FunctorCallbacks.h>

#include "HardwareElement.h"
#include "Callback.h"
#include "Response.h"
#include "Constants.h"


namespace modular_server
{

namespace pin
{
extern ConstantString mode_input;
extern ConstantString mode_input_pullup;
extern ConstantString mode_output;
extern ConstantString mode_low;
extern ConstantString mode_change;
extern ConstantString mode_rising;
extern ConstantString mode_falling;
}

class Callback;

class Pin : private HardwareElement
{
public:
  Pin();

  void setModeInput();
  void setModeInputPullup();
  void setModeOutput();

  int digitalRead();
  void digitalWrite(uint8_t value);
  int analogRead();
  void analogWrite(int value);

  size_t getPinNumber();
  int getInterruptNumber();

private:
  int interrupt_number_;
  size_t pin_number_;
  Callback * callback_ptr_;
  const ConstantString * mode_ptr_;
  FunctorCallbacks::Callback isr_;

  Pin(const ConstantString & name, const size_t pin_number);
  void setup(const ConstantString & name);
  Callback * getCallbackPtr();
  const ConstantString & getMode();

  void writeApi(Response & response,
                bool write_name_only,
                bool write_details);
  void setPinNumber(const size_t pin_number);
  void enablePullup();
  void disablePullup();
  void setCallback(Callback & callback);
  void removeCallback();
  void setModeInterrupt(const ConstantString & mode);
  void reattach();
  void attach(Callback & callback, const ConstantString & mode);
  void detach();
  void resetIsr();

  // Handlers
  void isrHandler();

  friend class Server;
  friend class Callback;
};
}
#endif
