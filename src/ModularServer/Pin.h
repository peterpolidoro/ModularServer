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

  int read();
  void write(const int value);

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
  void setMode(const ConstantString & pin_mode);

  void writeApi(Response & response,
                bool write_name_only,
                bool write_details);
  void setPinNumber(const size_t pin_number);
  void enablePullup();
  void disablePullup();
  void setCallback(Callback & callback);
  void removeCallback();
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
