// ----------------------------------------------------------------------------
// NonBlockBlink.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "NonBlockBlink.h"

NonBlockBlink::NonBlockBlink(int led_pin) :
  led_pin_(led_pin)
{
  duration_on = 300;
  duration_off = 500;
  count = 10;
}

void NonBlockBlink::start()
{
  if (count > 0)
  {
    previous_time_ = millis();
    led_state_ = HIGH;
    digitalWrite(led_pin_, led_state_);
    interval_ = duration_on;
    counter_ = 0;
    enabled_ = true;
  }
}

void NonBlockBlink::stop()
{
  enabled_ = false;
}

void NonBlockBlink::update()
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
        interval_ = duration_on;
      }
      else
      {
        led_state_ = LOW;
        interval_ = duration_off;
        counter_++;
        if (counter_ >= count)
        {
          stop();
        }
      }

      digitalWrite(led_pin_, led_state_);
    }
  }
}
