/**
   File: Definiciones para el manejo de la eeprom y la comunicacion serial y por webservice
 
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




#ifndef CONFIGWEB_H
#define CONFIGWEB_H

#include "Arduino.h"
#include <WebServer.h>
#include "precompilation.h"
#include <ArduinoJson.h>


                                                  
//#define EEPROM_ADDRESS_CONFIG         4       // Direccion en la epprom donde se almacena la configuracion.
#define MAGIC_NUMBER                    202    // Numero magico para detectar memoria sin inicializar.


#define POINT_POSITION_DEFAULT              10            // Posicion del punto por defecto.
#define POINT_SPEED_DEFAULT                 88            // Velocidad del punto por defecto.
#define DEAD_ZONE_MIN_DEFAULT               180           // Valor de comienzo de la zona muerta por defecto.
#define DEAD_ZONE_MAX_DEFAULT               220           // Valor de final de la zona muerta por defecto. 
#define SPEED_DEAD_ZONE_DEFAULT             150.0         // Valor de la velocidad dentro de la zona muerta, por defecto.
#define LED_BLINK_TIME_DEFAULT              1010          // Tiempo en ms del blinkeo del led
#define LED_BLINK_QUANTITY_DEFAULT          3             // Numero de blinkeo del led
#define ST_TEST_DEFAULT                     0             //  Estado del test pòr defecto.
#define ST_MODE_DEFAULT                     ST_MODE_TEST  //  Modo de operacion del sistema. 


// Mapa de direcciones de los campos de configuracion en la EEPROM. en esp el word es 4 bytes
#define EEPROM_ADDRESS_MAGIC_NUMBER     0
#define EEPROM_ADDRESS_POINT_POSITION     (EEPROM_ADDRESS_MAGIC_NUMBER + sizeof(uint32_t))    // Cambiado a uint32_t
#define EEPROM_ADDRESS_POINT_SPEED        (EEPROM_ADDRESS_POINT_POSITION + sizeof(uint32_t))  // Cambiado a uint32_t
#define EEPROM_ADDRESS_DEAD_ZONE_MIN      (EEPROM_ADDRESS_POINT_SPEED + sizeof(uint32_t))     // Cambiado a uint32_t
#define EEPROM_ADDRESS_DEAD_ZONE_MAX      (EEPROM_ADDRESS_DEAD_ZONE_MIN + sizeof(uint32_t))   // Cambiado a uint32_t
#define EEPROM_ADDRESS_SPEED_DEAD_ZONE    (EEPROM_ADDRESS_DEAD_ZONE_MAX + sizeof(float))      // float permanece como está
#define EEPROM_ADDRESS_LED_BLINK_TIME     (EEPROM_ADDRESS_SPEED_DEAD_ZONE + sizeof(uint32_t)) // Cambiado a uint32_t
#define EEPROM_ADDRESS_LED_BLINK_QUANTITY (EEPROM_ADDRESS_LED_BLINK_TIME + sizeof(uint32_t))  // Cambiado a uint32_t
#define EEPROM_ADDRESS_LOG_LEVEL          (EEPROM_ADDRESS_LED_BLINK_QUANTITY + sizeof(uint32_t))  // Cambiado a uint32_t
#define EEPROM_ADDRESS_ST_TEST            (EEPROM_ADDRESS_LOG_LEVEL + sizeof(uint32_t))       // Cambiado a uint32_t
#define EEPROM_ADDRESS_ST_MODE            (EEPROM_ADDRESS_ST_TEST + sizeof(uint32_t))         // Cambiado a uint32_t

/*
 *  Para poder leer los dispositivo y ejecutar accciones se pone un modo de operacion .
 *  
 */

#define ST_MODE_TEST                    0               // Modo de operacion normal, ensayo activado.
#define ST_MODE_DEMO                    200             //  Demo: Uso del estado modo.


class CConfig
{
  public:
    CConfig();
    void handleHttpRequest(WebServer &server);
    void init();
    uint32_t get_point_position( void );
    void set_point_position( uint32_t );
    
    uint32_t get_point_speed( void );
    void set_point_speed( uint32_t ); 

    uint32_t get_dead_zone_min( void );
    void set_dead_zone_min( uint32_t ); 

    uint32_t get_dead_zone_max( void );
    void set_dead_zone_max( uint32_t );        

    float get_speed_dead_zone( void );
    void set_speed_dead_zone( float );  

    uint32_t get_led_blink_time( void );
    void set_led_blink_time( uint32_t ); 

    uint32_t get_led_blink_quantity( void );
    void set_led_blink_quantity( uint32_t ); 

    uint32_t get_log_level( void );
    void set_log_level( uint32_t enable );

    uint32_t get_st_test( void );
    void set_st_test( uint32_t enable );

    uint32_t get_st_mode( void );
    void set_st_mode( uint32_t mode ); 

    void host_cmd( void );
    void send_test_finish(void);

  private:
    uint32_t log_level;          // 0 = log de informacion de control desactivada.
    uint32_t st_test;            // Estado del ensayo 
    uint32_t st_mode;            // Modo del ensayo


    uint32_t point_position;         // Posicion del punto.
    uint32_t point_speed;            // Velocidad del punto.
    uint32_t dead_zone_min;          // Valor de comienzo de la zona muerta.
    uint32_t dead_zone_max;          // Valor de fin de la zona muerta.    
    float    speed_dead_zone;        // Valor de la velocidad dentro de la zona muerta.
    uint32_t led_blink_time;         // Tiempo en ms de blinkeo del led. 
    uint32_t led_blink_quantity;     // Veces de blinkeo del led. 
  
    
    void send_all_params( JsonDocument& );
    void send_all_calibration( JsonDocument& doc );
    void send_version( JsonDocument& );
    void send_ok( JsonDocument& );
    void send_ack( JsonDocument& );
    void send_status( JsonDocument& doc );
    void send_point_position( JsonDocument& doc );
    void send_point_speed( JsonDocument& doc );    
    void send_dead_zone_min( JsonDocument& doc );
    void send_dead_zone_max( JsonDocument& doc );
    void send_speed_dead_zone( JsonDocument& doc );
    void send_led_blink_time( JsonDocument& doc );
    void send_led_blink_quantity( JsonDocument& doc );

    void send_log_level( JsonDocument& doc );
    void send_st_mode( JsonDocument& doc );
    
    
    
};

#endif // CONFIGWEB_H