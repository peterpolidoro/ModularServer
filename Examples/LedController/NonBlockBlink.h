// ----------------------------------------------------------------------------
// NonBlockBlink.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _NON_BLOCK_BLINK_H_
#define _NON_BLOCK_BLINK_H_
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class NonBlockBlink
{
public:
  NonBlockBlink(int led_pin);
  int duration_on;
  int duration_off;
  int count;
  void start();
  void stop();
  void update();
private:
  int led_pin_;
  int led_state_;
  boolean enabled_;
  int interval_;
  int counter_;
  unsigned long current_time_;
  unsigned long previous_time_;
};
#endif
