// ----------------------------------------------------------------------------
// Pin.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_PIN_H_
#define _MODULAR_SERVER_PIN_H_
#include <ConstantVariable.h>

#include "HardwareElement.h"
#include "Interrupt.h"
#include "Response.h"
#include "Constants.h"


namespace modular_server
{

namespace pin
{
extern ConstantString mode_interrupt;
extern ConstantString mode_digital_input;
extern ConstantString mode_digital_output;
}

class Pin : private HardwareElement
{
public:
  Pin();
  Interrupt * getInterruptPtr();
  const ConstantString & getMode();

  void writeApi(Response & response,
                bool write_name_only,
                bool write_details);

private:
  size_t pin_number_;
  Interrupt interrupt_;
  const ConstantString * mode_ptr_;
  Pin(const ConstantString & name, const size_t pin_number);
  void setup(const ConstantString & name);
  void setPinNumber(const size_t pin_number);
  size_t getPinNumber();
  void enablePullup();
  void disablePullup();
  void setMode(const ConstantString & mode);
  void reattach();
  void attach(Interrupt & interrupt, const ConstantString & mode);
  void detach();
  void resetIsr();

  // Handlers
  void isrHandler();

  friend class Server;
  friend class Interrupt;
};
}
#endif
