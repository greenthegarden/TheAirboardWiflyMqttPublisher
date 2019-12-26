#ifndef THEAIRBOARDWIFLYMQTTPUBLISHER_CONFIG_H_
#define THEAIRBOARDWIFLYMQTTPUBLISHER_CONFIG_H_


#ifndef ENABLE_THEAIRBOARD_SLEEP
#define ENABLE_THEAIRBOARD_SLEEP  false
#endif


// Global Constants
const int AFTER_ERROR_DELAY       = 1000;
const int BAUD_RATE               = 9600;
const byte FLOAT_DECIMAL_PLACES   = 1;
const unsigned long STATUS_UPDATE_INTERVAL = 1UL * 60UL * 1000UL;    // x minutes = x * 60 * 1000 milliseconds

// global variable definitions
unsigned long statusPreviousMillis = 0UL;

// Internal headers
#include "networkConfig.h"  // Network configuration
#include "wiflyConfig.h"   // WiFly configuration
#include "mqttConfig.h"    // MQTT configuration
#include "dht22Config.h"
#include "theairboardConfig.h"

#endif  /* THEAIRBOARDWIFLYMQTTPUBLISHER_CONFIG_H_ */
