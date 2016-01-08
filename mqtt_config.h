#ifndef THEAIRBOARDWIFLYMQTTPUBLISHER_MQTT_CONFIG_H_
#define THEAIRBOARDWIFLYMQTTPUBLISHER_MQTT_CONFIG_H_


// MQTT parameters
//byte mqtt_server_addr[]           = { 192, 168, 1, 55 };    // Pi
//byte mqtt_server_addr[]           = { 192, 168, 1, 30 };    // Airology
byte mqtt_server_addr[]           = { 192, 168, 42, 1 };    // Pi
char mqtt_client_id[]             = "robustness";
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


// DHT22 status messages
const char status_ok[]                PROGMEM = "DHT22: OK";
const char checksum_error[]           PROGMEM = "DHT22: Checksum error";
const char timeout_error[]            PROGMEM = "DHT22: Time out error";
const char connect_error[]            PROGMEM = "DHT22: Connect error";
const char ack_low_error[]            PROGMEM = "DHT22: Ack Low error";
const char ack_high_error[]           PROGMEM = "DHT22: Ack High error";
const char unknown_error[]            PROGMEM = "DHT22: Unknown error";

PGM_P const dht22_status_messages[]   PROGMEM = { status_ok,        // idx = 0
                                                  checksum_error,   // idx = 1
                                                  timeout_error,    // idx = 2
                                                  connect_error,    // idx = 3
                                                  ack_low_error,    // idx = 4
                                                  ack_high_error,   // idx = 5
                                                  unknown_error,    // idx = 6
                                                };

// measurement topics

const char MEASUREMENTS_START[]       PROGMEM = "theairboard/measurement/START";
const char MEASUREMENTS_END[]         PROGMEM = "theairboard/measurement/END";
const char AIRBOARD_BATTERY_TOPIC[]   PROGMEM = "theairboard/measurement/BATTERY";
const char DHT22_temp_topic[]         PROGMEM = "theairboard/measurement/DHT22_temp";
const char DHT22_humidity_topic[]     PROGMEM = "theairboard/measurement/DHT22_humidity";

//tables to refer to strings
PGM_P const MEASUREMENT_TOPICS[]       PROGMEM = { MEASUREMENTS_START,     // idx = 0
                                                  MEASUREMENTS_END,       // idx = 1
                                                  AIRBOARD_BATTERY_TOPIC, // idx = 2
                                                  DHT22_temp_topic,       // idx = 3
                                                  DHT22_humidity_topic,   // idx = 4
                                                };


#endif  /* THEAIRBOARDWIFLYMQTTPUBLISHER_MQTT_CONFIG_H_ */

