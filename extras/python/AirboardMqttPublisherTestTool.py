#!/usr/bin/env python

#---------------------------------------------------------------------------------------
# Load configuration values
#
#---------------------------------------------------------------------------------------

# see https://wiki.python.org/moin/ConfigParserShootout
from configobj import ConfigObj
config = ConfigObj('AirboardMqttPublisher.cfg')

print("{0}".format("Airboard MQTT Publisher Test Tool"))

import json

#---------------------------------------------------------------------------------------
# Modules and methods to support MQTT
#
#---------------------------------------------------------------------------------------

import paho.mqtt.client as mqtt

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc) :
	print("Connected with result code "+str(rc))
	# Subscribing in on_connect() means that if the connection is lost
	# the subscriptions will be renewed when reconnecting.
	print("Subscribing to topics ...")
	for topic in config['mqtt_topics']['TOPICS'] :
		client.subscribe(topic)
		print("{0}".format(topic))

def on_subscribe(client, userdata, mid, granted_qos) :
    print("Subscribed: "+str(mid)+" "+str(granted_qos))

def on_message(client, userdata, message) :
#    print(time.clock())
#	print(message.topic+" "+str(message.payload))
#    print("message received: topic is {0} with payload {1}".format(message.topic, message.payload))
	if (message.topic == "home/sensor/theairboard") :
		mess = json.loads(message.payload)
		print("status: {0}".format(str(mess["status"])))
		for measurement in mess["measurements"]	 :
			print("measurement: {0}".format(str(measurement)))

def on_publish(client, userdata, mid) :
    print("mid: {0}".format(str(mid)))

def on_disconnect(client, userdata, rc) :
	print("Disconnect returned:")
	print("client: {0}".format(str(client)))
	print("userdata: {0}".format(str(userdata)))
	print("result: {0}".format(str(rc)))

def on_log(client, userdata, level, buf) :
    print("{0}".format(buf))

mqttc               = mqtt.Client()
mqttc.on_connect    = on_connect
mqttc.on_subscribe  = on_subscribe
mqttc.on_message    = on_message
mqttc.on_publish    = on_publish
mqttc.on_disconnect = on_disconnect
# Uncomment to enable debug messages
#mqttc.on_log       = on_log


# Connect
#mqttc.username_pw_set(url.username, url.password)
mqttc.connect(config['mqtt_configuration']['MQTT_BROKER_IP'],
	               int(config['mqtt_configuration']['MQTT_BROKER_PORT']),
	               int(config['mqtt_configuration']['MQTT_BROKER_PORT_TIMEOUT']),
	               )

mqttc.loop_start()

command_types_valid_inputs = ['t', 'm']

print("Enter 't' for uptime or 'm' for free memory: ")

# Loop continuously
while True :
	try :
		command_type = raw_input()
		if command_type in command_types_valid_inputs :
			if command_type is 't' :
				mqttc.publish("relayduino/request/uptime");
			elif command_type is 'm' :
				mqttc.publish("relayduino/request/memory");
			else :
				print("Should not be here!!")
		else :
			print("Invalid command type entered")

	except KeyboardInterrupt :      #Triggered by pressing Ctrl+C
		running = False       #Stop thread1
		# Disconnect mqtt client
		mqttc.loop_stop()
		mqttc.disconnect()
		print("Bye")
		break         #Exit
