#ifndef THEAIRBOARDWIFLYMQTTPUBLISHER_CONFIG_H_
#define THEAIRBOARDWIFLYMQTTPUBLISHER_CONFIG_H_


#define DEBUG                       true

#if DEBUG
#include "Debug.h"
#endif


// Macros
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))


const int AFTER_ERROR_DELAY       = 1000;


// Serial parameters
const int BAUD_RATE               = 9600;


// Network configuration
#include "networkConfig.h"


// WiFly configuration
#include "wifly_config.h"


// MQTT configuration
#include "mqtt_config.h"


// program constants

const byte FLOAT_DECIMAL_PLACES   = 1;
const unsigned long MEASUREMENT_INTERVAL = 300000UL;    // 5 minutes = 5 * 60 * 1000 miliiseconds


#endif  /* THEAIRBOARDWIFLYMQTTPUBLISHER_CONFIG_H_ */

