/**
 * File:   Class of software timers based on the millis () function.
 *        
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

 
  
#include "timer.h"

CTimer::CTimer()
{
    start();
}

void CTimer::start( void )
{
    timer = millis();
}

bool CTimer::expired( uint32_t ms )
{
    return ((millis() - timer) > ms);
}
