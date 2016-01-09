#include "config.h"


/*--------------------------------------------------------------------------------------
  setup()
  Called by the Arduino framework once, before the main loop begins
  --------------------------------------------------------------------------------------*/
void setup()
{
  Serial.begin(BAUD_RATE);
  wifly_configure();
  publish_measurements();
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

