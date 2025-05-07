#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

void init_display();
void clear_display();
void display_screen();
void print_line(String text, int column, int row, int text_size);
void write_to_display(String text, int cursorX, int cursorY, int size);
void home_screen();
void print_time_now(void);

#endif
