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

namespace callback
{
enum{MODE_SUBSET_LENGTH=5};
extern ConstantString mode_detached;
extern ConstantString mode_low;
extern ConstantString mode_change;
extern ConstantString mode_rising;
extern ConstantString mode_falling;
extern modular_server::SubsetMemberType mode_ptr_subset[MODE_SUBSET_LENGTH];
}

class Interrupt : private NamedElement
{
public:
  Interrupt();

private:
  size_t number_;
  size_t pin_;
  Callback * callback_ptr_;
  const ConstantString * mode_ptr_;
  bool pullup_;
  Interrupt(const ConstantString & name, const size_t pin);
  void setup(const ConstantString & name);
  size_t getNumber();
  size_t getPin();
  void setPin(size_t pin);
  void attach(Callback * callback_ptr, const ConstantString & mode, bool pullup);
  void detach();
  Callback * getCallbackPtr();
  const ConstantString & getMode();
  bool getPullup();
  void enablePullup();
  void disablePullup();
  friend class Server;
};
}
#endif
