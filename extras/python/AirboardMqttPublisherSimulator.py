#!/usr/bin/env python

#---------------------------------------------------------------------------------------
# Load configuration values
#
#---------------------------------------------------------------------------------------

# see https://wiki.python.org/moin/ConfigParserShootout
from configobj import ConfigObj
config = ConfigObj('AirboardMqttPublisher.cfg')

print("{0}".format("Airboard MQTT Publisher Simulator"))


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
	# print("Subscribing to topics ...")
	# for topic in config['mqtt_topics']['TOPICS'] :
	# 	client.subscribe(topic)
	# 	print("{0}".format(topic))

def on_subscribe(client, userdata, mid, granted_qos) :
    print("Subscribed: "+str(mid)+" "+str(granted_qos))

def on_message(client, userdata, message) :
#    print(time.clock())
	print(message.topic+" "+str(message.payload))
	if (message.topic == "theairboard/request/uptime/") :
		client.publish("theairboard/status/uptime", 45588)
	elif (message.topic == "theairboard/request/memory") :
		client.publish("theairboard/status/memory", 345)
	else :
		print("unexpected message received!!")
#    print("message received: topic is {0} with payload {1}".format(message.topic, message.payload))

def on_publish(client, userdata, mid) :
    print("mid: {0}".format(str(mid)))

def on_disconnect(client, userdata, rc) :
	print("Disconnect returned:")
	print("client: {0}".format(str(client)))
	print("userdata: {0}".format(str(userdata)))
	print("result: {0}".format(str(rc)))

def on_log(client, userdata, level, buf) :
    print("{0}".format(buf))

client               = mqtt.Client()
client.on_connect    = on_connect
#client.on_subscribe  = on_subscribe
#client.on_message    = on_message
client.on_publish    = on_publish
client.on_disconnect = on_disconnect
# Uncomment to enable debug messages
#client.on_log       = on_log

client.connect(
               config['mqtt_configuration']['MQTT_BROKER_IP'],
               int(config['mqtt_configuration']['MQTT_BROKER_PORT']),
               int(config['mqtt_configuration']['MQTT_BROKER_PORT_TIMEOUT']),
               )

client.loop_start()

from threading import Timer

def tidyupAndExit() :
	t.cancel()
	running = False       #Stop thread1
	# Disconnect mqtt client			mqttc.loop_stop()
	client.disconnect()
	print("Bye")
	exit(0)

import random
import time
import json

def publish_measurements() :
	try :
#		m = json.dumps({'status' : {'uptime' : time.time(), 'memory' : 234}, 'measurements' : [ {'status': 'OK', 'temperature': random.uniform(20.0, 30.0), 'humidity': random.uniform(15.0, 50.0)}, {'status': 'OK', 'temperature': random.uniform(20.0, 30.0), 'humidity': random.uniform(15.0, 50.0)}, {'status': 'OK', 'temperature': random.uniform(20.0, 30.0), 'humidity': random.uniform(15.0, 50.0)}]})
		m = json.dumps({'airboardstatus' : {'uptime' : time.time(), 'memory' : 234}})
		client.publish("home/sensor/theairboard", m)
#		m = json.dumps({'measurement' : 'dht22', 'measurement': 'OK', 'temperature': random.uniform(20.0, 30.0), 'humidity': random.uniform(15.0, 50.0)})
#		m = json.dumps({'measurement' : {'sensor' : 'dht22', 'status': 'OK', 'temperature': random.uniform(20.0, 30.0), 'humidity': random.uniform(15.0, 50.0)}})
#		client.publish("home/sensor/theairboard", m)
#		client.publish("theairboard/status/uptime", time.time())
		# client.publish("theairboard/status/memory", 234)
		# client.publish("theairboard/measurement/DHTT22_1", json.dumps({'status': 'OK', 'temperature': random.uniform(20.0, 30.0), 'humidity': random.uniform(15.0, 50.0)}))
		# client.publish("theairboard/measurement/DHTT22_2", json.dumps({'status': 'OK', 'temperature': random.uniform(20.0, 30.0), 'humidity': random.uniform(15.0, 50.0)}))
		# client.publish("theairboard/measurement/DHTT22_3", json.dumps({'status': 'OK', 'temperature': random.uniform(20.0, 30.0), 'humidity': random.uniform(15.0, 50.0)}))
#		client.publish("theairboard/measurement/DHT22_T", random.uniform(10.0, 30.0))
#		client.publish("theairboard/measurement/DHT22_H", random.uniform(15.0, 80.0))

		#{"store":{"book":[{"category":"reference","author":"Nigel Rees","title": "Sayings of the Century", "price": 8.95  } ],  "bicycle": { "color": "red",  "price": 19.95} }}
		j_str = "{'measurements' : ["
		for x in xrange(int(config['SENSORS'])) :
			j_str += "{'sensor' : 'dht22', 'status': 'OK', 'temperature': " + str(random.uniform(20.0, 30.0)) + ", 'humidity': " + str(random.uniform(15.0, 50.0)) + "}"
			if x != int(config['SENSORS'])-1 :
				j_str += ", "
		j_str += "]}"
		m = json.dumps(j_str)
		client.publish("home/sensor/theairboard", m)

		t = Timer(float(config['PUBLISH_INTERVAL']), publish_measurements)
		t.start()
	except KeyboardInterrupt :
		tidyupAndExit()

try :
	t = Timer(float(config['PUBLISH_INTERVAL']), publish_measurements)
	t.start()
except KeyboardInterrupt :      #Triggered by pressing Ctrl+C
	tidyupAndExit()
