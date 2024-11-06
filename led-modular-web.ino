/**
   File:  
          Se propone familiarizarse con el maquina de estado de , 
          uso de codigo de terceros, C++
  

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

/*
Proyecto de Control de Parpadeo de LED en ESP32 con Interfaz Web
Descripción del Proyecto
Este proyecto presenta una arquitectura modular para el ESP32 utilizando Arduino,
 diseñada para soportar sistemas de control complejos y no bloqueantes.
  El sistema proporciona control mediante una interfaz web, persistencia de datos y salida serial 
  compatible con formatos JSON, texto y gráfico. 
  Implementa una estructura de máquina de estados adaptable a diversas aplicaciones, 
  como el control de una puerta motorizada con seguimiento preciso de posición. 
  En el ejemplo incluido, un LED parpadea utilizando temporizadores no bloqueantes ajustables.

Características
Soporte para Métodos No Bloqueantes: Arquitectura diseñada para manejar controles no bloqueantes, 
adecuada para gestionar dispositivos complejos como puertas motorizadas. 
Por ejemplo, el sistema maneja una máquina de estados principal para el estado general de la puerta y una máquina de estados 
adicional con control PID para una gestión precisa de la posición.
Control a través de Servicios Web: Aloja un servidor web en el puerto 80, 
permitiendo el control dinámico de configuraciones y estados mediante solicitudes HTTP GET y PUT.
Persistencia de Datos: Garantiza que las configuraciones se almacenen y puedan actualizarse dinámicamente, 
proporcionando funcionalidad constante incluso después de reinicios.
Salida Serial Flexible: Soporta salidas seriales estructuradas en formato JSON, texto simple y gráfico, 
útiles para el registro de datos en tiempo real y la depuración.
Implementación de Máquina de Estados: Una máquina de estados principal supervisa los estados de alto nivel del dispositivo,
 y permite el uso de procesos de control secundarios, como el control de posición basado en PID para operaciones motorizadas.
Temporizadores No Bloqueantes: El parpadeo del LED y otras acciones temporizadas se gestionan con temporizadores no bloqueantes, 
asegurando una operación fluida sin retrasar los procesos principales.
Estructura del Proyecto
Descripción de Archivos:
log.h, cfg_web.h, led.h, precompilation.h, timer.h: Definen el registro de eventos, manejo de configuración, control de LED, opciones de precompilación y utilidades de temporización.
Archivo principal .ino: Inicializa el Wi-Fi, el servidor web y el hardware, y coordina el flujo de control mediante una máquina de estados.
Configuración de Ejemplo
El código proporcionado:

Establece una conexión Wi-Fi e inicializa un servidor web para acceder al control del sistema.
Utiliza temporizadores no bloqueantes para gestionar la frecuencia de parpadeo del LED.
Registra actividades y estados en el monitor serial, facilitando la depuración y los ajustes en tiempo real.
Este marco proporciona una base sólida para agregar dispositivos complejos como puertas motorizadas, habilitando un control preciso a través de múltiples máquinas de estados y admitiendo ajustes basados en PID.

Configuración
Credenciales Wi-Fi: Reemplace ssid y password con los detalles de su red.
Estados de la Máquina de Estados: Los estados principales se gestionan mediante ST_LOOP_INIT, ST_LOOP_IDLE, ST_LOOP_LED_ON, y ST_LOOP_LED_OFF.
*/



#include "log.h"
#include "cfg_web.h"
#include "led.h"
#include "precompilation.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include "timer.h"


const char* ssid = "Pablo";
const char* password = "01410398716";


/*
 * Estado del loop principal de ejecucion
 */

#define ST_LOOP_INIT                    0     // Espera el modo de ejecucion.
#define ST_LOOP_IDLE                    1     // Empieza el ensayo.
#define ST_LOOP_LED_ON                  2     // Prende led.
#define ST_LOOP_LED_OFF                 3     // Apaga led.
#define ST_LOOP_END                     4     // Termina el ensayo.
#define ST_MODE_RUN_DEMO1               5     // Demo del uso del estado mode y serialmode.





/*
 * Clases del sistema instanciadas
 */
Clog    Log;
CConfig Config;
CLed     Led;
WebServer server(80);  // Inicializa el servidor web en el puerto 80


static void run_demo_serial_plotter( void);            // Demo del uso del serial_plotter


/*
 * Realiza el final de ensayo
 * vuelve el st_test = 0 y
 */

void end_experiment( void ) {
 
  
  Log.msg( F("Ensayo terminado"));
  
  Config.set_st_test( false );
  Config.send_test_finish(); // Informa por el puerto serie que termino el ensayo 
}



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
  static CTimer   Timer_led;
  static uint32_t led_blink_qty = 2;
  

   
  server.handleClient();  // Maneja las solicitudes del cliente
  
  // Actualiza el nivel de log para detener en tiempo real el envio de parametros.
  Log.set_level( Config.get_log_level() );
  
 
  switch ( st_loop ) {

    case ST_LOOP_INIT:

       if (Config.get_st_test() == true ) {                   // Espera que se comienzo al ensayo.        
          st_loop = ST_LOOP_IDLE; 
       }else if (Config.get_st_mode() == ST_MODE_DEMO ) { // Espera el modo runExample1
        st_loop =  ST_MODE_RUN_DEMO1 ;
      }
      
    break;

     // Espera "eventos" de ejecucion
    case ST_LOOP_IDLE:   
          led_blink_qty =  Config.get_led_blink_quantity();     // Carga las veces a blinkear
          Timer_led.start();
          Led.on();
          Log.msg( F("LED ON"));
          st_loop = ST_LOOP_LED_ON;       
      
    break;

    
    
    case ST_LOOP_LED_ON:     

     

      if( Timer_led.expired( Config.get_led_blink_time() ) ) {
                
           Log.msg( F("LED OFF"));      
           Led.off();             
           Timer_led.start();
           st_loop = ST_LOOP_LED_OFF; 
          }

    break;

   
    case ST_LOOP_LED_OFF:
    
      if( Timer_led.expired( Config.get_led_blink_time() ) ) {

          led_blink_qty--; // Decrementa la cantidad de blinks restantes
          if (led_blink_qty > 0) {
            Led.on();
            Log.msg(F("LED ON"));
            Timer_led.start();
            st_loop = ST_LOOP_LED_ON;
          } else {
            st_loop = ST_LOOP_END; // Termina el experimento después de 3 blinks
          }
               
        }

    break;

    case ST_LOOP_END: 

    // Envia comando de final de experimento  
       end_experiment();   
      st_loop = ST_LOOP_INIT;
    break; 

    case ST_MODE_RUN_DEMO1: 

    // Envia comando de final de experimento  
       run_demo_serial_plotter();   
      st_loop = ST_LOOP_INIT;
    break;      
      
      default:
      st_loop = ST_LOOP_INIT;

  }
  
#ifdef ST_DEBUG
  Log.msg( F("ST_LOOP= %d"), st_loop );
#endif //ST_DEBUG

}




 static void run_demo_serial_plotter( void ){
  
    uint16_t raw, filtered,danger_point=2500;
    uint8_t state=0; 
    
    Led.n_blink(5, 500);
    for (raw=0;raw<5000;raw++){
        filtered = raw+ 500 ;
        if (filtered >danger_point ){ 
          state=1 ;
        }
        Log.ctrl (raw, filtered,state,danger_point);
    }
     Config.set_st_mode( ST_MODE_TEST );
 }
