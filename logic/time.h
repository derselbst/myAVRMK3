#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED

#include <stdint.h>

struct _time
{
    uint8_t hour;
    uint8_t wday;
    uint8_t minute;
    uint8_t sec;
};

extern volatile struct _time time;

void timeLoop();

#endif // TIME_H_INCLUDED
