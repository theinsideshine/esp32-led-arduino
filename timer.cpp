 /**
 * File: Class of software timers based on the millis () function
 * from arduino that being based on a cpu timer by irq,
 * does not block execution.
 *
 * - Compiler:           Arduino IDE 2.3.3
 * - Supported devices:  ESP32 DEV Module. = Esp32-WROOM-REV3
 *
 * -author               educacion.ta@gmail.com
 *                       
 *                       
* Date:  27-11-2024
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
