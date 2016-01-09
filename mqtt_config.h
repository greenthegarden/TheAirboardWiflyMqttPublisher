#ifndef THEAIRBOARDWIFLYMQTTPUBLISHER_MQTT_CONFIG_H_
#define THEAIRBOARDWIFLYMQTTPUBLISHER_MQTT_CONFIG_H_


// MQTT parameters
//byte mqtt_server_addr[]           = { 192, 168, 1, 55 };    // Pi
//byte mqtt_server_addr[]           = { 192, 168, 1, 30 };    // Airology
byte mqtt_server_addr[]           = { 192, 168, 42, 1 };    // Pi
char mqtt_client_id[]             = "theairboard";
const int MQTT_PORT               = 1883;
#define MQTT_MAX_PACKET_SIZE        168
#define MQTT_KEEPALIVE              300


const char COMMAND_SEPARATOR      = ':';

const byte BUFFER_SIZE            = 32;
char prog_buffer[BUFFER_SIZE];
char char_buffer[BUFFER_SIZE];
char message[BUFFER_SIZE];


// MQTT topic definitions

// status topics
const char CONNECTION_STATUS[] PROGMEM = "theairboard/status/connected";
const char UPTIME_STATUS[]     PROGMEM = "theairboard/status/uptime";
const char MEMORY_STATUS[]     PROGMEM = "theairboard/status/memory";
const char BATTERY_STATUS[]    PROGMEM = "theairboard/status/battery";
const char DHT22_STATUS[]      PROGMEM = "theairboard/status/dht22";

PGM_P const STATUS_TOPICS[]    PROGMEM = {CONNECTION_STATUS,// idx = 0
                                          UPTIME_STATUS,    // idx = 1
                                          MEMORY_STATUS,    // idx = 2
                                          BATTERY_STATUS,   // idx = 3
                                          DHT22_STATUS      // idx = 4
                                          };

// measurement topics

const char MEASUREMENTS_START[]       PROGMEM = "theairboard/measurement/START";
const char MEASUREMENTS_END[]         PROGMEM = "theairboard/measurement/END";
const char DHT22_temp_topic[]         PROGMEM = "theairboard/measurement/DHT22_T";
const char DHT22_humidity_topic[]     PROGMEM = "theairboard/measurement/DHT22_H";

//tables to refer to strings
PGM_P const MEASUREMENT_TOPICS[]      PROGMEM = { MEASUREMENTS_START,     // idx = 0
                                                  MEASUREMENTS_END,       // idx = 1
                                                  DHT22_temp_topic,       // idx = 2
                                                  DHT22_humidity_topic,   // idx = 3
                                                };


#endif  /* THEAIRBOARDWIFLYMQTTPUBLISHER_MQTT_CONFIG_H_ */

