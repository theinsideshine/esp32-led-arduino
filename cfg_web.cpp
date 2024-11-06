/**
   File: Clase para el manejo de la eeprom y la comunicacion serial y por webservice
 
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




#include "cfg_web.h"
#include "log.h"



#include <ArduinoJson.h>
#include <EEPROM.h>

CConfig::CConfig()
{

}

void CConfig::init() {
    // Iniciar el puerto serial para depuración
    Serial.begin(115200);
    delay(100); // Espera breve para asegurar que el puerto esté listo

    uint32_t magic_number;

    // Iniciar la EEPROM con un tamaño adecuado
    EEPROM.begin(64);  // Cambia 64 por el tamaño que necesites
    EEPROM.get(EEPROM_ADDRESS_MAGIC_NUMBER, magic_number);

    // Imprimir el valor inicial del magic_number leído desde EEPROM
    Serial.print("Valor de magic_number leído: ");
    Serial.println(magic_number);

    if (magic_number != MAGIC_NUMBER) {
        // No coincide, inicializamos valores por defecto
        Serial.println("Magic number no coincide. Configurando valores por defecto...");
        magic_number = MAGIC_NUMBER;
        EEPROM.put(EEPROM_ADDRESS_MAGIC_NUMBER, magic_number);
        
        set_point_position(POINT_POSITION_DEFAULT);
        set_point_speed(POINT_SPEED_DEFAULT);
        set_dead_zone_min(DEAD_ZONE_MIN_DEFAULT);
        set_dead_zone_max(DEAD_ZONE_MAX_DEFAULT);
        set_speed_dead_zone(SPEED_DEAD_ZONE_DEFAULT);
        set_led_blink_time(LED_BLINK_TIME_DEFAULT);
        set_led_blink_quantity(LED_BLINK_QUANTITY_DEFAULT);
        
        set_log_level(LOG_MSG); // Setea log por defecto
        set_st_test(ST_TEST_DEFAULT);
        set_st_mode(ST_MODE_DEFAULT);

        EEPROM.commit();  // Asegura que los valores se escriben en la EEPROM

        Serial.println("Valores de configuración por defecto establecidos y guardados en EEPROM.");
    } else {
        // Cargar los valores desde la EEPROM
        Serial.println("Magic number coincide. Cargando valores de EEPROM...");
        EEPROM.get(EEPROM_ADDRESS_POINT_POSITION, point_position);
        EEPROM.get(EEPROM_ADDRESS_POINT_SPEED, point_speed);
        EEPROM.get(EEPROM_ADDRESS_DEAD_ZONE_MIN, dead_zone_min);
        EEPROM.get(EEPROM_ADDRESS_DEAD_ZONE_MAX, dead_zone_max);
        EEPROM.get(EEPROM_ADDRESS_SPEED_DEAD_ZONE, speed_dead_zone);
        EEPROM.get(EEPROM_ADDRESS_LED_BLINK_TIME, led_blink_time);
        EEPROM.get(EEPROM_ADDRESS_LED_BLINK_QUANTITY, led_blink_quantity);

        EEPROM.get(EEPROM_ADDRESS_LOG_LEVEL, log_level);

        // Imprimir valores leídos
        Serial.print("Point position: ");
        Serial.println(point_position);
        Serial.print("Point speed: ");
        Serial.println(point_speed);
        Serial.print("Dead zone min: ");
        Serial.println(dead_zone_min);
        Serial.print("Dead zone max: ");
        Serial.println(dead_zone_max);
        Serial.print("Speed dead zone: ");
        Serial.println(speed_dead_zone);
        Serial.print("Led blink time: ");
        Serial.println(led_blink_time);
        Serial.print("Led blink quantity: ");
        Serial.println(led_blink_quantity);
        Serial.print("Log level: ");
        Serial.println(log_level);

        set_st_test(ST_TEST_DEFAULT);
        set_st_mode(ST_MODE_DEFAULT);
    }
}


void CConfig::handleHttpRequest(WebServer &server) {
    bool known_key = false;

    if (server.method() == HTTP_GET) {
        StaticJsonDocument<512> doc;  // Documento JSON para llenar la respuesta
    
        // Llenar el doc con todos los parámetros
        send_all_params(doc);

        // Serializar el documento JSON en una cadena
        String response;
        serializeJson(doc, response);
    
        // Enviar la respuesta con código 200 (OK) y el contenido en JSON
        server.send(200, "application/json", response);

    } else if (server.method() == HTTP_PUT) {
        if (server.hasArg("plain")) {
            StaticJsonDocument<512> doc;
            auto error = deserializeJson(doc, server.arg("plain"));
            
            if (!error) {
                // Procesar los parámetros recibidos
                if (doc.containsKey("point_position")) {
                    set_point_position(doc["point_position"]);
                    known_key = true;
                }

                if ( doc.containsKey("point_speed") ) {
                set_point_speed( doc["point_speed"] );
                known_key = true;
                }

                if ( doc.containsKey("dead_zone_min") ) {
                set_dead_zone_min( doc["dead_zone_min"] );
                known_key = true;
                }

                if ( doc.containsKey("dead_zone_max") ) {
                    set_dead_zone_max( doc["dead_zone_max"] );
                    known_key = true;
                }

                if ( doc.containsKey("speed_dead_zone") ) {
                    set_speed_dead_zone( doc["speed_dead_zone"] );
                    known_key = true;
                }

                if ( doc.containsKey("led_blink_time") ) {
                    set_led_blink_time( doc["led_blink_time"] );
                    known_key = true;
                }

                if ( doc.containsKey("led_blink_quantity") ) {
                    set_led_blink_quantity( doc["led_blink_quantity"] );
                    known_key = true;
                }
           
            
                if ( doc.containsKey("log_level") ) {
                    set_log_level( doc["log_level"] );
                    known_key = true;
                }  

                if ( doc.containsKey("st_mode") ) {
                    set_st_mode( doc["st_mode"] );
                    known_key = true;
                } 

                if ( doc.containsKey("info") ) {
                String key = doc["info"];

                if( key == "all-params" ) {
                    send_all_params( doc );
                }else if( key == "all-calibration" ) {
                    send_all_calibration( doc );
                }else if( key == "version" ) {
                    send_version( doc );
                }else if( key == "status" ) {
                    send_status( doc );
                }else if( key == "point_position" ) {
                    send_point_position( doc );
                }else if( key == "point_speed" ) {
                    send_point_speed( doc );
                }else if( key == "dead_zone_min" ) {
                    send_dead_zone_min( doc );
                }else if( key == "dead_zone_max" ) {
                    send_dead_zone_max( doc );
                }else if( key == "speed_dead_zone" ) {
                    send_speed_dead_zone( doc );
                }else if( key == "led_blink_time" ) {
                    send_led_blink_time( doc );
                }else if( key == "led_blink_quantity" ) {
                    send_led_blink_quantity( doc );
                }else if( key == "st_mode" ) {
                    send_st_mode( doc );
                }else if( key == "log_level" ) {
                    send_log_level( doc );
                }               
            }

                if (doc.containsKey("cmd")) {
                    String key = doc["cmd"];                
                              
                    if (key == "start") {
                        set_st_test(1);  // Comienza el ensayo.
                        send_ack(doc);
                    }
                } else if (known_key == true) {
                    send_ok(doc);
                }
                // Serializar el documento JSON en una cadena
                String response;
                serializeJson(doc, response);               
                // Enviar la respuesta con código 200 (OK) y el contenido en JSON
                server.send(200, "application/json", response);

            } else {
                // Enviar una respuesta de error si el JSON no es válido
                server.send(400, "application/json", "{\"result\":\"error\", \"message\":\"Invalid JSON\"}");
            }

        } else {
            // Enviar una respuesta de error si no hay cuerpo en la solicitud
            server.send(400, "application/json", "{\"result\":\"error\", \"message\":\"No body\"}");
        }

    } else {
        // Enviar una respuesta de error si el método no es permitido
        server.send(405, "application/json", "{\"result\":\"error\", \"message\":\"Method not allowed\"}");
    }
}


uint32_t CConfig::get_point_position( void )
{
    return point_position;
}

void CConfig::set_point_position( uint32_t val )
{
    point_position = val;
    EEPROM.put( EEPROM_ADDRESS_POINT_POSITION, val );
    EEPROM.commit();  // Asegura que el valor se escriba
}

uint32_t CConfig::get_point_speed( void )
{
    return point_speed;
}

void CConfig::set_point_speed( uint32_t val )
{
    point_speed = val;
    EEPROM.put( EEPROM_ADDRESS_POINT_SPEED, val );
    EEPROM.commit();  // Asegura que el valor se escriba
}

uint32_t CConfig::get_dead_zone_min( void )
{
    return dead_zone_min;
}

void CConfig::set_dead_zone_min( uint32_t val )
{
   
    dead_zone_min = val;
    EEPROM.put( EEPROM_ADDRESS_DEAD_ZONE_MIN, val );
    EEPROM.commit();  // Asegura que el valor se escriba
}

uint32_t CConfig::get_dead_zone_max( void )
{
    return dead_zone_max;
}

void CConfig::set_dead_zone_max( uint32_t val )
{
    dead_zone_max = val;
    EEPROM.put( EEPROM_ADDRESS_DEAD_ZONE_MAX, val );
    EEPROM.commit();  // Asegura que el valor se escriba
}


float CConfig::get_speed_dead_zone( void )
{
    return speed_dead_zone;
}

void CConfig::set_speed_dead_zone( float val )
{
    speed_dead_zone = val;
    EEPROM.put( EEPROM_ADDRESS_SPEED_DEAD_ZONE, val );
    EEPROM.commit();  // Asegura que el valor se escriba
}

uint32_t CConfig::get_led_blink_time( void )
{
    return led_blink_time;
}

void CConfig::set_led_blink_time( uint32_t val )
{
    led_blink_time = val;
    EEPROM.put( EEPROM_ADDRESS_LED_BLINK_TIME, val );
    EEPROM.commit();  // Asegura que el valor se escriba
}

uint32_t CConfig::get_led_blink_quantity( void )
{
    return led_blink_quantity;
}

void CConfig::set_led_blink_quantity( uint32_t val )
{
    led_blink_quantity = val;
    EEPROM.put( EEPROM_ADDRESS_LED_BLINK_QUANTITY, val );
    EEPROM.commit();  // Asegura que el valor se escriba
}

uint32_t CConfig::get_log_level( void )
{
    return log_level;
}

void CConfig::set_log_level( uint32_t enable )
{
    log_level = enable;
    EEPROM.put( EEPROM_ADDRESS_LOG_LEVEL, log_level );
    EEPROM.commit();  // Asegura que el valor se escriba
}
/*
 * Estos deberia ser booleano !!!!!
 * 
 */
 
