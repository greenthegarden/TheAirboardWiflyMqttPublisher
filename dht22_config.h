#ifndef THEAIRBOARDWIFLYMQTTPUBLISHER_DHT22_CONFIG_H_
#define THEAIRBOARDWIFLYMQTTPUBLISHER_DHT22_CONFIG_H_


#include <dht.h>                // DHT22 temperature/humidty sensor library

dht DHT;

const int DHT22_PIN = 10;

// DHT22 status messages
const char status_ok[]                PROGMEM = "OK";
const char checksum_error[]           PROGMEM = "Checksum error";
const char timeout_error[]            PROGMEM = "Time out error";
const char connect_error[]            PROGMEM = "Connect error";
const char ack_low_error[]            PROGMEM = "Ack Low error";
const char ack_high_error[]           PROGMEM = "Ack High error";
const char unknown_error[]            PROGMEM = "Unknown error";

PGM_P const dht22_status_messages[]   PROGMEM = { status_ok,        // idx = 0
                                                  checksum_error,   // idx = 1
                                                  timeout_error,    // idx = 2
                                                  connect_error,    // idx = 3
                                                  ack_low_error,    // idx = 4
                                                  ack_high_error,   // idx = 5
                                                  unknown_error,    // idx = 6
                                                };

void publish_temperature_measurement()
{
  DEBUG_LOG(3, "DHT22 temperature measurement: ");
  // value is stored in DHT object
  DEBUG_LOG(3, DHT.temperature);
  buf[0] = '\0';
  dtostrf(DHT.temperature, 1, FLOAT_DECIMAL_PLACES, buf);
  prog_buffer[0] = '\0';
  strcpy_P(prog_buffer, (char*)pgm_read_word(&(MEASUREMENT_TOPICS[2])));
  mqtt_client.publish(prog_buffer, buf);
}

void publish_humidity_measurement()
{
  DEBUG_LOG(3, "DHT22 humidity measurement: ");
  // value is stored in DHT object
  DEBUG_LOG(3, DHT.humidity);
  buf[0] = '\0';
  dtostrf(DHT.humidity, 1, FLOAT_DECIMAL_PLACES, buf);
  prog_buffer[0] = '\0';
  strcpy_P(prog_buffer, (char*)pgm_read_word(&(MEASUREMENT_TOPICS[3])));
  mqtt_client.publish(prog_buffer, buf);
}

void publish_dht22_measurement()
{
  // READ DATA
  int chk = DHT.read22(DHT22_PIN);

  DEBUG_LOG(3, "DHT22 read state");
  DEBUG_LOG(3, "  value: ");
  DEBUG_LOG(3, chk);
  DEBUG_LOG(3, "  string: ");

  prog_buffer[0] = '\0';
  strcpy_P(prog_buffer, (char*)pgm_read_word(&(STATUS_TOPICS[4])));

  switch (chk) {
    case DHTLIB_OK :
      DEBUG_LOG(3, "OK");
      char_buffer[0] = '\0';
      strcpy_P(char_buffer, (char*)pgm_read_word(&(dht22_status_messages[0])));
      mqtt_client.publish(prog_buffer, char_buffer);
      publish_temperature_measurement();
      publish_humidity_measurement();
      break;
    case DHTLIB_ERROR_CHECKSUM :
      DEBUG_LOG(3, "Checksum error");
      char_buffer[0] = '\0';
      strcpy_P(char_buffer, (char*)pgm_read_word(&(dht22_status_messages[1])));
      mqtt_client.publish(prog_buffer, char_buffer);
      break;
    case DHTLIB_ERROR_TIMEOUT :
      DEBUG_LOG(3, "Time out error,\t");
      char_buffer[0] = '\0';
      strcpy_P(char_buffer, (char*)pgm_read_word(&(dht22_status_messages[2])));
      mqtt_client.publish(prog_buffer, char_buffer);
      break;
    case DHTLIB_ERROR_CONNECT :
      DEBUG_LOG(3, "Connect error");
      char_buffer[0] = '\0';
      strcpy_P(char_buffer, (char*)pgm_read_word(&(dht22_status_messages[3])));
      mqtt_client.publish(prog_buffer, char_buffer);
      break;
    case DHTLIB_ERROR_ACK_L :
      DEBUG_LOG(3, "Ack Low error");
      char_buffer[0] = '\0';
      strcpy_P(char_buffer, (char*)pgm_read_word(&(dht22_status_messages[4])));
      mqtt_client.publish(prog_buffer, char_buffer);
      break;
    case DHTLIB_ERROR_ACK_H :
      DEBUG_LOG(3, "Ack High error");
      char_buffer[0] = '\0';
      strcpy_P(char_buffer, (char*)pgm_read_word(&(dht22_status_messages[5])));
      mqtt_client.publish(prog_buffer, char_buffer);
      break;
    default :
      DEBUG_LOG(3, "Unknown error");
      char_buffer[0] = '\0';
      strcpy_P(char_buffer, (char*)pgm_read_word(&(dht22_status_messages[6])));
      mqtt_client.publish(prog_buffer, char_buffer);
      break;
  }
}


#endif  /* THEAIRBOARDWIFLYMQTTPUBLISHER_DHT22_CONFIG_H_ */

