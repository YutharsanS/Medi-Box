#include "sensor.h"
#include "display.h"
#include <DHTesp.h>
#include "alarm.h"

// dht sensor class instance
DHTesp dhtSensor;

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
