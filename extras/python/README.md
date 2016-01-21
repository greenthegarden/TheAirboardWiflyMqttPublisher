# MqttNotifier
Simple Python script to print topics and messages from a MQTT broker

The script used the [paho-mqtt] (https://pypi.python.org/pypi/paho-mqtt) Python module and an MQTT broker to connect to. I run the script on a Raspberry Pi running the [Mosquitto broker] (http://mosquitto.org/).

To use, add topics to be subscribed to within the function on_connect().
