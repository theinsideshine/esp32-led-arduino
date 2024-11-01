/**
   File:  
          Se propone familiarizarse con el maquina de estado de , 
          uso de codigo de terceros, C++
  

   - Compiler:           Arduino IDE 2.3.3
   - Supported devices:  ESP32 DEV Module

   Author               pablo.tavolaro@docentes.udemm.edu.ar
   Date:  07-10-2024

        Universidad de la Marina Mercante.
*/


#include "log.h"
#include "cfg_web.h"
#include "led.h"
#include "precompilation.h"
#include <WiFi.h>
#include <WiFiClient.h>


const char* ssid = "Pablo";
const char* password = "01410398716";


/*
 * Estado del loop principal de ejecucion
 */

#define ST_LOOP_INIT                    0     // Inicializa el programa (carga la configuracion).
#define ST_LOOP_IDLE                    1     // Espera la recepcion por comando.
#define ST_LOOP_LED_ON                  2     // Prende led
#define ST_LOOP_LED_OFF                 3     // Apaga led






/*
 * Clases del sistema instanciadas
 */
Clog    Log;
CConfig Config;
CLed     Led;
WebServer server(80);  // Inicializa el servidor web en el puerto 80
/*
 *  Inicializa los dispositivos del ensayo segun opciones de precompilacion.
 */



void setup()
{  
  Serial.begin(LOG_SERIAL_SPEED);
  delay(100); // Espera breve para asegurar que el puerto esté listo
  Config.init();
  Log.init( Config.get_log_level() );
  
  Serial.println("Init Serial");

  /*
      Para activar la visualisacion  enviar por serie {log_level:'1'}
  */

  Log.msg( F("Programacion avanzada - %s -ESP32"), FIRMWARE_VERSION );
  Log.msg( F("UDEMM - 2024") );

  // Inicialización de la conexión Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a Wi-Fi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConectado a Wi-Fi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Inicializa el servidor web
  server.on("/obtenerParametros", HTTP_GET, []() {
    Config.handleHttpRequest(server);
  });
  
  
  server.on("/parametros", HTTP_PUT, []() {
    Config.handleHttpRequest(server);
  });
  
  
  server.begin();  // Inicia el servidor web
  Serial.println("Servidor web iniciado");

  #ifdef LED_PRESENT
    Led.init();
    Log.msg( F("Led init") );
    delay(2000); // Espera para pasar de estado.
    Led.n_blink(2, 2000); // 2 blinks cada 1000 ms
  #endif // LED_PRESENT   

  Log.msg( F("Sistema inicializado") );
}


void loop()
{
  static uint8_t  st_loop = ST_LOOP_INIT;   
  

   
  server.handleClient();  // Maneja las solicitudes del cliente
  
  // Actualiza el nivel de log para detener en tiempo real el envio de parametros.
  Log.set_level( Config.get_log_level() );
  
 
  switch ( st_loop ) {

    case ST_LOOP_INIT:

       if (Config.get_st_test() == true ) {                   // Espera que se comienzo al ensayo.        
        st_loop = ST_LOOP_LED_ON; 
      }
      
    break;

    
    
    case ST_LOOP_LED_ON:

            
      Log.msg( F("LED ON"));
       Led.on(); 
      delay(1000); // Espera para pasar de estado.
      st_loop = ST_LOOP_LED_OFF;

    break;

   
    case ST_LOOP_LED_OFF:
    
      Log.msg( F("LED OFF"));      
      Led.off(); 
      delay(1000); // Espera para pasar de estado.
      st_loop = ST_LOOP_LED_ON;
      
    break;       
      
      default:
      st_loop = ST_LOOP_INIT;

  }
  
#ifdef ST_DEBUG
  Log.msg( F("ST_LOOP= %d"), st_loop );
#endif //ST_DEBUG

}
