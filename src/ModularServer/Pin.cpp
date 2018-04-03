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
CONSTANT_STRING(mode_interrupt,"INTERRUPT");
CONSTANT_STRING(mode_digital_input,"DIGITAL_INPUT");
CONSTANT_STRING(mode_digital_output,"DIGITAL_OUTPUT");
}

// public
Pin::Pin()
{
  setup(constants::empty_constant_string);
}

Interrupt * Pin::getInterruptPtr()
{
  return &interrupt_;
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
  }

  Interrupt * interrupt_ptr = getInterruptPtr();
  if (interrupt_ptr != NULL)
  {
    response.write(constants::interrupt_constant_string,interrupt_ptr->getName());
  }

  response.write(constants::mode_constant_string,getMode());

  response.endObject();
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
  pin_number_ = 0;
  interrupt_ptr_ = NULL;
  mode_ptr_ = &pin::mode_detached;
  isr_ = NULL;
}

void Pin::setPinNumber(const size_t pin_number)
{
  pin_number_ = pin_number;
}

size_t Pin::getPinNumber()
{
  return pin_number_;
}

void Pin::enablePullup()
{
  pinMode(pin_number_,INPUT_PULLUP);
}

void Pin::disablePullup()
{
  pinMode(pin_number_,INPUT);
}

void Pin::setInterrupt(Interrupt & interrupt)
{
  interrupt_ptr_ = &interrupt;
  reattach();
}

void Pin::removeInterrupt()
{
  detach();
  interrupt_ptr_ = NULL;
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
  if (!interrupt_ptr_)
  {
    return;
  }
  if (!interrupt_ptr_->getFunctor())
  {
    return;
  }
  if (mode_ptr_ == &pin::mode_low)
  {
    resetIsr();
    detachPin(digitalPinToPin(pin_number_));
    enablePullup();
    attachPin(digitalPinToPin(pin_number_),
                    isr_,
                    LOW);
  }
  else if (mode_ptr_ == &pin::mode_change)
  {
    resetIsr();
    detachPin(digitalPinToPin(pin_number_));
    enablePullup();
    attachPin(digitalPinToPin(pin_number_),
                    isr_,
                    CHANGE);
  }
  else if (mode_ptr_ == &pin::mode_rising)
  {
    resetIsr();
    detachPin(digitalPinToPin(pin_number_));
    enablePullup();
    attachPin(digitalPinToPin(pin_number_),
                    isr_,
                    RISING);
  }
  else if (mode_ptr_ == &pin::mode_falling)
  {
    resetIsr();
    detachPin(digitalPinToPin(pin_number_));
    enablePullup();
    attachPin(digitalPinToPin(pin_number_),
                    isr_,
                    FALLING);
  }
}

void Pin::attach(Interrupt & interrupt, const ConstantString & mode)
{
  setInterrupt(interrupt);
  setMode(mode);
}

void Pin::detach()
{
  detachPin(digitalPinToPin(pin_number_));
  disablePullup();
  mode_ptr_ = &pin::mode_detached;
}

void Pin::resetIsr()
{
  FunctorInterrupts::remove(isr_);
  isr_ = FunctorInterrupts::add(makeFunctor((Functor0 *)0,*this,&Pin::isrHandler));
}

void Pin::isrHandler()
{
  if (!interrupt_ptr_)
  {
    return;
  }
  if (!interrupt_ptr_->getFunctor())
  {
    return;
  }
  interrupt_ptr_->functor(this);
}

}
