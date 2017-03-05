#include <Arduino.h>

#include "config.h"


/*--------------------------------------------------------------------------------------
  setup()
  Called by the Arduino framework once, before the main loop begins
  --------------------------------------------------------------------------------------*/
void setup()
{
#if (ENABLE_THEAIRBOARD_SUPPORT && ENABLE_THEAIRBOARD_SLEEP)
  pinMode(RF, OUTPUT);
  digitalWrite(RF, 1);          // turn on wireless module
#endif
  Serial.begin(BAUD_RATE);
  wifly_configure();
  publish_status();
#if (ENABLE_THEAIRBOARD_SUPPORT && ENABLE_THEAIRBOARD_SLEEP)
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
#if (ENABLE_THEAIRBOARD_SUPPORT && ENABLE_THEAIRBOARD_SLEEP)
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
  unsigned long currentMillis = millis();

  if (currentMillis - previousMeasurementMillis >= PUBLISH_INTERVAL) {
    previousMeasurementMillis = currentMillis;
    publish_status();
#if ENABLE_SENSOR_DHT22
    publish_measurements();
#endif
  }
#endif
}
