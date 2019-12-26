#ifndef THEAIRBOARDWIFLYMQTTPUBLISHER_DHT22_CONFIG_H_
#define THEAIRBOARDWIFLYMQTTPUBLISHER_DHT22_CONFIG_H_

// DHT22 temperature/humidity sensor library
#include <dht.h>
#include <dht22StatusConfig.h>

const int DHT22_PIN = 12;

dht dht22;

byte dht22_measurement()
{
  byte chk = dht22.read22(DHT22_PIN);

  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer,
           (char *)pgm_read_word(&(MEASUREMENT_TOPICS[MEASUREMENT_DHT22_STATUS_IDX])));

  switch (chk)
  {
  case DHTLIB_OK:
    // DEBUG_LOG(1, "OK");
    payloadBuffer[0] = '\0';
    strcpy_P(payloadBuffer, (char *)pgm_read_word(
                                &(DHT22_STATUS_MESSAGES[DHT22_STATUS_OK_IDX])));
    mqttClient.publish(topicBuffer, payloadBuffer);
    break;
  case DHTLIB_ERROR_CHECKSUM:
    // DEBUG_LOG(1, "Checksum error");
    payloadBuffer[0] = '\0';
    strcpy_P(payloadBuffer, (char *)pgm_read_word(&(
                             DHT22_STATUS_MESSAGES[DHT22_CHECKSUM_ERROR_IDX])));
    mqttClient.publish(topicBuffer, payloadBuffer);
    break;
  case DHTLIB_ERROR_TIMEOUT:
    // DEBUG_LOG(1, "Time out error");
    payloadBuffer[0] = '\0';
    strcpy_P(payloadBuffer, (char *)pgm_read_word(&(
                             DHT22_STATUS_MESSAGES[DHT22_TIMEOUT_ERROR_IDX])));
    mqttClient.publish(topicBuffer, payloadBuffer);
    break;
  case DHTLIB_ERROR_CONNECT:
    // DEBUG_LOG(1, "Connect error");
    payloadBuffer[0] = '\0';
    strcpy_P(payloadBuffer, (char *)pgm_read_word(&(
                             DHT22_STATUS_MESSAGES[DHT22_CONNECT_ERROR_IDX])));
    mqttClient.publish(topicBuffer, payloadBuffer);
    break;
  case DHTLIB_ERROR_ACK_L:
    // DEBUG_LOG(1, "Ack Low error");
    payloadBuffer[0] = '\0';
    strcpy_P(payloadBuffer, (char *)pgm_read_word(&(
                             DHT22_STATUS_MESSAGES[DHT22_ACK_LOW_ERROR_IDX])));
    mqttClient.publish(topicBuffer, payloadBuffer);
    break;
  case DHTLIB_ERROR_ACK_H:
    // DEBUG_LOG(1, "Ack High error");
    payloadBuffer[0] = '\0';
    strcpy_P(payloadBuffer, (char *)pgm_read_word(&(
                             DHT22_STATUS_MESSAGES[DHT22_ACK_HIGH_ERROR_IDX])));
    mqttClient.publish(topicBuffer, payloadBuffer);
    break;
  default:
    // DEBUG_LOG(1, "Unknown error");
    payloadBuffer[0] = '\0';
    strcpy_P(payloadBuffer, (char *)pgm_read_word(&(
                             DHT22_STATUS_MESSAGES[DHT22_UNKNOWN_ERROR_IDX])));
    mqttClient.publish(topicBuffer, payloadBuffer);
    break;
  }
  return chk;
}

void publish_dht22_temperature_measurement()
{
  // DEBUG_LOG(3, "DHT22 temperature measurement: ");
  // value is stored in DHT object
  // DEBUG_LOG(3, dht.temperature);
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char *)pgm_read_word(&(MEASUREMENT_TOPICS[MEASUREMENT_DHT22_TEMPERATURE_IDX])));
  payloadBuffer[0] = '\0';
  dtostrf(dht22.temperature, 1, FLOAT_DECIMAL_PLACES, payloadBuffer);
  mqttClient.publish(topicBuffer, payloadBuffer);
}

void publish_dht22_humidity_measurement()
{
  // DEBUG_LOG(3, "DHT22 humidity measurement: ");
  // value is stored in DHT object
  // DEBUG_LOG(3, dht.humidity);
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char *)pgm_read_word(&(MEASUREMENT_TOPICS[MEASUREMENT_DHT22_HUMIDITY_IDX])));
  payloadBuffer[0] = '\0';
  dtostrf(dht22.humidity, 1, FLOAT_DECIMAL_PLACES, payloadBuffer);
  mqttClient.publish(topicBuffer, payloadBuffer);
}

void publish_dht22_measurements()
{
  if (dht22_measurement() == DHTLIB_OK)
  {
    publish_dht22_temperature_measurement();
    publish_dht22_humidity_measurement();
  }
}


#endif  /* THEAIRBOARDWIFLYMQTTPUBLISHER_DHT22_CONFIG_H_ */