uint32_t CConfig::get_st_test( void )
{
    return st_test;
}

void CConfig::set_st_test( uint32_t enable )
{
    st_test = enable;
    EEPROM.put( EEPROM_ADDRESS_ST_TEST, st_test );
    EEPROM.commit();  // Asegura que el valor se escriba
}

uint32_t CConfig::get_st_mode( void )
{
    return st_mode;
}

void CConfig::set_st_mode( uint32_t mode )
{
    st_mode = mode;
    EEPROM.put( EEPROM_ADDRESS_ST_MODE, st_mode );
    EEPROM.commit();  // Asegura que el valor se escriba
}
/*

API Endpoints
1. Obtener Parámetros
Puedes consultar los parámetros configurados en el dispositivo mediante el endpoint GET:

Endpoint: GET {{urlLocalEsp}}/obtenerParametros

curl --location 'http://192.168.0.53/obtenerParametros'


2. Modificar Parámetros y Enviar Comandos
Para modificar parámetros o enviar comandos, utiliza el endpoint PUT:

Endpoint: PUT {{urlLocalEsp}}/parametros

curl --location --request PUT 'http://192.168.0.53/parametros' \
--header 'Content-Type: application/json' \
--data '{ "log_level": "1" }'

Parámetros y Comandos Disponibles
Estos son los parámetros y comandos que puedes enviar mediante JSON en el cuerpo de la solicitud:

General Information:

{ "info": "all-params" } – Envía todos los parámetros en formato JSON.
{ "info": "all-calibration" } – Envía todos los parámetros de calibración.
{ "info": "version" } – Envía la versión del firmware.
{ "info": "status" } – Devuelve el estado del ensayo.
{ "info": "point_position" } – Devuelve la posición del punto.
{ "info": "point_speed" } – Devuelve la velocidad del punto.
{ "info": "dead_zone_min" } – Devuelve el inicio de la zona muerta.
{ "info": "dead_zone_max" } – Devuelve el fin de la zona muerta.
{ "info": "speed_dead_zone" } – Devuelve la velocidad en la zona muerta.
{ "info": "led_blink_time" } – Devuelve el tiempo de parpadeo del LED en ms.
{ "info": "led_blink_quantity" } – Devuelve las veces del parpadeo del LED.
{ "info": "st_mode" } – Devuelve el modo del ensayo.
{ "info": "log_level" } – Devuelve el nivel de log por puerto serie.
Log Level:

{ "log_level": "0" } – Desactiva el log.
{ "log_level": "1" } – Log básico de mensajes.
{ "log_level": "2" } – Log habilitado en formato json.              
{ "log_level": "3" } – Log en formato compatible con Arduino plotter.

Control Commands:

{ "cmd": "start" } – Inicia el ensayo.
Configuración de Parámetros:

{ "point_position": "100" } – Establece la posición del punto.
{ "point_speed": "200" } – Establece la velocidad del punto.
{ "dead_zone_min": "1" } – Configura el inicio de la zona muerta.
{ "dead_zone_max": "2" } – Configura el final de la zona muerta.
{ "speed_dead_zone": "150.0" } – Configura la velocidad en la zona muerta.
{ "led_blink_time": "1000" } – Tiempo en ms para el parpadeo del LED.
{ "led_blink_quantity": "5" } – Cantidad de veces del parpadeo del LED.
{ "st_test": "1" } – Activa el ensayo (0 para desactivado, 1 para activado).
{ "st_mode": "0" } – Configura el modo del ensayo:
0 -   ST_MODE_TEST: Ensayo activado.
200 - ST_MODE_DEMO: Uso del demo


Demo para salida Json
{ "st_mode": "200" ,
"log_level": "2" }

Demo para salida Arduino Plotter
{ "st_mode": "200" ,
"log_level": "3" }

*/


