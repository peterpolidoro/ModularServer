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
CONSTANT_STRING(mode_detached,"DETACHED");
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

size_t Pin::getNumber()
{
  return number_;
}

size_t Pin::getPin()
{
  return pin_;
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
                   bool write_number_pin_details)
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

  if (write_number_pin_details)
  {
    response.write(constants::number_constant_string,getNumber());

    response.write(constants::pin_constant_string,getPin());
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
Pin::Pin(const ConstantString & name, const size_t pin)
{
  setup(name);
  setPin(pin);
}

void Pin::setup(const ConstantString & name)
{
  setName(name);
  number_ = 0;
  pin_ = 0;
  callback_ptr_ = NULL;
  mode_ptr_ = &pin::mode_detached;
  isr_ = NULL;
}

void Pin::setPin(const size_t pin)
{
  number_ = digitalPinToPin(pin);
  pin_ = pin;
}

void Pin::enablePullup()
{
  pinMode(pin_,INPUT_PULLUP);
}

void Pin::disablePullup()
{
  pinMode(pin_,INPUT);
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
  if (mode_ptr_ == &pin::mode_low)
  {
    resetIsr();
    detachPin(digitalPinToPin(pin_));
    enablePullup();
    attachPin(digitalPinToPin(pin_),
              isr_,
              LOW);
  }
  else if (mode_ptr_ == &pin::mode_change)
  {
    resetIsr();
    detachPin(digitalPinToPin(pin_));
    enablePullup();
    attachPin(digitalPinToPin(pin_),
              isr_,
              CHANGE);
  }
  else if (mode_ptr_ == &pin::mode_rising)
  {
    resetIsr();
    detachPin(digitalPinToPin(pin_));
    enablePullup();
    attachPin(digitalPinToPin(pin_),
              isr_,
              RISING);
  }
  else if (mode_ptr_ == &pin::mode_falling)
  {
    resetIsr();
    detachPin(digitalPinToPin(pin_));
    enablePullup();
    attachPin(digitalPinToPin(pin_),
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
  detachPin(digitalPinToPin(pin_));
  disablePullup();
  mode_ptr_ = &pin::mode_detached;
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
