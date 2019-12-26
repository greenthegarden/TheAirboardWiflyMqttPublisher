#include <Arduino.h>

#include "config.h"

/*--------------------------------------------------------------------------------------
  helper functions
  --------------------------------------------------------------------------------------*/
// void publish_theairboard_battery();
// void publish_theairboard_led_colour(byte colour_idx);
// void publish_theairboard_temperature();

void publish_configuration()
{
  publish_status_interval();
#if 0
  publish_ip_address();
#endif
}

void publish_status()
{
  publish_uptime();
  publish_theairboard_battery();
  publish_theairboard_temperature();
  publish_dht22_temperature_measurement();
  publish_dht22_humidity_measurement();
}


boolean mqtt_connect()
{
  if (!wiflyConnectedToNetwork)
    wifly_connect();

  if (wiflyConnectedToNetwork)
  {
    // if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
    if (mqttClient.connect(MQTT_CLIENT_ID))
    {
      publish_connected();
      publish_configuration();
      publish_status();
    }
    mqttClient.disconnect();
  }
  return mqttClient.connected();
}

/*--------------------------------------------------------------------------------------
  setup()
  Called by the Arduino framework once, before the main loop begins
  --------------------------------------------------------------------------------------*/
void setup()
{
  theairboard_init();
  wifly_configure();
  mqtt_connect();
#if ENABLE_THEAIRBOARD_SLEEP
  delay(5000);                  // allow time to launch programming, before a possible wireless module power down
  board.setWatchdog(8000);      // set watchdog timeout in milliseconds (max 8000)
#endif
}


/*--------------------------------------------------------------------------------------
  loop()
  Arduino main loop
  --------------------------------------------------------------------------------------*/
void loop()
{
  unsigned long now = millis();

#if ENABLE_THEAIRBOARD_SLEEP
  if(f_wdt == true) {            // on watchdog expire (every 8 seconds)
    timeout++;
    if(timeout == 10) {           // timeout every 10*8 = 80 seconds
      analogWrite(BLUE, 1);      // instruction can be removed to save even more power
      digitalWrite(RF, HIGH);    // turn on wireless module
      delay(50);                 // delay for WiFly power to stabilize
      publish_status();
      delay(1000);               // delay for data to transmit
      digitalWrite(RF, LOW);     // turn off wireless module
      analogWrite(BLUE, 0);      // reset communication indicator
      timeout = 0;
    }
    f_wdt = false;
    board.powerDown();           // goto deep sleep
  }
#else

  if (now - statusPreviousMillis >= STATUS_UPDATE_INTERVAL) {
    if (mqttClient.connected()) {
      statusPreviousMillis = now;
      mqtt_connect();
    }
  }

#endif
}
