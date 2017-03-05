#ifndef THEAIRBOARDWIFLYMQTTPUBLISHER_NETWORK_CONFIG_H_
#define THEAIRBOARDWIFLYMQTTPUBLISHER_NETWORK_CONFIG_H_

#ifndef USE_SECURE_NETWORK
#define USE_SECURE_NETWORK      true
#endif

// Wifi parameters
const char SSID[]             = "";
#if USE_SECURE_NETWORK
const char PASSPHRASE[]       = "";
boolean    mode               = true; //or WEP_MODE
#endif


#endif  /* THEAIRBOARDWIFLYMQTTPUBLISHER_NETWORK_CONFIG_H_ */
