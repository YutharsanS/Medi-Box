#include "alarm.h"
#include "display.h"
#include "buttons.h"
#include "time_control.h"
#include <Arduino.h>

#define BUZZER 5
#define LED_1 15

bool alarm_enabled = true;
int n_alarms = 2;
int alarm_hours[] = {20, 20};
int alarm_minutes[] = {20, 20};
bool alarm_triggered[] = {false, false};

int n_notes = 8;
int C = 262;
int D = 294;
int E = 330;
int F = 349;
int G = 392;
int A = 440;
int B = 494;
int C_H = 523;
int notes[] = {C, D, E, F, G, A, B, C_H};

int warning_notes[] = {C, C_H};
bool stop_warning = false;

void ring_alarm(int alarm_index)
{
    clear_display();
    print_line("MEDICINE TIME", 0, 0, 2);

    digitalWrite(LED_1, HIGH);

    bool break_happened = false;

    // Ring the buzzer
    while (!break_happened)
    {
        for (int i = 0; i < n_notes; i++)
        {
            if (digitalRead(PB_CANCEL) == LOW)
            {
                // Stop the alarm
                delay(200);
                break_happened = true;
                alarm_triggered[alarm_index] = true; // Mark alarm as triggered
                break;
            }

            if (digitalRead(PB_OK) == LOW)
            {
                // Snooze the alarm for 5 minutes
                delay(200);
                alarm_triggered[alarm_index] = false;
                alarm_minutes[alarm_index] += 1;
                if (alarm_minutes[alarm_index] >= 60)
                {
                    alarm_minutes[alarm_index] -= 60;
                    alarm_hours[alarm_index] = (alarm_hours[alarm_index] + 1) % 24;
                }
                break_happened = true;
                break;
            }

            tone(BUZZER, notes[i]);
            delay(500);
            noTone(BUZZER);
            delay(2);
        }
    }
    digitalWrite(LED_1, LOW);
    clear_display();
}

void update_time_with_check_alarm(void)
{
    update_time();

    if (alarm_enabled == true)
    {
        for (int i = 0; i < n_alarms; i++)
        {
            if (alarm_triggered[i] == false && alarm_hours[i] == hours && alarm_minutes[i] == minutes)
            {
                alarm_triggered[i] = true;
                ring_alarm(i);
            }
        }
    }
}

void set_alarm(int alarm)
{
    int temp_hour = alarm_hours[alarm];

    while (true)
    {
        clear_display();
        print_line("Enter hour: " + String(temp_hour), 0, 0, 2);

        int pressed = wait_for_button_press();

        if (pressed == PB_UP)
        {
            delay(200);
            temp_hour += 1;
            temp_hour = temp_hour % 24;
        }

        if (pressed == PB_DOWN)
        {
            delay(200);
            temp_hour -= 1;
            if (temp_hour < 0)
            {
                temp_hour = 23;
            }
        }

        else if (pressed == PB_OK)
        {
            delay(200);
            alarm_hours[alarm] = temp_hour;
            alarm_triggered[alarm] = false;
            break;
        }

        else if (pressed == PB_CANCEL)
        {
            delay(200);
            go_to_menu();
        }
    }

    int temp_minutes = alarm_minutes[alarm];

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
            alarm_minutes[alarm] = temp_minutes;
            break;
        }

        else if (pressed == PB_CANCEL)
        {
            delay(200);
            go_to_menu();
        }
    }
    clear_display();
    print_line("Alarm Set", 0, 0, 2);
    delay(1000);
}

// Delete specific alarm
void delete_alarm()
{
    int alarm_to_delete = 0;

    while (true)
    {
        clear_display();
        print_line("Del Alarm: " + String(alarm_to_delete + 1), 0, 0, 2);

        int pressed = wait_for_button_press();

        if (pressed == PB_UP)
        {
            delay(200);
            alarm_to_delete = (alarm_to_delete + 1) % n_alarms;
        }

        if (pressed == PB_DOWN)
        {
            delay(200);
            alarm_to_delete = (alarm_to_delete - 1 + n_alarms) % n_alarms;
        }

        else if (pressed == PB_OK)
        {
            delay(200);
            alarm_hours[alarm_to_delete] = -1; // flag deleted alarm
            alarm_minutes[alarm_to_delete] = -1;
            alarm_triggered[alarm_to_delete] = false;
            break;
        }

        else if (pressed == PB_CANCEL)
        {
            delay(200);
            break;
        }
    }

    clear_display();
    print_line("Alarm Deleted", 0, 0, 2);
    delay(2000);
}

// view all the alarm
void view_alarms()
{
    while (true)
    {
        clear_display();
        int row = 0;
        for (int i = 0; i < n_alarms; i++)
        {
            if (alarm_hours[i] == -1 || alarm_minutes[i] == -1)
            {
                continue;
            }

            print_line("Alarm " + String(i + 1) + ": " + String(alarm_hours[i]) + ":" + String(alarm_minutes[i]), 0, i * 10, 1);
            row++;
        }

        // If no active alarms, display a message
        if (row == 0)
        {
            print_line("No Active Alarms", 0, 0, 2);
        }

        int pressed = wait_for_button_press();

        if (pressed == PB_CANCEL)
        {
            delay(200);
            break;
        }
    }
}

void ring_warning()
{
    while (!stop_warning)
    {
        for (int i = 0; i < sizeof(warning_notes) / sizeof(warning_notes[0]); i++)
        {
            if (digitalRead(PB_CANCEL) == LOW)
            {
                delay(200);
                stop_warning = true;
                break;
            }
            tone(BUZZER, warning_notes[i]);
            delay(500);
            noTone(BUZZER);
            delay(2);
        }
    }
}
