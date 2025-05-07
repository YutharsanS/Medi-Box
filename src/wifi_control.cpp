#include "wifi_control.h"
#include "display.h"
#include <WiFi.h>

const char *default_ssid = "Wokwi-GUEST";
const char *default_password = "";

void init_wifi()
{
    WiFi.begin(default_ssid, default_password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(250);
        clear_display();
        print_line("Connecting to WIFI", 0, 0, 2);
        delay(250);
    }
    clear_display();
    print_line("Connected to WIFI", 0, 0, 2);
    clear_display();
    delay(1000);
}
