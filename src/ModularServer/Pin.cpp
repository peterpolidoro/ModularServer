// ----------------------------------------------------------------------------
// Pin.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "Pin.h"


namespace modular_server
{
EventController<modular_server::constants::PIN_PWM_EVENT_COUNT_MAX> Pin::pin_pwm_event_controller_;

// public
Pin::Pin()
{
  setup(constants::empty_constant_string);
}

void Pin::setModeDigitalInput()
{
  if (callback_ptr_)
  {
    callback_ptr_->detachFrom(*this);
  }
  mode_ptr_ = &constants::pin_mode_digital_input;
  disablePullup();
}

void Pin::setModeDigitalInputPullup()
{
  if (callback_ptr_)
  {
    callback_ptr_->detachFrom(*this);
  }
  mode_ptr_ = &constants::pin_mode_digital_input_pullup;
  enablePullup();
}

void Pin::setModeDigitalOutput()
{
  if (callback_ptr_)
  {
    callback_ptr_->detachFrom(*this);
  }
  mode_ptr_ = &constants::pin_mode_digital_output;
  pinMode(pin_number_,OUTPUT);
}

void Pin::setModeAnalogInput()
{
  if (callback_ptr_)
  {
    callback_ptr_->detachFrom(*this);
  }
  mode_ptr_ = &constants::pin_mode_analog_input;
  disablePullup();
}

void Pin::setModeAnalogOutput()
{
  if (callback_ptr_)
  {
    callback_ptr_->detachFrom(*this);
  }
  mode_ptr_ = &constants::pin_mode_analog_output;
  pinMode(pin_number_,OUTPUT);
}

void Pin::setModePulseRising()
{
  if (callback_ptr_)
  {
    callback_ptr_->detachFrom(*this);
  }
  mode_ptr_ = &constants::pin_mode_pulse_rising;
  pinMode(pin_number_,OUTPUT);
  ::digitalWrite(pin_number_,LOW);
}

void Pin::setModePulseFalling()
{
  if (callback_ptr_)
  {
    callback_ptr_->detachFrom(*this);
  }
  mode_ptr_ = &constants::pin_mode_pulse_falling;
  pinMode(pin_number_,OUTPUT);
  ::digitalWrite(pin_number_,HIGH);
}

void Pin::setModePwmRising()
{
  if (callback_ptr_)
  {
    callback_ptr_->detachFrom(*this);
  }
  mode_ptr_ = &constants::pin_mode_pwm_rising;
  pinMode(pin_number_,OUTPUT);
  ::digitalWrite(pin_number_,LOW);
}

void Pin::setModePwmFalling()
{
  if (callback_ptr_)
  {
    callback_ptr_->detachFrom(*this);
  }
  mode_ptr_ = &constants::pin_mode_pwm_falling;
  pinMode(pin_number_,OUTPUT);
  ::digitalWrite(pin_number_,HIGH);
}

int Pin::getValue()
{
  int value = 0;
  if (mode_ptr_ != &constants::pin_mode_analog_input)
  {
    value = ::digitalRead(pin_number_);
  }
  else
  {
    value = ::analogRead(pin_number_);
  }
  return value;
}

void Pin::setValue(int value)
{
  if (mode_ptr_ == &constants::pin_mode_digital_output)
  {
    if (value <= 0)
    {
      ::digitalWrite(pin_number_,LOW);
    }
    else
    {
      ::digitalWrite(pin_number_,HIGH);
    }
  }
  else if (mode_ptr_ == &constants::pin_mode_analog_output)
  {
    ::analogWrite(pin_number_,value);
  }
  else if (mode_ptr_ == &constants::pin_mode_pulse_rising)
  {
    EventIdPair event_id_pair = pin_pwm_event_controller_.addPwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&Pin::setPinHighHandler),
      makeFunctor((Functor1<int> *)0,*this,&Pin::setPinLowHandler),
      constants::pin_pulse_delay,
      value*2,
      value,
      constants::pin_pulse_count);
    pin_pwm_event_controller_.enable(event_id_pair);
  }
  else if (mode_ptr_ == &constants::pin_mode_pulse_falling)
  {
    EventIdPair event_id_pair = pin_pwm_event_controller_.addPwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&Pin::setPinLowHandler),
      makeFunctor((Functor1<int> *)0,*this,&Pin::setPinHighHandler),
      constants::pin_pulse_delay,
      value*2,
      value,
      constants::pin_pulse_count);
    pin_pwm_event_controller_.enable(event_id_pair);
  }
}

