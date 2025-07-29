#include "sensor.h"
#include "display.h"
#include <DHTesp.h>
#include "alarm.h"
#include <ESP32Servo.h>


// dht sensor class instance
DHTesp dhtSensor;
Servo servo;

// ldr parameters
int samplingInterval = 2 * 1000;
int dataInterval = 10 * 1000;
int maxSamples = 24;
int samples[24];

int sampleCount = 0;
unsigned long lastSampledTime = 0;
unsigned long lastAveragedTime = 0;

// servo parameters
int minimumAngle = 30;
float controllingFactor = 0.75;
static int motorAngle = 30;

// dht22 parameters
int measuredTemperature;
int medicineTemperature = 30;

void init_dht()
{
    dhtSensor.setup(DHTPIN, DHTesp::DHT22);
    if (dhtSensor.getStatusString() != "OK")
    {
        Serial.println("DHT sensor error!");
    }
    else
    {
        Serial.println("DHT sensor initialized successfully.");
        clear_display();
        delay(1000);
    }
}

void init_servo()
{
    servo.attach(SERVO_PIN);
}

void control_motor(){
  // control servo
  servo.write(motorAngle);
}

// checks and controls temperature
void check_temperature_and_humidity()
{
    bool warning = false;
    TempAndHumidity data = dhtSensor.getTempAndHumidity();

    if (data.temperature > 32 || data.temperature < 24)
    {
        warning = true;
        write_to_display("Temperature Warning", 0, 50, 1);
    }

    if (data.humidity > 85 || data.humidity < 65)
    {
        warning = true;
        write_to_display("Humidity Warning", 0, 40, 1);
    }

    if (warning == true)
    {
        digitalWrite(LED1, HIGH);
        delay(500);
    }
    else
    {
        digitalWrite(LED1, LOW);
    }
}

// gets temperature and humidity

float* get_temperature_and_humidity() {
    static float result[2];  // preserve the array
    TempAndHumidity data = dhtSensor.getTempAndHumidity();
    result[0] = data.temperature;
    result[1] = data.humidity;
    return result;
}

void calculate_servo_angle(float lightIntensity) {
  // get temperature
  float *data = get_temperature_and_humidity();
  float temperature = data[0];
  
  // Calculate raw angle
  float rawAngle = minimumAngle + ((180 - minimumAngle) * lightIntensity * controllingFactor * temperature / medicineTemperature);
    
  // Ensure angle is within valid bounds
  motorAngle = (rawAngle < minimumAngle) ? minimumAngle : (rawAngle > 180) ? 180 : rawAngle;
}
