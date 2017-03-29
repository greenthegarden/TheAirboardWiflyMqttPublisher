#ifndef THEAIRBOARDWIFLYMQTTPUBLISHER_THEAIRBOARD_CONFIG_H_
#define THEAIRBOARDWIFLYMQTTPUBLISHER_THEAIRBOARD_CONFIG_H_


// The Airboard (https://github.com/theairboard/TheAirBoard)
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


void theairboard_init() {
  pinMode(RED, OUTPUT);         // initialize the RED LED pin as an output
  pinMode(GREEN, OUTPUT);       // initialize the GREEN LED pin as an output
  pinMode(BLUE, OUTPUT);        // initialize the BLUE LED pin as an output
  analogWrite(GREEN, 1);        // switch ON indicator at low power
  #if ENABLE_THEAIRBOARD_SLEEP
    pinMode(RF, OUTPUT);
    digitalWrite(RF, 1);          // turn on wireless module
  #endif
  Serial.begin(BAUD_RATE);
}

void publish_battery() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[BATTERY_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  dtostrf(board.batteryChk(), 1, FLOAT_DECIMAL_PLACES, payloadBuffer);
  mqttClient.publish(topicBuffer, payloadBuffer);
}

void publish_led_colour(byte colour_idx) {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[LED_COLOUR_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  mqttClient.publish(topicBuffer, itoa(colour_idx, payloadBuffer, 10));
}

void publish_temperature() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[TEMPERATURE_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  dtostrf(board.getTemp(), 1, FLOAT_DECIMAL_PLACES, payloadBuffer);
  mqttClient.publish(topicBuffer, payloadBuffer);
}


#endif /* THEAIRBOARDWIFLYMQTTPUBLISHER_THEAIRBOARD_CONFIG_H_ */
