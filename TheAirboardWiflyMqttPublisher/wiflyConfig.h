#ifndef THEAIRBOARDWIFLYMQTTPUBLISHER_WIFLY_CONFIG_H_
#define THEAIRBOARDWIFLYMQTTPUBLISHER_WIFLY_CONFIG_H_


#include <SPI.h>
#include <WiFly.h>

#include "networkConfig.h"

boolean wiflyConnectedToNetwork = false;

WiFlyClient    wiflyClient;

void wifly_configure() {
  WiFly.setUart(&Serial);
}

byte wifly_connect() {
  WiFly.begin();
  delay(5000);  // allow time to WiFly to initialise
  if (!WiFly.join(SSID, PASSPHRASE, mode)) {
    wiflyConnectedToNetwork = false;
    delay(AFTER_ERROR_DELAY);
  } else {
    wiflyConnectedToNetwork = true;
    return 1;
  }
  return 0;
}


#endif  /* THEAIRBOARDWIFLYMQTTPUBLISHER_WIFLY_CONFIG_H_ */
