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

void Interrupt::enablePullup()
{
  pullup_ = true;
  pinMode(pin_,INPUT_PULLUP);
}

void Interrupt::disablePullup()
{
  pullup_ = false;
  pinMode(pin_,INPUT);
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

bool Interrupt::getPullup()
{
  return pullup_;
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
  pullup_ = false;
}

void Interrupt::setPin(size_t pin)
{
  number_ = digitalPinToInterrupt(pin);
  pin_ = pin;
}

void Interrupt::attach(Callback * callback_ptr, const ConstantString & mode)
{
  if (!callback_ptr->getIsr())
  {
    return;
  }
  if (&mode == &interrupt::mode_detached)
  {
    detach();
  }
  else if (&mode == &interrupt::mode_low)
  {
    callback_ptr_ = callback_ptr;
    mode_ptr_ = &interrupt::mode_low;
    attachInterrupt(digitalPinToInterrupt(pin_),
                    callback_ptr_->getIsr(),
                    LOW);
  }
  else if (&mode == &interrupt::mode_change)
  {
    callback_ptr_ = callback_ptr;
    mode_ptr_ = &interrupt::mode_change;
    attachInterrupt(digitalPinToInterrupt(pin_),
                    callback_ptr_->getIsr(),
                    CHANGE);
  }
  else if (&mode == &interrupt::mode_rising)
  {
    callback_ptr_ = callback_ptr;
    mode_ptr_ = &interrupt::mode_rising;
    attachInterrupt(digitalPinToInterrupt(pin_),
                    callback_ptr_->getIsr(),
                    RISING);
  }
  else if (&mode == &interrupt::mode_falling)
  {
    callback_ptr_ = callback_ptr;
    mode_ptr_ = &interrupt::mode_falling;
    attachInterrupt(digitalPinToInterrupt(pin_),
                    callback_ptr_->getIsr(),
                    FALLING);
  }
}

void Interrupt::detach()
{
  detachInterrupt(digitalPinToInterrupt(pin_));
  callback_ptr_ = NULL;
  mode_ptr_ = &interrupt::mode_detached;
}

}
