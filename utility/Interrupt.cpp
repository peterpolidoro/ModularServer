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
// public
Interrupt::Interrupt()
{
  setup(constants::empty_constant_string);
}

void Interrupt::setPin(size_t pin)
{
  pin_ = pin;
  number_ = digitalPinToInterrupt(pin);
  if (!pullup_)
  {
    pinMode(pin_,INPUT);
  }
  else
  {
    pinMode(pin_,INPUT_PULLUP);
  }
}

void Interrupt::enablePullup()
{
  pullup_ = true;
  pinMode(pin_,INPUT_PULLUP);
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
  pin_ = 0;
  number_ = 0;
  callback_ptr_ = NULL;
  pullup_ = false;
}

size_t Interrupt::getPin()
{
  return pin_;
}

size_t Interrupt::getNumber()
{
  return number_;
}

Callback * Interrupt::getCallbackPtr()
{
  return callback_ptr_;
}

bool Interrupt::getPullup()
{
  return pullup_;
}

}
