#ifndef TIME_H
#define TIME_H

extern int days;
extern int hours;
extern int minutes;
extern int seconds;
extern struct tm timeinfo;

void init_time();
void update_time();
void set_time();
void set_time_zone();

#endif
