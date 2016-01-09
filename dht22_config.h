#ifndef THEAIRBOARDWIFLYMQTTPUBLISHER_DHT22_CONFIG_H_
#define THEAIRBOARDWIFLYMQTTPUBLISHER_DHT22_CONFIG_H_


#if SENSOR_DHT22
#include <dht.h>                // DHT22 temperature/humidty sensor library
dht DHT;
const int DHT22_PIN = 10;
byte dht22_measurement_ok = false;

byte dht22_measurement()
{
  // READ DATA
  int chk = DHT.read22(DHT22_PIN);

  switch (chk) {
    case DHTLIB_OK:
      //  Serial.print("OK,\t");
      break;
    case DHTLIB_ERROR_CHECKSUM:
      //   Serial.print("Checksum error,\t");
      char_buffer[0] = '\0';
      strcpy_P(char_buffer, (char*)pgm_read_word(&(dht22_status_messages[1])));
      mqtt_client.publish(prog_buffer, char_buffer);
      break;
    case DHTLIB_ERROR_TIMEOUT:
      //    Serial.print("Time out error,\t");
      char_buffer[0] = '\0';
      strcpy_P(char_buffer, (char*)pgm_read_word(&(dht22_status_messages[2])));
      mqtt_client.publish(prog_buffer, char_buffer);
      break;
    case DHTLIB_ERROR_CONNECT:
      //    Serial.print("Connect error,\t");
      char_buffer[0] = '\0';
      strcpy_P(char_buffer, (char*)pgm_read_word(&(dht22_status_messages[3])));
      mqtt_client.publish(prog_buffer, char_buffer);
      break;
    case DHTLIB_ERROR_ACK_L:
      //    Serial.print("Ack Low error,\t");
      char_buffer[0] = '\0';
      strcpy_P(char_buffer, (char*)pgm_read_word(&(dht22_status_messages[4])));
      mqtt_client.publish(prog_buffer, char_buffer);
      break;
    case DHTLIB_ERROR_ACK_H:
      //    Serial.print("Ack High error,\t");
      char_buffer[0] = '\0';
      strcpy_P(char_buffer, (char*)pgm_read_word(&(dht22_status_messages[5])));
      mqtt_client.publish(prog_buffer, char_buffer);
      break;
    default:
      //    Serial.print("Unknown error,\t");
      char_buffer[0] = '\0';
      strcpy_P(char_buffer, (char*)pgm_read_word(&(dht22_status_messages[6])));
      mqtt_client.publish(prog_buffer, char_buffer);
      break;
  }

  return chk;
}

void publish_temperature_measurement()
{
  if (dht22_measurement_ok) {
    // value is stored in DHT object
    buf[0] = '\0';
    dtostrf(DHT.temperature, 1, FLOAT_DECIMAL_PLACES, buf);
    prog_buffer[0] = '\0';
    strcpy_P(prog_buffer, (char*)pgm_read_word(&(MEASUREMENT_TOPICS[2])));
    mqtt_client.publish(prog_buffer, buf);
  }
}

void publish_humidity_measurement()
{
  if (dht22_measurement_ok) {
    // value is stored in DHT object
    buf[0] = '\0';
    dtostrf(DHT.humidity, 1, FLOAT_DECIMAL_PLACES, buf);
    prog_buffer[0] = '\0';
    strcpy_P(prog_buffer, (char*)pgm_read_word(&(MEASUREMENT_TOPICS[3])));
    mqtt_client.publish(prog_buffer, buf);
  }
}
#endif


#endif  /* THEAIRBOARDWIFLYMQTTPUBLISHER_DHT22_CONFIG_H_ */

