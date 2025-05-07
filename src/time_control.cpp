#include "time_control.h"
#include <WiFi.h>
#include <time.h>
#include "buttons.h"
#include "display.h"

int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;
struct tm timeinfo;

#define NTP_SERVER "pool.ntp.org"
#define UTC_OFFSET 0
#define UTC_OFFSET_DST 0


void init_time()
{
    configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
}

// fetch the time from ntp server and updating it
void update_time()
{
    if (!getLocalTime(&timeinfo))
    {
        print_line("Failed to fetch time", 0, 0, 1);
        display_screen();
        delay(200);
        return;
    }

    // Extract time components directly from timeinfo struct
    hours = timeinfo.tm_hour;
    minutes = timeinfo.tm_min;
    seconds = timeinfo.tm_sec;
    days = timeinfo.tm_mday;
}

tm get_time_info(){
    return timeinfo;
}

void set_time()
{
    int temp_hours = hours;

    while (true)
    {
        clear_display();
        print_line("Enter hour: " + String(temp_hours), 0, 0, 2);

        int pressed = wait_for_button_press();

        if (pressed == PB_UP)
        {
            delay(200);
            temp_hours += 1;
            temp_hours = temp_hours % 24;
        }

        if (pressed == PB_DOWN)
        {
            delay(200);
            temp_hours -= 1;
            if (temp_hours < 0)
            {
                temp_hours = 23;
            }
        }

        else if (pressed == PB_OK)
        {
            delay(200);
            hours = temp_hours;

        }

        else if (pressed == PB_CANCEL)
        {
            delay(200);
            break;
        }
    }

    int temp_minutes = minutes;

    while (true)
    {
        clear_display();
        print_line("Enter minutes: " + String(temp_minutes), 0, 0, 2);

        int pressed = wait_for_button_press();

        if (pressed == PB_UP)
        {
            delay(200);
            temp_minutes += 1;
            temp_minutes = temp_minutes % 60;
        }

        if (pressed == PB_DOWN)
        {
            delay(200);
            temp_minutes -= 1;
            if (temp_minutes < 0)
            {
                temp_minutes = 59;
            }
        }

        else if (pressed == PB_OK)
        {
            delay(200);
            minutes = temp_minutes;
            break;
        }

        else if (pressed == PB_CANCEL)
        {
            delay(200);
            go_to_menu();
        }
    }

    clear_display();
    print_line("Time is set", 0, 0, 2);
    delay(2000);
}

// Function to set the time zone offset (supports fractional offsets)
void set_time_zone()
{
    float utc_offset = UTC_OFFSET;
    float temp_utc_offset = utc_offset;

    while (true)
    {
        clear_display();
        print_line("UTC Offset: " + String(temp_utc_offset, 1), 0, 0, 2);

        int pressed = wait_for_button_press();

        if (pressed == PB_UP)
        {
            delay(200);
            temp_utc_offset += 0.5;

            if (temp_utc_offset > 14.0)
            {
                temp_utc_offset = -12.0;
            }
        }

        if (pressed == PB_DOWN)
        {
            delay(200);
            temp_utc_offset -= 0.5;

            if (temp_utc_offset < -12.0)
            {
                temp_utc_offset = 14.0;
            }
        }

        else if (pressed == PB_OK)
        {
            delay(200);
            utc_offset = temp_utc_offset;

            configTime(utc_offset * 3600, UTC_OFFSET_DST, NTP_SERVER);
            break;
        }

        else if (pressed == PB_CANCEL)
        {
            delay(200);
            break;
        }
    }

    clear_display();
    print_line("Time Zone Set", 0, 0, 2);
    delay(1000);
}
