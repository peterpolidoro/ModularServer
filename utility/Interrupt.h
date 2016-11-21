// ----------------------------------------------------------------------------
// Interrupt.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_INTERRUPT_H_
#define _MODULAR_SERVER_INTERRUPT_H_
#include "Streaming.h"
#include "Array.h"
#include "ConstantVariable.h"

#include "NamedElement.h"
#include "Callback.h"
#include "Constants.h"


namespace modular_server
{
class Interrupt : private NamedElement
{
public:
  Interrupt();

  void setPin(size_t pin);
  void enablePullup();

private:
  size_t pin_;
  size_t number_;
  Callback * callback_ptr_;
  bool pullup_;
  Interrupt(const ConstantString & name, const size_t pin);
  void setup(const ConstantString & name);
  size_t getPin();
  size_t getNumber();
  Callback * getCallbackPtr();
  bool getPullup();
  friend class Server;
};
}
#endif
