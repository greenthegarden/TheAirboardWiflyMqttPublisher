#ifndef THEAIRBOARDWIFLYMQTTPUBLISHER_NETWORK_CONFIG_H_
#define THEAIRBOARDWIFLYMQTTPUBLISHER_NETWORK_CONFIG_H_


#define USE_SECURE_NETWORK      false

// Wifi parameters
const char MY_SSID[]          = "sensorNet";
#if USE_SECURE_NETWORK
const char MY_PASSPHRASE[]    = "";
boolean    mode               = true; //or WEP_MODE
#endif


#endif  /* THEAIRBOARDWIFLYMQTTPUBLISHER_NETWORK_CONFIG_H_ */

