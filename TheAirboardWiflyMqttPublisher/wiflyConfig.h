/*
 * RN-XV WiFly Module - Wire Antenna
 * MAC: 00:06:66:50:71:6f
 * IP: 192.168.1.52
 *
 * RN-XV WiFly Module – SMA
 * MAC: 00:06:66:71:68:d5
 * IP: 192.168.1.51
 */

/*
 * WiFly configuration
 * reboot
 * $$$
 * factory RESET
 *
 * set wlan join 0    // Stop device connecting while we setup
 *
 * set ip dhcp 3
 * set wlan ssid xxx
 * set wlan phrase xxx
 * set wlan join 1
 *
 * set time address 203.0.178.191
 * set time zone 0
 * set time enable 1
 *
 * save
 * reboot
 */


#ifndef THEAIRBOARDWIFLYMQTTPUBLISHER_WIFLY_CONFIG_H_
#define THEAIRBOARDWIFLYMQTTPUBLISHER_WIFLY_CONFIG_H_


// WiFly libraries
#include <SPI.h>
#include <WiFly.h>

#include "networkConfig.h"

boolean wiflyConnectedToNetwork = false;

WiFlyClient    wiflyClient;

#if ENABLE_THEAIRBOARD_SUPPORT

#define RX 0                      // digital I/O pin for the TheAirboard UART serial receive
#define TX 1                      // digital I/O pin for the TheAirboard UART serial receive port

const byte UART_RX        = RX;   // TheAirboard UART serial receive (RX) port connected to TX of WiFly
const byte UART_TX        = TX;   // TheAirboard UART serial transmit (TX) port connected to TX of WiFly

void wifly_configure() {
  WiFly.setUart(&Serial);
}

#else

const byte UART_RX        = 11;
const byte UART_TX        = 12;

#include <SoftwareSerial.h>

SoftwareSerial wiflySerial(UART_RX, UART_TX);

void wifly_configure() {
  wiflySerial.begin(BAUD_RATE);
  WiFly.setUart(&wiflySerial);
}
#endif

byte wifly_connect() {
  DEBUG_LOG(1, "initialising wifly");

  WiFly.begin();
  delay(5000);  // allow time to WiFly to initialise

  DEBUG_LOG(1, "joining network");

  if (!WiFly.join(SSID, PASSPHRASE, mode)) {
//  if (!WiFly.join(MY_SSID)) {   // needs to be fixed to allow a passphrase if secure
    wiflyConnectedToNetwork = false;
    DEBUG_LOG(1, "  failed");
    delay(AFTER_ERROR_DELAY);
  } else {
    wiflyConnectedToNetwork = true;
    DEBUG_LOG(1, "  connected");
    return 1;
  }
  return 0;
}


#endif  /* THEAIRBOARDWIFLYMQTTPUBLISHER_WIFLY_CONFIG_H_ */
