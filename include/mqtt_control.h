#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h>
#define LED_BUILTIN 13

extern PubSubClient mqttClient;

void setupMqtt();
void connectToBroker();

#endif
