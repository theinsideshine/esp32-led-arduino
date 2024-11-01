  /**
   File: Clase que da soporte a los leds de debug
  
   - Compiler:           Arduino IDE 2.3.3
   - Supported devices:  ESP32 DEV Module

   Author               pablo.tavolaro@docentes.udemm.edu.ar
   Date:  07-10-2024

        Universidad de la Marina Mercante.
*/
 
#include "led.h"

CLed::CLed()
{

}

void CLed::init(void){
  pinMode(PIN_LED_DEBUG, OUTPUT);
  
}

void CLed::on(void){

   digitalWrite(PIN_LED_DEBUG, LOW); 
 
}

void CLed::off(void){

   digitalWrite(PIN_LED_DEBUG, HIGH);  
}
/*
 * n_blink numero de blinkeos
 * t_blink tiempo de blinkeos en ms
 */
 
void CLed::n_blink(uint8_t n_blink,uint8_t t_blink){

  for (uint8_t i=0; i<n_blink; i++){    
   digitalWrite(PIN_LED_DEBUG, LOW); 
   delay(t_blink);
   digitalWrite(PIN_LED_DEBUG, HIGH); 
   delay(t_blink); 
  }
}
