// ----------------------------------------------------------------------------
// Blinker.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Blinker.h"

void Blinker::setup(modular_server::Pin & pin)
{
  led_pin_ptr_ = &pin;
  duration_on_ = 300;
  duration_off_ = 500;
  count_ = 10;
}

void Blinker::start()
{
  if (count_ > 0)
  {
    previous_time_ = millis();
    led_state_ = HIGH;
    led_pin_ptr_->setValue(led_state_);
    interval_ = duration_on_;
    counter_ = 0;
    enabled_ = true;
  }
}

void Blinker::stop()
{
  enabled_ = false;
}

void Blinker::update()
{
  if (enabled_)
  {
    current_time_ = millis();

    if((current_time_ - previous_time_) > interval_)
    {
      previous_time_ = current_time_;
      if (led_state_ == LOW)
      {
        led_state_ = HIGH;
        interval_ = duration_on_;
      }
      else
      {
        led_state_ = LOW;
        interval_ = duration_off_;
        counter_++;
        if (counter_ >= count_)
        {
          stop();
        }
      }

      led_pin_ptr_->setValue(led_state_);
    }
  }
}

void Blinker::setDurationOn(double value)
{
  duration_on_ = (int)(value*1000);
}

void Blinker::setDurationOff(double value)
{
  duration_off_ = (int)(value*1000);
}

void Blinker::setCount(long value)
{
  count_ = (long)value;
}
