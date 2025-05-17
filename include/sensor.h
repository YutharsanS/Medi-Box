#ifndef SENSOR_H
#define SENSOR_H

#define DHTPIN 12
#define SERVO_PIN 25
#define LDR_PIN 36

extern int samplingInterval;
extern int dataInterval;
extern int maxSamples;
extern float controllingFactor;
extern int medicineTemperature;

extern int samplingInterval;
extern int dataInterval;
extern int maxSamples;
extern int samples[24];

extern int sampleCount;
extern unsigned long lastSampledTime;
extern unsigned long lastAveragedTime;

extern int minimumAngle;


void init_dht();
void init_servo();
void control_motor();
void check_temperature_and_humidity();
float *get_temperature_and_humidity();
void calculate_servo_angle(float lightIntensity);

#endif
