#include <PubSubClient.h>
#include <WiFi.h>
#include "mqtt_control.h"
#include "sensor.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void receiveCallback(char* topic, byte* payload, unsigned int length){
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("]");
    
    // array to store the data with null termination character
    char payloadCharAr[length + 1];

    // convert data and store
    for(int i = 0; i < length; i++){
        Serial.print((char)payload[i]);
        payloadCharAr[i] = (char)payload[i];
    }

    // add null termination character
    payloadCharAr[length] = '\0';

    Serial.println("");

    // assign the values if it's from interval
    if (!(strcmp(topic, "ENTC-INTERVAL"))) {
      char* valueStr = &payloadCharAr[1]; 

      if (payloadCharAr[0] == 's'){
        samplingInterval = atoi(valueStr) * 1000; // Convert to integer and multiply by 1000 to get milliseconds
        Serial.print("New sampling interval: ");
        Serial.println(samplingInterval);
      } else if (payloadCharAr[0] == 'd'){
        dataInterval = atoi(valueStr) * 1000;
        Serial.print("New data interval: ");
        Serial.println(dataInterval);
      }

      // recompute the max_samples
      maxSamples = dataInterval / samplingInterval;

    } else if (!(strcmp(topic, "ENTC-ANGLE"))) {
      minimumAngle = atoi(payloadCharAr);
      Serial.print("New minimum angle: ");
      Serial.println(minimumAngle);
    } else if (!(strcmp(topic, "ENTC-CONTROL"))) {
      controllingFactor = atof(payloadCharAr);
      Serial.print("New controlling factor: ");
      Serial.println(controllingFactor);
    } else if (!(strcmp(topic, "ENTC-TEMP"))) {
      medicineTemperature = atoi(payloadCharAr);
      Serial.print("New medicine temperature: ");
      Serial.println(medicineTemperature);
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
      mqttClient.subscribe("ENTC-INTERVAL");
      mqttClient.subscribe("ENTC-ANGLE");
      mqttClient.subscribe("ENTC-CONTROL");
      mqttClient.subscribe("ENTC-TEMP1");
    } else {
      Serial.print("failed");
      Serial.print(mqttClient.state());
      delay(2000);
    }
  }
}