void Pin::addPwm(uint32_t period_ms,
  uint32_t on_duration_ms,
  int32_t count)
{
  if (mode_ptr_ == &constants::pin_mode_pwm_rising)
  {
    EventIdPair event_id_pair = pin_pwm_event_controller_.addPwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&Pin::setPinHighHandler),
      makeFunctor((Functor1<int> *)0,*this,&Pin::setPinLowHandler),
      constants::pin_pulse_delay,
      period_ms,
      on_duration_ms,
      count);
    pin_pwm_event_controller_.enable(event_id_pair);
  }
  else if (mode_ptr_ == &constants::pin_mode_pwm_falling)
  {
    EventIdPair event_id_pair = pin_pwm_event_controller_.addPwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&Pin::setPinLowHandler),
      makeFunctor((Functor1<int> *)0,*this,&Pin::setPinHighHandler),
      constants::pin_pulse_delay,
      period_ms,
      on_duration_ms,
      count);
    pin_pwm_event_controller_.enable(event_id_pair);
  }
}

size_t Pin::getPinNumber()
{
  return pin_number_;
}

int Pin::getInterruptNumber()
{
  return interrupt_number_;
}

// protected

// private
Pin::Pin(const ConstantString & name,
  size_t pin_number)
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
  mode_ptr_ = &constants::pin_mode_digital_input;
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

void Pin::setMode(const ConstantString & pin_mode)
{
  if ((&pin_mode == &constants::pin_mode_interrupt_low) ||
    (&pin_mode == &constants::pin_mode_interrupt_change) ||
    (&pin_mode == &constants::pin_mode_interrupt_rising) ||
    (&pin_mode == &constants::pin_mode_interrupt_falling))
  {
    mode_ptr_ = &pin_mode;
    reattach();
  }
  else if (&pin_mode == &constants::pin_mode_digital_input)
  {
    setModeDigitalInput();
  }
  else if (&pin_mode == &constants::pin_mode_digital_input_pullup)
  {
    setModeDigitalInputPullup();
  }
  else if (&pin_mode == &constants::pin_mode_digital_output)
  {
    setModeDigitalOutput();
  }
  else if (&pin_mode == &constants::pin_mode_analog_input)
  {
    setModeAnalogInput();
  }
  else if (&pin_mode == &constants::pin_mode_analog_output)
  {
    setModeAnalogOutput();
  }
  else if (&pin_mode == &constants::pin_mode_pulse_rising)
  {
    setModePulseRising();
  }
  else if (&pin_mode == &constants::pin_mode_pulse_falling)
  {
    setModePulseFalling();
  }
  else if (&pin_mode == &constants::pin_mode_pwm_rising)
  {
    setModePwmRising();
  }
  else if (&pin_mode == &constants::pin_mode_pwm_falling)
  {
    setModePwmFalling();
  }
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

  response.write(constants::pin_mode_constant_string,getMode());

  response.endObject();
}

void Pin::setPinNumber(size_t pin_number)
{
  interrupt_number_ = digitalPinToInterrupt(pin_number);
  pin_number_ = pin_number;
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
  if (mode_ptr_ == &constants::pin_mode_interrupt_low)
  {
    resetIsr();
    detachInterrupt(interrupt_number_);
    attachInterrupt(interrupt_number_,
      isr_,
      LOW);
  }
  else if (mode_ptr_ == &constants::pin_mode_interrupt_change)
  {
    resetIsr();
    detachInterrupt(interrupt_number_);
    attachInterrupt(interrupt_number_,
      isr_,
      CHANGE);
  }
  else if (mode_ptr_ == &constants::pin_mode_interrupt_rising)
  {
    resetIsr();
    detachInterrupt(interrupt_number_);
    attachInterrupt(interrupt_number_,
      isr_,
      RISING);
  }
  else if (mode_ptr_ == &constants::pin_mode_interrupt_falling)
  {
    resetIsr();
    detachInterrupt(interrupt_number_);
    attachInterrupt(interrupt_number_,
      isr_,
      FALLING);
  }
}

void Pin::attach(Callback & callback,
  const ConstantString & mode)
{
  if (interrupt_number_ == NOT_AN_INTERRUPT)
  {
    return;
  }
  setCallback(callback);
  setMode(mode);
}

void Pin::detach()
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
  detachInterrupt(interrupt_number_);
  mode_ptr_ = &constants::pin_mode_digital_input;
}

void Pin::resetIsr()
{
  FunctorCallbacks::remove(isr_);
  isr_ = FunctorCallbacks::add(makeFunctor((Functor0 *)0,*this,&Pin::isrHandler));
}

void Pin::setupPinPulseEventController()
{
  pin_pwm_event_controller_.setup(constants::pin_pulse_timer_number);
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

void Pin::setPinHighHandler(int index)
{
  ::digitalWrite(pin_number_,HIGH);
}

void Pin::setPinLowHandler(int index)
{
  ::digitalWrite(pin_number_,LOW);
}

}
