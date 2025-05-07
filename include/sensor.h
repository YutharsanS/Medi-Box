#ifndef SENSOR_H
#define SENSOR_H

#define DHTPIN 12

void init_dht();
void check_temperature_and_humidity();
float *get_temperature_and_humidity();

#endif
