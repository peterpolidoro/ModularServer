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
constants::SubsetMemberType mode_ptr_subset[MODE_SUBSET_LENGTH] =
  {
    {.cs_ptr=&mode_detached},
    {.cs_ptr=&mode_low},
    {.cs_ptr=&mode_change},
    {.cs_ptr=&mode_rising},
    {.cs_ptr=&mode_falling},
  };
}

// public
Interrupt::Interrupt()
{
  setup(constants::empty_constant_string);
}

size_t Interrupt::getNumber()
{
  return number_;
}

size_t Interrupt::getPin()
{
  return pin_;
}

Callback * Interrupt::getCallbackPtr()
{
  return callback_ptr_;
}

const ConstantString & Interrupt::getMode()
{
  return *mode_ptr_;
}

// protected

// private
Interrupt::Interrupt(const ConstantString & name, const size_t pin)
{
  setup(name);
  setPin(pin);
}

void Interrupt::setup(const ConstantString & name)
{
  setName(name);
  number_ = 0;
  pin_ = 0;
  callback_ptr_ = NULL;
  mode_ptr_ = &interrupt::mode_detached;
}

void Interrupt::setPin(const size_t pin)
{
  number_ = digitalPinToInterrupt(pin);
  pin_ = pin;
}

void Interrupt::enablePullup()
{
  pinMode(pin_,INPUT_PULLUP);
}

void Interrupt::disablePullup()
{
  pinMode(pin_,INPUT);
}

void Interrupt::setCallback(const Callback & callback)
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
  if (!callback_ptr_->getIsr())
  {
    return;
  }
  if (mode_ptr_ == &interrupt::mode_low)
  {
    detachInterrupt(digitalPinToInterrupt(pin_));
    enablePullup();
    attachInterrupt(digitalPinToInterrupt(pin_),
                    callback_ptr_->getIsr(),
                    LOW);
  }
  else if (mode_ptr_ == &interrupt::mode_change)
  {
    detachInterrupt(digitalPinToInterrupt(pin_));
    enablePullup();
    attachInterrupt(digitalPinToInterrupt(pin_),
                    callback_ptr_->getIsr(),
                    CHANGE);
  }
  else if (mode_ptr_ == &interrupt::mode_rising)
  {
    detachInterrupt(digitalPinToInterrupt(pin_));
    enablePullup();
    attachInterrupt(digitalPinToInterrupt(pin_),
                    callback_ptr_->getIsr(),
                    RISING);
  }
  else if (mode_ptr_ == &interrupt::mode_falling)
  {
    detachInterrupt(digitalPinToInterrupt(pin_));
    enablePullup();
    attachInterrupt(digitalPinToInterrupt(pin_),
                    callback_ptr_->getIsr(),
                    FALLING);
  }
}

void Interrupt::attach(const Callback & callback, const ConstantString & mode)
{
  setCallback(callback);
  setMode(mode);
}

void Interrupt::detach()
{
  detachInterrupt(digitalPinToInterrupt(pin_));
  disablePullup();
  mode_ptr_ = &interrupt::mode_detached;
}

}
