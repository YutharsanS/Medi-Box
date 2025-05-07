#include "display.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "time_control.h"
#include "buttons.h"
#include "sensor.h"
#include "values.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3c


// Class instance declarations
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void init_display()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    display.display();
    delay(100);
}

// wrapper function for clearing display
void clear_display()
{
    display.clearDisplay();
}

// wrapper function for displaying
void display_screen()
{
    display.display();
}

// Display a custom message
void print_line(String text, int row, int column, int text_size) {
    display.setTextSize(text_size);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(row, column);
    display.println(text); // Display the text variable - macro doesn't work, because it's not a static string
    display.display(); // flushes contents to display
}

// writes text to the display
void write_to_display(String text, int cursor_x, int cursor_y, int size)
{

    display.setTextSize(size);
    display.setTextColor(WHITE);
    display.setCursor(cursor_x, cursor_y);
    display.println(text);
}

// prints the current time in display
void print_time_now(void)
{
    clear_display();
    print_line(String(days), 0, 0, 2);
    print_line(":", 20, 0, 2);
    print_line(String(hours), 30, 0, 2);
    print_line(":", 50, 0, 2);
    print_line(String(minutes), 60, 0, 2);
    print_line(":", 80, 0, 2);
    print_line(String(seconds), 90, 0, 2);
    delay(200);
}


void home_screen()
{
    // Clear display
    display.clearDisplay();

    // Display time at the top
    char time_str[10];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", &timeinfo);
    write_to_display(time_str, 1, 0, 2);  // Row 1, column 0, font size 2

    // Display temperature and humidity (rows 3-4)
    char temp_str[16];
    char humid_str[16];
    sprintf(temp_str, "Temp: %.1f C", temperature);
    sprintf(humid_str, "Humid: %.1f%%", humidity);
    write_to_display(temp_str, 4, 20, 1);  // Row 3, smaller font
    write_to_display(humid_str, 4, 30, 1);  // Row 4, smaller font

    check_temperature_and_humidity();

    // Update the display
    display_screen();
}
