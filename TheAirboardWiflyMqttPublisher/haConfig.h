#ifndef THEAIRBOARDWIFLYMQTTPUBLISHER_HA_CONFIG_H_
#define THEAIRBOARDWIFLYMQTTPUBLISHER_HA_CONFIG_H_

#include <ArduinoHA.h>

#define BROKER_ADDR IPAddress(192, 168, 1, 186)

HADevice device("theAirboard");

HAMqtt mqtt(wiflyClient, device);

// "myValve" is unique ID of the sensor. You should define your own ID.
HASensorNumber theAirboardBattery("theAirboardBattery");
HASensorNumber theAirboardTemperature("theAirboardTemperature");

void onMqttMessage(const char *topic, const uint8_t *payload, uint16_t length)
{
  // This callback is called when message from MQTT broker is received.
  // Please note that you should always verify if the message's topic is the one you expect.
  // For example: if (memcmp(topic, "myCustomTopic") == 0) { ... }

  Serial.print("New message on topic: ");
  Serial.println(topic);
  Serial.print("Data: ");
  Serial.println((const char *)payload);

  mqtt.publish("myPublishTopic", "hello");
}

void onMqttConnected()
{
  Serial.println("Connected to the broker!");

  // You can subscribe to custom topic if you need
  mqtt.subscribe("myCustomTopic");
}

// void setup()
// {
//   Serial.begin(9600);
//   Ethernet.begin(mac);

//   mqtt.onMessage(onMqttMessage);
//   mqtt.onConnected(onMqttConnected);

//   // If you use custom discovery prefix you can change it as following:
//   // mqtt.setDiscoveryPrefix("customPrefix");

//   // If you want to change prefix only for non-discovery prefix:
//   // mqtt.setDataPrefix("data");

//   mqtt.begin(BROKER_ADDR);
// }

// void loop()
// {
//   Ethernet.maintain();
//   mqtt.loop();
// }

#endif /* THEAIRBOARDWIFLYMQTTPUBLISHER_HA_CONFIG_H_ */
