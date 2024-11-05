/**
 * File: Class of software timers based on the millis () function
 * from arduino that being based on a cpu timer by irq,
 * does not block execution.
 *
 * - Compiler:           Arduino IDE 2.3.3
 * - Supported devices:  ESP32 DEV Module
 *
 * -author               educacion.ta@gmail.com
 *                       
 *                       
* Date:  07-10-2024
 *
 *      The inside shine.
 *
  */
 
#ifndef TIMER_H
#define TIMER_H

#include "Arduino.h"

class CTimer
{
  public:
    CTimer();
    void start();
    bool expired(uint32_t);

  private:
      uint32_t timer;
};

#endif // TIMER_H
