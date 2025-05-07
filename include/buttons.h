#ifndef BUTTONS_H
#define BUTTONS_H

#define PB_CANCEL 34
#define PB_DOWN 35
#define PB_OK 32
#define PB_UP 33

extern int current_mode;

void init_pins();
bool is_button_pressed(int button);
int wait_for_button_press();
void go_to_menu();

#endif