void CConfig::send_all_params( JsonDocument& doc )
{
    doc["point_position"]     = get_point_position();
    doc["point_speed"]        = get_point_speed();
    doc["dead_zone_min"]      = get_dead_zone_min();
    doc["dead_zone_max"]      = get_dead_zone_max();
    doc["speed_dead_zone"]    = get_speed_dead_zone();   
    doc["led_blink_time"]     = get_led_blink_time();  
    doc["led_blink_quantity"] = get_led_blink_quantity();         
    doc["st_test"]            = get_st_test();    
   
    serializeJsonPretty( doc, Serial );
}

/*
 *  Envia todos los parametros de calibracion de la flexion y el nivel de loggeo.
 */

void CConfig::send_all_calibration( JsonDocument& doc )
{
    doc["log_level"] =  get_log_level();
    serializeJsonPretty( doc, Serial );
}

//Envia el status del test
void CConfig::send_test_finish( void )
{
    StaticJsonDocument<512> doc;   
    doc["st_test"] = get_st_test();  
    serializeJsonPretty( doc, Serial );
}



// Envia la version del firmware.
void CConfig::send_version( JsonDocument& doc )
{
    doc["version"] = FIRMWARE_VERSION;

    serializeJsonPretty( doc, Serial );
}

// Envia la point_position
void CConfig::send_point_position( JsonDocument& doc )
{
    doc["point_position"] =  get_point_position();;

    serializeJsonPretty( doc, Serial );
}

