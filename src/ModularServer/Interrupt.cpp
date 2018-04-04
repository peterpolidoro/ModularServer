// ----------------------------------------------------------------------------
// Interrupt.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Interrupt.h"


namespace modular_server
{

namespace interrupt
{
CONSTANT_STRING(mode_detached,"DETACHED");
CONSTANT_STRING(mode_low,"LOW");
CONSTANT_STRING(mode_change,"CHANGE");
CONSTANT_STRING(mode_rising,"RISING");
CONSTANT_STRING(mode_falling,"FALLING");
}

// public
Interrupt::Interrupt()
{
  setup(constants::empty_constant_string);
}

Callback * Interrupt::getCallbackPtr()
{
  return callback_ptr_;
}

const ConstantString & Interrupt::getMode()
{
  return *mode_ptr_;
}

void Interrupt::writeApi(Response & response,
                         bool write_name_only,
                         bool write_details)
{
  if (response.error())
  {
    return;
  }

  const ConstantString & name = getName();
  if (write_name_only)
  {
    response.write(name);
    return;
  }

  response.beginObject();

  response.write(constants::name_constant_string,name);

  if (write_details)
  {
    response.write(constants::interrupt_number_constant_string,getInterruptNumber());
  }

  Callback * callback_ptr = getCallbackPtr();
  if (callback_ptr != NULL)
  {
    response.write(constants::callback_constant_string,callback_ptr->getName());
  }

  response.write(constants::mode_constant_string,getMode());

  response.endObject();
}

// protected

// private
Interrupt::Interrupt(const ConstantString & name, const int interrupt_number)
{
  setup(name);
  setInterruptNumber(interrupt_number);
}

void Interrupt::setup(const ConstantString & name)
{
  setName(name);
  interrupt_number_ =  NOT_AN_INTERRUPT;
  callback_ptr_ = NULL;
  mode_ptr_ = &interrupt::mode_detached;
  isr_ = NULL;
}

void Interrupt::setInterruptNumber(const int interrupt_number)
{
  interrupt_number_ = interrupt_number;
}

int Interrupt::getInterruptNumber()
{
  return interrupt_number_;
}

void Interrupt::setCallback(Callback & callback)
{
  callback_ptr_ = &callback;
  reattach();
}

void Interrupt::removeCallback()
{
  detach();
  callback_ptr_ = NULL;
}

void Interrupt::setMode(const ConstantString & mode)
{
  if (&mode == &interrupt::mode_low)
  {
    mode_ptr_ = &interrupt::mode_low;
    reattach();
  }
  else if (&mode == &interrupt::mode_change)
  {
    mode_ptr_ = &interrupt::mode_change;
    reattach();
  }
  else if (&mode == &interrupt::mode_rising)
  {
    mode_ptr_ = &interrupt::mode_rising;
    reattach();
  }
  else if (&mode == &interrupt::mode_falling)
  {
    mode_ptr_ = &interrupt::mode_falling;
    reattach();
  }
  else
  {
    detach();
  }
}

void Interrupt::reattach()
{
  if (!callback_ptr_)
  {
    return;
  }
  if (!callback_ptr_->getFunctor())
  {
    return;
  }
  if (mode_ptr_ == &interrupt::mode_low)
  {
    resetIsr();
    detachInterrupt(interrupt_number_);
    enablePullup();
    attachInterrupt(interrupt_number_,
                    isr_,
                    LOW);
  }
  else if (mode_ptr_ == &interrupt::mode_change)
  {
    resetIsr();
    detachInterrupt(interrupt_number_);
    enablePullup();
    attachInterrupt(interrupt_number_,
                    isr_,
                    CHANGE);
  }
  else if (mode_ptr_ == &interrupt::mode_rising)
  {
    resetIsr();
    detachInterrupt(interrupt_number_);
    enablePullup();
    attachInterrupt(interrupt_number_,
                    isr_,
                    RISING);
  }
  else if (mode_ptr_ == &interrupt::mode_falling)
  {
    resetIsr();
    detachInterrupt(interrupt_number_);
    enablePullup();
    attachInterrupt(interrupt_number_,
                    isr_,
                    FALLING);
  }
}

void Interrupt::attach(Callback & callback, const ConstantString & mode)
{
  setCallback(callback);
  setMode(mode);
}

void Interrupt::detach()
{
  detachInterrupt(interrupt_number_);
  disablePullup();
  mode_ptr_ = &interrupt::mode_detached;
}

void Interrupt::resetIsr()
{
  FunctorCallbacks::remove(isr_);
  isr_ = FunctorCallbacks::add(makeFunctor((Functor0 *)0,*this,&Interrupt::isrHandler));
}

void Interrupt::isrHandler()
{
  if (!callback_ptr_)
  {
    return;
  }
  if (!callback_ptr_->getFunctor())
  {
    return;
  }
  callback_ptr_->functor(this);
}

}
