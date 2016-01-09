#include "config.h"



// character buffer to support conversion of floats to char
char buf[12];

// global variable definitions
unsigned long previousMeasurementMillis = 0;

// function declarations
void takeMeasurement();


// WiFly setup and connection routines

//void wifly_configure()
//{
//  // Configure WiFly
//  wifly_serial.begin(BAUD_RATE);
//  WiFly.setUart(&wifly_serial);
//}

void wifly_connect()
{
  DEBUG_LOG(1, "initialising wifly");

  WiFly.begin();
  delay(5000);  // allow time to WiFly to initialise

  DEBUG_LOG(1, "joining network");

  //  if (!WiFly.join(MY_SSID, MY_PASSPHRASE, mode)) {
  if (!WiFly.join(MY_SSID)) {   // needs to be fixed to allow a passphrase if secure
    wifly_connected = false;
    DEBUG_LOG(1, "  failed");
    delay(AFTER_ERROR_DELAY);
  } else {
    wifly_connected = true;
    DEBUG_LOG(1, "  connected");
  }
}


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
  DEBUG_LOG(1, "Publishing measurements");
  if (mqtt_connect()) {
    // publish measurement start topic
    prog_buffer[0] = '\0';
    strcpy_P(prog_buffer, (char*)pgm_read_word(&(MEASUREMENT_TOPICS[0])));
    mqtt_client.publish(prog_buffer, "");

#if ENABLE_SENSOR_DHT22
    // take measurement as sensor cannot be be sampled at short intervals
    dht22_measurement();
//    if (dht22_measurement() == DHTLIB_OK) {
//      // value is stored in DHT object
//      dht22_measurement_ok = true;
//
//      publish_temperature_measurement();
//      publish_humidity_measurement();
//    }
#endif

    // publish measurement end topic with message
    // message is number of measurements included

    //  buf[0] = '\0';
    //  itoa(measurement_count, buf, 10);

    // publish measurement end topic
    prog_buffer[0] = '\0';
    strcpy_P(prog_buffer, (char*)pgm_read_word(&(MEASUREMENT_TOPICS[1])));
    mqtt_client.publish(prog_buffer, "");

#if ENABLE_SENSOR_DHT22
    // reset measurements
    //dht22_measurement_ok = false;
#endif
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
#if ENABLE_THEAIRBOARD_SUPPORT
  WiFly.setUart(&Serial);
#else
  wifly_configure();
#endif
  publish_measurements();
//  if (mqtt_connect()) {
//    mqtt_client.disconnect();
//  }
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