// Envia la point_speed
void CConfig::send_point_speed( JsonDocument& doc )
{
    doc["point_speed"] =  get_point_speed();;

    serializeJsonPretty( doc, Serial );
}

// Envia dead_zone_min
void CConfig::send_dead_zone_min( JsonDocument& doc )
{
    doc["dead_zone_min"] =  get_dead_zone_min();

    serializeJsonPretty( doc, Serial );
}

// Envia dead_zone_max
void CConfig::send_dead_zone_max( JsonDocument& doc )
{
    doc["dead_zone_max"] =  get_dead_zone_max();

    serializeJsonPretty( doc, Serial );
}

// Envia speed_dead_zone
void CConfig::send_speed_dead_zone( JsonDocument& doc )
{
    doc["speed_dead_zone"] =  get_speed_dead_zone();

    serializeJsonPretty( doc, Serial );
}


// Envia led_blink_time
void CConfig::send_led_blink_time( JsonDocument& doc )
{
    doc["led_blink_time"] =  get_led_blink_time();

    serializeJsonPretty( doc, Serial );
}

// Envia led_blink_time
void CConfig::send_led_blink_quantity( JsonDocument& doc )
{
    doc["led_blink_quantity"] =  get_led_blink_quantity();

    serializeJsonPretty( doc, Serial );
}

// Envia el nivel de logeo
void CConfig::send_log_level( JsonDocument& doc )
{
    doc["log_level"] =  get_log_level();

    serializeJsonPretty( doc, Serial );
}

// Envia el estatus del ensayo.
void CConfig::send_status( JsonDocument& doc )
{
    doc["status"] = get_st_test();;

    serializeJsonPretty( doc, Serial );
}

// Envia el modo del ensayo.
void CConfig::send_st_mode( JsonDocument& doc )
{
    doc["st_mode"] = get_st_mode();;

    serializeJsonPretty( doc, Serial );
}

// Envia el resultado en formato json
void CConfig::send_ok( JsonDocument& doc )
{
    doc[ "result" ] = "ok";

    serializeJsonPretty( doc, Serial );
}

// Envia el resultado en formato json
void CConfig::send_ack( JsonDocument& doc )
{
    doc[ "result" ] = "ack";

    serializeJsonPretty( doc, Serial );
}