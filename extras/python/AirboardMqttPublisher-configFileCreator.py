# config file creator

from configobj import ConfigObj
config = ConfigObj()
config.filename = 'AirboardMqttPublisher.cfg'


# mqtt configuration
mqtt_configuration = {
	'MQTT_BROKER_IP'           : "localhost",
	'MQTT_BROKER_PORT'         : "1883",
	'MQTT_BROKER_PORT_TIMEOUT' : "60",
	}
config['mqtt_configuration'] = mqtt_configuration


config['mqtt_topics'] = {}
# add additional topics to subscribe to here!!
config['mqtt_topics']['TOPICS'] = [
                                   'home/sensor/theairboard',
                                   'theairboard/status/#',
								   'theairboard/request/#',
                                   'theairboard/measurement/#',
                                   ]

config['PUBLISH_INTERVAL'] = 15.0


config.write()
