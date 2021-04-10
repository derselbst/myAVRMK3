#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED

#include <stdint.h>

struct _time
{
    uint8_t hour;
    uint8_t minute;
    uint8_t sec;
};

extern volatile uint32_t time;
extern volatile uint8_t tick;

void timeLoop();

#endif // TIME_H_INCLUDED
