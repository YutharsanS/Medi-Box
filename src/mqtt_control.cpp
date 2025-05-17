#include <PubSubClient.h>
#include <WiFi.h>
#include "mqtt_control.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void receiveCallback(char* topic, byte* payload, unsigned int length){
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("]");
    
    // array to store the data
    char payloadCharAr[length];

    // convert data and store
    for(int i = 0; i < length; i++){
        Serial.print((char)payload[i]);
        payloadCharAr[i] = (char)payload[i];
    }

    Serial.println("");

    // turn on or off built in led
    if (!strcmp(topic, "ENTC-ON-OFF")){
        if (payloadCharAr[0] == '1') {
            digitalWrite(LED_BUILTIN, HIGH);
        } else {
            digitalWrite(LED_BUILTIN, HIGH);
        }
    }
}

void setupMqtt() {
  mqttClient.setServer("test.mosquitto.org", 1883);
  mqttClient.setCallback(receiveCallback);
}

void connectToBroker() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (mqttClient.connect("ESP32-06b53728-40f6-49cb-9d09-c73d89502a0b")){
      Serial.println("connected");
      mqttClient.subscribe("ENTC-ON-OFF");
    } else {
      Serial.print("failed");
      Serial.print(mqttClient.state());
      delay(2000);
    }
  }
}