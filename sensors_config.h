#ifndef THEAIRBOARDWIFLYMQTTPUBLISHER_SENSORS_CONFIG_H_
#define THEAIRBOARDWIFLYMQTTPUBLISHER_SENSORS_CONFIG_H_


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


#endif  /* THEAIRBOARDWIFLYMQTTPUBLISHER_SENSORS_CONFIG_H_ */


