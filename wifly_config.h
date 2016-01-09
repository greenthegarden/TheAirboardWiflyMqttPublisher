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


#include <SPI.h>
#include <WiFly.h>

WiFlyClient    wifly_client;

boolean wifly_connected           = false;


#if ENABLE_THEAIRBOARD_SUPPORT
#define RX 0                      // digital I/O pin for the TheAirboard UART serial receive
#define TX 1                      // digital I/O pin for the TheAirboard UART serial receive port
const byte UART_RX        = RX;   // TheAirboard UART serial receive (RX) port connected to TX of WiFly
const byte UART_TX        = TX;   // TheAirboard UART serial transmit (TX) port connected to TX of WiFly
#else
const byte UART_RX        = 11;
const byte UART_TX        = 12;


#include <SoftwareSerial.h>

SoftwareSerial wifly_serial(UART_RX, UART_TX);
void wifly_configure()
{
  // Configure WiFly
  wifly_serial.begin(BAUD_RATE);
  WiFly.setUart(&wifly_serial);
}
#endif


#endif  /* THEAIRBOARDWIFLYMQTTPUBLISHER_WIFLY_CONFIG_H_ */
