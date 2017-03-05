#ifndef THEAIRBOARDWIFLYMQTTPUBLISHER_CONFIG_H_
#define THEAIRBOARDWIFLYMQTTPUBLISHER_CONFIG_H_


#ifndef ENABLE_THEAIRBOARD_SUPPORT
#define ENABLE_THEAIRBOARD_SUPPORT  true
#endif

#include "debug.h"


// GLobal Constants
const int AFTER_ERROR_DELAY       = 1000;
const int BAUD_RATE               = 9600;
const byte FLOAT_DECIMAL_PLACES   = 1;
const unsigned long MEASUREMENT_INTERVAL = 300000UL;    // 5 minutes = 5 * 60 * 1000 miliiseconds

// global variable definitions
#if !(ENABLE_THEAIRBOARD_SUPPORT)
unsigned long previousMeasurementMillis = 0;
#endif

// External libraries
#if ENABLE_THEAIRBOARD_SUPPORT
#include <TheAirBoard.h>
TheAirBoard board;
volatile boolean f_wdt = true;
unsigned int timeout = 0;

// interrupt service routine: minimize time spent here
ISR(WDT_vect) {
  if(f_wdt == false)
    f_wdt = true;
}

#endif
#include <MemoryFree.h>


// Internal headers
#include "networkConfig.h"  // Network configuration
#include "wiflyConfig.h"   // WiFly configuration
#include "mqttConfig.h"    // MQTT configuration


// Define sensors
#ifndef ENABLE_SENSOR_DHT22
#define ENABLE_SENSOR_DHT22 true
#endif
#include "sensorsConfig.h"


#endif  /* THEAIRBOARDWIFLYMQTTPUBLISHER_CONFIG_H_ */
