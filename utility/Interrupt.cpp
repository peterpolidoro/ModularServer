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
  number_ = digitalPinToInterrupt(pin);
  pin_ = pin;
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

void Interrupt::setMode()
{
  mode_ = mode;
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
  pullup_ = false;
  callback_ptr_ = NULL;
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
