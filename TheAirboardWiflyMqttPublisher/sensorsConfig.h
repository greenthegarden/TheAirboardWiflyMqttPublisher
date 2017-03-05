#ifndef THEAIRBOARDWIFLYMQTTPUBLISHER_SENSORS_CONFIG_H_
#define THEAIRBOARDWIFLYMQTTPUBLISHER_SENSORS_CONFIG_H_


#if ENABLE_SENSOR_DHT22
#include <DHT22Config.h>

const byte DHT22_PIN = 2;

void publish_temperature_measurement() {
  DEBUG_LOG(3, "DHT22 temperature measurement: ");
  // value is stored in DHT object
  DEBUG_LOG(3, dht.temperature);
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(MEASUREMENT_TOPICS[DHT22_TEMP_IDX])));
  payloadBuffer[0] = '\0';
  dtostrf(dht.temperature, 1, FLOAT_DECIMAL_PLACES, payloadBuffer);
  mqttClient.publish(topicBuffer, payloadBuffer);
}

void publish_humidity_measurement() {
  DEBUG_LOG(3, "DHT22 humidity measurement: ");
  // value is stored in DHT object
  DEBUG_LOG(3, dht.humidity);
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(MEASUREMENT_TOPICS[DHT22_HUMIDITY_IDX])));
  payloadBuffer[0] = '\0';
  dtostrf(dht.humidity, 1, FLOAT_DECIMAL_PLACES, payloadBuffer);
  mqttClient.publish(topicBuffer, payloadBuffer);
}

void publish_dht22_measurement()
{
  // READ DATA
  int chk = dht.read22(DHT22_PIN);

  DEBUG_LOG(3, "DHT22 read state");
  DEBUG_LOG(3, "  value: ");
  DEBUG_LOG(3, chk);
  DEBUG_LOG(3, "  string: ");

  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[DHT22_STATUS_IDX])));

  switch (chk) {
    case DHTLIB_OK :
      DEBUG_LOG(3, "OK");
      payloadBuffer[0] = '\0';
      strcpy_P(payloadBuffer, (char*)pgm_read_word(&(DHT22_STATUS_MESSAGES[DHT22_STATUS_OK_IDX])));
      mqttClient.publish(topicBuffer, payloadBuffer);
      publish_temperature_measurement();
      publish_humidity_measurement();
      break;
    case DHTLIB_ERROR_CHECKSUM :
      DEBUG_LOG(3, "Checksum error");
      payloadBuffer[0] = '\0';
      strcpy_P(payloadBuffer, (char*)pgm_read_word(&(DHT22_STATUS_MESSAGES[DHT22_CHECKSUM_ERROR_IDX])));
      mqttClient.publish(topicBuffer, payloadBuffer);
      break;
    case DHTLIB_ERROR_TIMEOUT :
      DEBUG_LOG(3, "Time out error,\t");
      payloadBuffer[0] = '\0';
      strcpy_P(payloadBuffer, (char*)pgm_read_word(&(DHT22_STATUS_MESSAGES[DHT22_TIMEOUT_ERROR_IDX])));
      mqttClient.publish(topicBuffer, payloadBuffer);
      break;
    case DHTLIB_ERROR_CONNECT :
      DEBUG_LOG(3, "Connect error");
      payloadBuffer[0] = '\0';
      strcpy_P(payloadBuffer, (char*)pgm_read_word(&(DHT22_STATUS_MESSAGES[DHT22_CONNECT_ERROR_IDX])));
      mqttClient.publish(topicBuffer, payloadBuffer);
      break;
    case DHTLIB_ERROR_ACK_L :
      DEBUG_LOG(3, "Ack Low error");
      payloadBuffer[0] = '\0';
      strcpy_P(payloadBuffer, (char*)pgm_read_word(&(DHT22_STATUS_MESSAGES[DHT22_ACK_LOW_ERROR_IDX])));
      mqttClient.publish(topicBuffer, payloadBuffer);
      break;
    case DHTLIB_ERROR_ACK_H :
      DEBUG_LOG(3, "Ack High error");
      payloadBuffer[0] = '\0';
      strcpy_P(payloadBuffer, (char*)pgm_read_word(&(DHT22_STATUS_MESSAGES[DHT22_ACK_HIGH_ERROR_IDX])));
      mqttClient.publish(topicBuffer, payloadBuffer);
      break;
    default :
      DEBUG_LOG(3, "Unknown error");
      payloadBuffer[0] = '\0';
      strcpy_P(payloadBuffer, (char*)pgm_read_word(&(DHT22_STATUS_MESSAGES[DHT22_UNKNOWN_ERROR_IDX])));
      mqttClient.publish(topicBuffer, payloadBuffer);
      break;
  }
}
#endif

byte publish_measurements()
{
  DEBUG_LOG(1, "publishing measurements");
  if (mqtt_connect()) {
    // publish report start topic
    topicBuffer[0] = '\0';
    strcpy_P(topicBuffer, (char *)pgm_read_word(&(STATUS_TOPICS[REPORT_STATUS_IDX])));
    payloadBuffer[0] = '\0';
    strcpy_P(payloadBuffer, (char *)pgm_read_word(&(MQTT_PAYLOADS[MQTT_PAYLOAD_START_IDX])));
    mqttClient.publish(topicBuffer, payloadBuffer);

#if ENABLE_SENSOR_DHT22
    publish_dht22_measurement();
#endif

    // publish report end topic
    topicBuffer[0] = '\0';
    strcpy_P(topicBuffer, (char *)pgm_read_word(&(STATUS_TOPICS[REPORT_STATUS_IDX])));
    payloadBuffer[0] = '\0';
    strcpy_P(payloadBuffer, (char *)pgm_read_word(&(MQTT_PAYLOADS[MQTT_PAYLOAD_END_IDX])));
    mqttClient.publish(topicBuffer, payloadBuffer);

    mqttClient.disconnect();

    return 1;
  }
  return 0;
}


#endif  /* THEAIRBOARDWIFLYMQTTPUBLISHER_SENSORS_CONFIG_H_ */
