// ----------------------------------------------------------------------------
// Blinker.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _BLINKER_H_
#define _BLINKER_H_
#include <ModularServer.h>

#include "Constants.h"


class Blinker
{
public:
  void setup(modular_server::Pin & pin);
  void start();
  void stop();
  void update();
  void setDurationOn(double value);
  void setDurationOff(double value);
  void setCount(long value);
private:
  int duration_on_;
  int duration_off_;
  int count_;
  modular_server::Pin * led_pin_ptr_;
  int led_state_;
  bool enabled_;
  size_t interval_;
  int counter_;
  unsigned long current_time_;
  unsigned long previous_time_;
};

#endif
