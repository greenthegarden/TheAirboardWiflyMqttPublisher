#include "config.h"

// MQTT related routines

#include <PubSubClient.h>

// callback definition for MQTT
void callback(char* topic,
              uint8_t* payload,
              unsigned int payload_length)
{
  // nothing to do here as only publishing
}

PubSubClient   mqtt_client(mqtt_server_addr, MQTT_PORT, callback, wifly_client);

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
      return false;
    }
    return false;
  }
  return false;
}

#if ENABLE_SENSOR_DHT22
#include "dht22_config.h"
#endif

void publish_measurements()
{
  DEBUG_LOG(1, "publishing measurements");
  if (mqtt_connect()) {
    // publish measurement start topic
    prog_buffer[0] = '\0';
    strcpy_P(prog_buffer, (char*)pgm_read_word(&(MEASUREMENT_TOPICS[0])));
    mqtt_client.publish(prog_buffer, "");

#if ENABLE_SENSOR_DHT22
    publish_dht22_measurement();
#endif

    // publish measurement end topic with message
    // message is number of measurements included

    //  buf[0] = '\0';
    //  itoa(measurement_count, buf, 10);

    // publish measurement end topic
    prog_buffer[0] = '\0';
    strcpy_P(prog_buffer, (char*)pgm_read_word(&(MEASUREMENT_TOPICS[1])));
    mqtt_client.publish(prog_buffer, "");

    mqtt_client.disconnect();
  }
}


/*--------------------------------------------------------------------------------------
  setup()
  Called by the Arduino framework once, before the main loop begins
  --------------------------------------------------------------------------------------*/
void setup()
{
  Serial.begin(BAUD_RATE);
  wifly_configure();
  publish_measurements();
}


/*--------------------------------------------------------------------------------------
  loop()
  Arduino main loop
  --------------------------------------------------------------------------------------*/
void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMeasurementMillis >= MEASUREMENT_INTERVAL) {
    previousMeasurementMillis = currentMillis;
    publish_measurements();
  }
}

