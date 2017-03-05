#ifndef THEAIRBOARDWIFLYMQTTPUBLISHER_MQTT_CONFIG_H_
#define THEAIRBOARDWIFLYMQTTPUBLISHER_MQTT_CONFIG_H_

#include <PubSubClient.h>


// MQTT parameters
IPAddress mqttServerAddr(192, 168, 1, 50); // emonPi
char mqttClientId[] = "theairboard";
const int MQTT_PORT = 1883;

const byte BUFFER_SIZE            = 32;
char topicBuffer[BUFFER_SIZE];
char payloadBuffer[BUFFER_SIZE];
//char message[BUFFER_SIZE];

// callback definition for MQTT
void callback(char* topic,
              uint8_t* payload,
              unsigned int length)
{
  // nothing to do here as only publishing
}

PubSubClient mqttClient(mqttServerAddr, MQTT_PORT, callback, wiflyClient);


// MQTT topic definitions

const char MQTT_PAYLOAD_CONNECTED[] PROGMEM = "CONNECTED";
const char MQTT_PAYLOAD_OK[] PROGMEM = "OK";
const char MQTT_PAYLOAD_ERROR[] PROGMEM = "ERROR";
const char MQTT_PAYLOAD_START[] PROGMEM = "START";
const char MQTT_PAYLOAD_END[] PROGMEM = "END";
const char MQTT_PAYLOAD_SLEEP[] PROGMEM = "SLEEP";

PGM_P const MQTT_PAYLOADS[] PROGMEM = {
    MQTT_PAYLOAD_CONNECTED, // idx = 0
    MQTT_PAYLOAD_OK,        // idx = 1
    MQTT_PAYLOAD_ERROR,     // idx = 2
    MQTT_PAYLOAD_START,     // idx = 3
    MQTT_PAYLOAD_END,       // idx = 4
    MQTT_PAYLOAD_SLEEP,     // idx = 5
};

/* MQTT_PAYLOADS indices, must match table above */
typedef enum {
  MQTT_PAYLOAD_CONNECTED_IDX = 0,
  MQTT_PAYLOAD_OK_IDX = 1,
  MQTT_PAYLOAD_ERROR_IDX = 2,
  MQTT_PAYLOAD_START_IDX = 3,
  MQTT_PAYLOAD_END_IDX = 4,
  MQTT_PAYLOAD_SLEEP_IDX = 5,
} mqtt_payloads;

// status topics
const char MQTT_STATUS[] PROGMEM = "theairboard/status/mqtt";
const char VERSION_STATUS[] PROGMEM = "theairboard/status/version";
const char INTERVAL_STATUS[] PROGMEM = "theairboard/status/interval";
const char IP_ADDR_STATUS[] PROGMEM = "theairboard/status/ip_addr";
const char UPTIME_STATUS[] PROGMEM = "theairboard/status/uptime";
const char MEMORY_STATUS[] PROGMEM = "theairboard/status/memory";
const char BATTERY_STATUS[] PROGMEM = "theairboard/status/battery";
const char REPORT_STATUS[] PROGMEM = "theairboard/status/report";
const char DHT22_STATUS[] PROGMEM = "theairboard/status/dht22";

PGM_P const STATUS_TOPICS[] PROGMEM = {
    MQTT_STATUS,          // idx = 0
    VERSION_STATUS,        // idx = 1
    INTERVAL_STATUS,         // idx = 2
    IP_ADDR_STATUS,         // idx = 3
    UPTIME_STATUS,         // idx = 4
    MEMORY_STATUS, // idx = 5
    BATTERY_STATUS,          // idx = 6
    REPORT_STATUS,        // idx = 7
    DHT22_STATUS,       // idx = 8
};

typedef enum {
  MQTT_STATUS_IDX = 0,
  VERSION_STATUS_IDX = 1,
  INTERVAL_STATUS_IDX = 2,
  IP_ADDR_STATUS_IDX = 3,
  UPTIME_STATUS_IDX = 4,
  MEMORY_STATUS_IDX = 5,
  BATTERY_STATUS_IDX = 6,
  REPORT_STATUS_IDX = 7,
  DHT22_STATUS_IDX = 8,
} status_topics;

// measurement topics
const char DHT22_TEMP []              PROGMEM = "theairboard/measurement/DHT22_T";
const char DHT22_HUMIDITY[]           PROGMEM = "theairboard/measurement/DHT22_H";

//tables to refer to strings
PGM_P const MEASUREMENT_TOPICS[]      PROGMEM = { DHT22_TEMP,             // idx = 0
                                                  DHT22_HUMIDITY,         // idx = 1
                                                };

typedef enum {
  DHT22_TEMP_IDX = 0,
  DHT22_HUMIDITY_IDX = 1,
} measurement_topics_idx;


void publish_connected() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer,
           (char *)pgm_read_word(&(STATUS_TOPICS[MQTT_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  strcpy_P(payloadBuffer,
           (char *)pgm_read_word(&(MQTT_PAYLOADS[MQTT_PAYLOAD_CONNECTED_IDX])));
  mqttClient.publish(topicBuffer, payloadBuffer);
}

#if ENABLE_VERSION
void publish_version() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer,
           (char *)pgm_read_word(&(STATUS_TOPICS[VERSION_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  sprintf(payloadBuffer, "%s", VERSION);
  mqttClient.publish(topicBuffer, payloadBuffer);
}
#endif

#if 0
void publish_status_interval() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer,
           (char *)pgm_read_word(&(STATUS_TOPICS[INTERVAL_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  mqttClient.publish(topicBuffer,
                     ltoa(STATUS_UPDATE_INTERVAL, payloadBuffer, 10));
}
#endif

#if 0
void publish_ip_address() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer,
           (char *)pgm_read_word(&(STATUS_TOPICS[IP_ADDR_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  IPAddress ip = WiFly.ip();
  sprintf(payloadBuffer, "%i%c%i%c%i%c%i", ip[0], '.', ip[1], '.', ip[2], '.',
          ip[3]);
  mqttClient.publish(topicBuffer, payloadBuffer);
}
#endif

void publish_uptime() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer,
           (char *)pgm_read_word(&(STATUS_TOPICS[UPTIME_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  mqttClient.publish(topicBuffer, ltoa(millis(), payloadBuffer, 10));
}

# if USE_MEMORY_FREE
void publish_memory() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer,
           (char *)pgm_read_word(&(STATUS_TOPICS[MEMORY_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  mqttClient.publish(topicBuffer, itoa(getFreeMemory(), payloadBuffer, 10));
}
#endif

void publish_configuration() {
#if ENABLE_VERSION
  publish_version();
#endif
#if 0
  publish_status_interval();
  publish_ip_address();
#endif
}

#if ENABLE_THEAIRBOARD_SUPPORT
void publish_battery()
{
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[BATTERY_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  dtostrf(board.batteryChk(), 1, FLOAT_DECIMAL_PLACES, payloadBuffer);
  mqttClient.publish(topicBuffer, payloadBuffer);
}
#endif

void publish_status()
{
  publish_connected();
  publish_uptime();
#if USE_MEMORY_FREE
  publish_memory();
#endif
#if ENABLE_THEAIRBOARD_SUPPORT
  publish_battery();
#endif
}


byte mqtt_connect()
{
  if (!wiflyConnectedToNetwork)
    wifly_connect();

  if (wiflyConnectedToNetwork) {
    DEBUG_LOG(1, "connecting to broker");
    if (mqttClient.connect(mqttClientId)) {
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
