#ifndef THEAIRBOARDWIFLYMQTTPUBLISHER_CONFIG_H_
#define THEAIRBOARDWIFLYMQTTPUBLISHER_CONFIG_H_


#ifndef ENABLE_THEAIRBOARD_SUPPORT
#define ENABLE_THEAIRBOARD_SUPPORT  true
#endif
#ifndef ENABLE_THEAIRBOARD_SLEEP
#define ENABLE_THEAIRBOARD_SLEEP  false
#endif


#include "debug.h"


// Global Constants
const int AFTER_ERROR_DELAY       = 1000;
const int BAUD_RATE               = 9600;
const byte FLOAT_DECIMAL_PLACES   = 1;
const unsigned long PUBLISH_INTERVAL = 1UL * 60UL * 1000UL;    // x minutes = x * 60 * 1000 milliseconds

// global variable definitions
unsigned long previousMeasurementMillis = 0;

// External libraries
#if ENABLE_THEAIRBOARD_SUPPORT
#include <TheAirBoard.h>
TheAirBoard board;
#if ENABLE_THEAIRBOARD_SLEEP
volatile boolean f_wdt = true;
unsigned int timeout = 0;

// interrupt service routine: minimize time spent here
ISR(WDT_vect) {
  if(f_wdt == false)
    f_wdt = true;
}
#endif

#endif
#include <MemoryFree.h>


// Internal headers
#include "networkConfig.h"  // Network configuration
#include "wiflyConfig.h"   // WiFly configuration
#include "mqttConfig.h"    // MQTT configuration


// Define sensors
#ifndef ENABLE_SENSOR_DHT22
#define ENABLE_SENSOR_DHT22 false
#endif
#include "sensorsConfig.h"


#endif  /* THEAIRBOARDWIFLYMQTTPUBLISHER_CONFIG_H_ */
