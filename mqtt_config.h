#ifndef THEAIRBOARDWIFLYMQTTPUBLISHER_MQTT_CONFIG_H_
#define THEAIRBOARDWIFLYMQTTPUBLISHER_MQTT_CONFIG_H_

#include <PubSubClient.h>



// MQTT parameters
//byte mqtt_server_addr[]           = { 192, 168, 1, 55 };  // Pi eth0 interfaceß
byte mqtt_server_addr[]           = { 192, 168, 42, 1 };    // Pi wlan interface
char mqtt_client_id[]             = "theairboard";
const int MQTT_PORT               = 1883;
#define MQTT_MAX_PACKET_SIZE        168
#define MQTT_KEEPALIVE              300

const byte BUFFER_SIZE            = 32;
char prog_buffer[BUFFER_SIZE];
char char_buffer[BUFFER_SIZE];
char message[BUFFER_SIZE];

// callback definition for MQTT
void callback(char* topic,
              uint8_t* payload,
              unsigned int payload_length)
{
  // nothing to do here as only publishing
}

PubSubClient   mqtt_client(mqtt_server_addr, MQTT_PORT, callback, wifly_client);


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


void publish_connected()
{
  prog_buffer[0] = '\0';
  strcpy_P(prog_buffer, (char*)pgm_read_word(&(STATUS_TOPICS[0])));
  mqtt_client.publish(prog_buffer, "");
}

void publish_uptime()
{
  prog_buffer[0] = '\0';
  strcpy_P(prog_buffer, (char*)pgm_read_word(&(STATUS_TOPICS[1])));
  char_buffer[0] = '\0';
  ltoa(millis(), char_buffer, 10);
  mqtt_client.publish(prog_buffer, char_buffer);
}

void publish_memory()
{
  prog_buffer[0] = '\0';
  strcpy_P(prog_buffer, (char*)pgm_read_word(&(STATUS_TOPICS[2])));
  char_buffer[0] = '\0';
  itoa(freeMemory(), char_buffer, 10);
  mqtt_client.publish(prog_buffer, char_buffer);
}

#if ENABLE_THEAIRBOARD_SUPPORT
void publish_battery()
{
  buf[0] = '\0';
  dtostrf(board.batteryChk(), 1, FLOAT_DECIMAL_PLACES, buf);
  prog_buffer[0] = '\0';
  strcpy_P(prog_buffer, (char*)pgm_read_word(&(STATUS_TOPICS[3])));
  mqtt_client.publish(prog_buffer, buf);
}
#endif

void publish_status()
{
  publish_connected();
  publish_uptime();
  publish_memory();
#if ENABLE_THEAIRBOARD_SUPPORT
  publish_battery();
#endif
}


byte mqtt_connect()
{
  if (!wifly_connected)
    wifly_connect();

  if (wifly_connected) {
    DEBUG_LOG(1, "connecting to broker");
    if (mqtt_client.connect(mqtt_client_id)) {
      DEBUG_LOG(1, "  connected");
      publish_status();
      return true;
    } else {
      DEBUG_LOG(1, "  failed");
      delay(AFTER_ERROR_DELAY);
    }
  }
  return false;
}

#endif  /* THEAIRBOARDWIFLYMQTTPUBLISHER_MQTT_CONFIG_H_ */

