#ifndef THEAIRBOARDWIFLYMQTTPUBLISHER_DHT22_CONFIG_H_
#define THEAIRBOARDWIFLYMQTTPUBLISHER_DHT22_CONFIG_H_


// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"

#include "config.h"

#define DHTPIN 12 // Digital pin connected to the DHT sensor

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

void publish_dht22_temperature()
{
  float reading = dht.readTemperature();
  if (!isnan(reading)){
    topicBuffer[0] = '\0';
    dtostrf(reading, 1, FLOAT_DECIMAL_PLACES, topicBuffer);
    payloadBuffer[0] = '\0';
    strcpy_P(payloadBuffer, (char *)pgm_read_word(&(MEASUREMENT_TOPICS[MEASUREMENT_DHT22_TEMPERATURE_IDX])));
    mqttClient.publish(topicBuffer, payloadBuffer);
  }
}

void publish_dht22_humidity()
{
  float reading = dht.readHumidity();
  if (!isnan(reading))
  {
    topicBuffer[0] = '\0';
    dtostrf(reading, 1, FLOAT_DECIMAL_PLACES, topicBuffer);
    payloadBuffer[0] = '\0';
    strcpy_P(payloadBuffer, (char *)pgm_read_word(&(MEASUREMENT_TOPICS[MEASUREMENT_DHT22_HUMIDITY_IDX])));
    mqttClient.publish(topicBuffer, payloadBuffer);
  }
}

#endif  /* THEAIRBOARDWIFLYMQTTPUBLISHER_DHT22_CONFIG_H_ */
