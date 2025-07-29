#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>
#include <WiFi.h>
#include <display.h>
#include <time_control.h>
#include <buttons.h>
#include "alarm.h"
#include "sensor.h"
#include "wifi_control.h"
#include "values.h"
#include "mqtt_control.h"

void setup()
{
  // initializing the pins
  init_pins();

  Serial.begin(115200);

  // Initialize the OLED display
  init_display();

  // Initialize the DHT sensor
  init_dht();

  // Initialize the wifi connection
  init_wifi();

  // Initialize the time
  init_time();

  // Setup mqtt
  setupMqtt();

  // Intialize Servo
  init_servo();

  delay(500);
  print_line("Welcome to Medibox!", 5, 15, 2);
  delay(2000);
  clear_display();
}

void loop()
{
  // Control Servo
  control_motor();

  // Get the temperature and humidity
  float *data = get_temperature_and_humidity();
  temperature = data[0];
  humidity = data[1];

  // publish to mqtt
  if (!mqttClient.connected()) {
    connectToBroker();
  }

  mqttClient.loop(); // keeps the pub sub active


  unsigned long current_time = millis();

    // sampling
  if ((current_time - lastSampledTime) >= samplingInterval && sampleCount < maxSamples) {
    samples[sampleCount] = analogRead(LDR_PIN);
    sampleCount++;
    lastSampledTime = current_time;
  }

  // averaging out
  if ((current_time - lastAveragedTime) >= dataInterval && sampleCount > 0) {
    long sum = 0;
    for(int i = 0; i < sampleCount; i++){
      sum += samples[i];
    }

    int average = sum / sampleCount;

    // normalizing the average
    float normalized = 1.0 - (average / 4095.0);

    // Creating JSON payload
    char payload[50];
    snprintf(payload, sizeof(payload), "%.2f", normalized);
    Serial.println(payload);
    // Send to mqtt broker
    mqttClient.publish("ENTC-LIGHT", payload);
    // Calculate servo angle
    calculate_servo_angle(normalized);

    // resetting for the next batch 
    lastAveragedTime = current_time;
    sampleCount = 0;
  }

  // put your main code here, to run repeatedly:
  update_time_with_check_alarm();
  clear_display();
  home_screen();

  if (digitalRead(PB_OK) == LOW)
  {
    delay(200); // allow the pushbutton to debounce
    go_to_menu();
  }

  delay(200); // this speeds up the simulation
}
