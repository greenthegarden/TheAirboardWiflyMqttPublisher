#include <TheAirBoard.h>

TheAirBoard board;


// WiFly libraries
#include <SPI.h>
#include <WiFly.h>

#include "config.h"

#include <MemoryFree.h>


// character buffer to support conversion of floats to char
char buf[12];

// global variable definitions
unsigned long previousMeasurementMillis = 0;

// function declarations
void takeMeasurement();


// WiFly setup and connection routines

void wifly_configure()
{
  // Configure WiFly
  wifly_serial.begin(BAUD_RATE);
  WiFly.setUart(&wifly_serial);
}

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



void mqtt_connect()
{
  if (!wifly_connected)
    wifly_connect();

  if (wifly_connected) {
    DEBUG_LOG(1, "connecting to broker");
    if (mqtt_client.connect(mqtt_client_id)) {
      DEBUG_LOG(1, "  connected");
      publish_connected();
#if USE_FREEMEM
      publish_memory();
#endif
    } else {
      DEBUG_LOG(1, "  failed");
      delay(AFTER_ERROR_DELAY);
    }
  }
}

void reset_connection()
{
  if (mqtt_client.connected())
    mqtt_client.disconnect();
  wifly_connect();
  mqtt_connect();
}

void publish_connected()
{
  prog_buffer[0] = '\0';
  strcpy_P(prog_buffer, (char*)pgm_read_word(&(STATUS_TOPICS[0])));
  mqtt_client.publish(prog_buffer, "connected");
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

void publish_measurements()
{
  if (!wifly_connected)
    wifly_connect();

  if (wifly_connected) {
    // MQTT client setup
    //    mqttClient.disconnect();
    DEBUG_LOG(1, "connecting to broker");
    if (mqtt_client.connect(mqtt_client_id)) {
      DEBUG_LOG(1, "  connected");
      prog_buffer[0] = '\0';
      strcpy_P(prog_buffer, (char*)pgm_read_word(&(STATUS_TOPICS[0])));
      char_buffer[0] = '\0';
      strcpy_P(char_buffer, (char*)pgm_read_word(&(MQTT_STATUS_MESSAGES[0])));
      mqtt_client.publish(prog_buffer, char_buffer);

      takeMeasurement();

      mqtt_client.disconnect();
    }
  }
}


// sensor support

#define SENSOR_DHT22 false

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
#endif

void takeMeasurement(void)
{
  // publish measurement start topic
  prog_buffer[0] = '\0';
  strcpy_P(prog_buffer, (char*)pgm_read_word(&(MEASUREMENT_TOPICS[0])));
  mqtt_client.publish(prog_buffer, "");

  theairboard_battery_measurement();

#if SENSOR_DHT22
  // take measurement as sensor cannot be be sampled at short intervals
  if (dht22_measurement() == DHTLIB_OK) {
    // value is stored in DHT object
    dht22_measurement_ok = true;
  }

  temperature_measurement();
  humidity_measurement();
#endif

  // publish measurement end topic with message
  // message is number of measurements included

  //  buf[0] = '\0';
  //  itoa(measurement_count, buf, 10);

  // publish measurement end topic
  prog_buffer[0] = '\0';
  strcpy_P(prog_buffer, (char*)pgm_read_word(&(MEASUREMENT_TOPICS[1])));
  mqtt_client.publish(prog_buffer, "");

#if SENSOR_DHT22
  // reset measurements
  dht22_measurement_ok = false;
#endif
}

void theairboard_battery_measurement()
{
  buf[0] = '\0';
  dtostrf(board.batteryChk(), 1, FLOAT_DECIMAL_PLACES, buf);
  prog_buffer[0] = '\0';
  strcpy_P(prog_buffer, (char*)pgm_read_word(&(MEASUREMENT_TOPICS[2])));
  mqtt_client.publish(prog_buffer, buf);
}

#if SENSOR_DHT22
void temperature_measurement()
{
  if (dht22_measurement_ok) {
    // value is stored in DHT object
    buf[0] = '\0';
    dtostrf(DHT.temperature, 1, FLOAT_DECIMAL_PLACES, buf);
    prog_buffer[0] = '\0';
    strcpy_P(prog_buffer, (char*)pgm_read_word(&(MEASUREMENT_TOPICS[3])));
    mqtt_client.publish(prog_buffer, buf);
  }
}

void humidity_measurement()
{
  if (dht22_measurement_ok) {
    // value is stored in DHT object
    buf[0] = '\0';
    dtostrf(DHT.humidity, 1, FLOAT_DECIMAL_PLACES, buf);
    prog_buffer[0] = '\0';
    strcpy_P(prog_buffer, (char*)pgm_read_word(&(MEASUREMENT_TOPICS[4])));
    mqtt_client.publish(prog_buffer, buf);
  }
}
#endif


/*--------------------------------------------------------------------------------------
  setup()
  Called by the Arduino framework once, before the main loop begins
  --------------------------------------------------------------------------------------*/
void setup()
{
  Serial.begin(BAUD_RATE);
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

