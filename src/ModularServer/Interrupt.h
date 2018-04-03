// ----------------------------------------------------------------------------
// Interrupt.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_INTERRUPT_H_
#define _MODULAR_SERVER_INTERRUPT_H_
#include <ConstantVariable.h>
#include <Functor.h>
#include <FunctorCallbacks.h>

#include "NamedElement.h"
#include "Callback.h"
#include "Response.h"
#include "Constants.h"


namespace modular_server
{

namespace interrupt
{
extern ConstantString mode_detached;
extern ConstantString mode_low;
extern ConstantString mode_change;
extern ConstantString mode_rising;
extern ConstantString mode_falling;
}

class Callback;

class Interrupt : private NamedElement
{
public:
  Interrupt();
  Callback * getCallbackPtr();
  const ConstantString & getMode();

  void writeApi(Response & response,
                bool write_name_only,
                bool write_details);

private:
  size_t interrupt_number_;
  Callback * callback_ptr_;
  const ConstantString * mode_ptr_;
  FunctorCallbacks::Callback isr_;
  Interrupt(const ConstantString & name, const size_t interrupt_number);
  void setup(const ConstantString & name);
  void setInterruptNumber(const size_t interrupt_number);
  size_t getInterruptNumber();
  void setCallback(Callback & callback);
  void removeCallback();
  void setMode(const ConstantString & mode);
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
