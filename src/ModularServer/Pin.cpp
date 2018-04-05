// ----------------------------------------------------------------------------
// Pin.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Pin.h"


namespace modular_server
{

namespace pin
{
CONSTANT_STRING(mode_input,"INPUT");
CONSTANT_STRING(mode_output,"OUTPUT");
CONSTANT_STRING(mode_low,"LOW");
CONSTANT_STRING(mode_change,"CHANGE");
CONSTANT_STRING(mode_rising,"RISING");
CONSTANT_STRING(mode_falling,"FALLING");
}

// public
Pin::Pin()
{
  setup(constants::empty_constant_string);
}

// protected

// private
Pin::Pin(const ConstantString & name, const size_t pin_number)
{
  setup(name);
  setPinNumber(pin_number);
}

void Pin::setup(const ConstantString & name)
{
  setName(name);
  interrupt_number_ = NOT_AN_INTERRUPT;
  pin_number_ = 0;
  callback_ptr_ = NULL;
  mode_ptr_ = &pin::mode_input;
  isr_ = NULL;
}

Callback * Pin::getCallbackPtr()
{
  return callback_ptr_;
}

const ConstantString & Pin::getMode()
{
  return *mode_ptr_;
}

void Pin::writeApi(Response & response,
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

  const ConstantString & hardware_name = getHardwareName();
  response.write(constants::hardware_constant_string,hardware_name);

  if (write_details)
  {
    response.write(constants::pin_number_constant_string,getPinNumber());

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

void Pin::setPinNumber(const size_t pin_number)
{
  interrupt_number_ = digitalPinToInterrupt(pin_number);
  pin_number_ = pin_number;
}

size_t Pin::getPinNumber()
{
  return pin_number_;
}

int Pin::getInterruptNumber()
{
  return interrupt_number_;
}

void Pin::enablePullup()
{
  pinMode(pin_number_,INPUT_PULLUP);
}

void Pin::disablePullup()
{
  pinMode(pin_number_,INPUT);
}

void Pin::setCallback(Callback & callback)
{
  callback_ptr_ = &callback;
  reattach();
}

void Pin::removeCallback()
{
  detach();
  callback_ptr_ = NULL;
}

void Pin::setMode(const ConstantString & mode)
{
  if (&mode == &pin::mode_low)
  {
    mode_ptr_ = &pin::mode_low;
    reattach();
  }
  else if (&mode == &pin::mode_change)
  {
    mode_ptr_ = &pin::mode_change;
    reattach();
  }
  else if (&mode == &pin::mode_rising)
  {
    mode_ptr_ = &pin::mode_rising;
    reattach();
  }
  else if (&mode == &pin::mode_falling)
  {
    mode_ptr_ = &pin::mode_falling;
    reattach();
  }
  else
  {
    detach();
  }
}

void Pin::reattach()
{
  if (!callback_ptr_)
  {
    return;
  }
  if (!callback_ptr_->getFunctor())
  {
    return;
  }
  if (interrupt_number_ == NOT_AN_INTERRUPT)
  {
    return;
  }
  if (mode_ptr_ == &pin::mode_low)
  {
    resetIsr();
    detachInterrupt(interrupt_number_);
    enablePullup();
    attachInterrupt(interrupt_number_,
                    isr_,
                    LOW);
  }
  else if (mode_ptr_ == &pin::mode_change)
  {
    resetIsr();
    detachInterrupt(interrupt_number_);
    enablePullup();
    attachInterrupt(interrupt_number_,
                    isr_,
                    CHANGE);
  }
  else if (mode_ptr_ == &pin::mode_rising)
  {
    resetIsr();
    detachInterrupt(interrupt_number_);
    enablePullup();
    attachInterrupt(interrupt_number_,
                    isr_,
                    RISING);
  }
  else if (mode_ptr_ == &pin::mode_falling)
  {
    resetIsr();
    detachInterrupt(interrupt_number_);
    enablePullup();
    attachInterrupt(interrupt_number_,
                    isr_,
                    FALLING);
  }
}

void Pin::attach(Callback & callback, const ConstantString & mode)
{
  setCallback(callback);
  setMode(mode);
}

void Pin::detach()
{
  if (interrupt_number_ == NOT_AN_INTERRUPT)
  {
    return;
  }
  detachInterrupt(interrupt_number_);
  disablePullup();
  mode_ptr_ = &pin::mode_input;
}

void Pin::resetIsr()
{
  FunctorCallbacks::remove(isr_);
  isr_ = FunctorCallbacks::add(makeFunctor((Functor0 *)0,*this,&Pin::isrHandler));
}

void Pin::isrHandler()
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
