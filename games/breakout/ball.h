#ifndef BALL_H_INCLUDED
#define BALL_H_INCLUDED

#include <stdint.h>
#include "lcd.h"

typedef uint8_t position;

struct _ball
{
  position x;
  position y;
  int8_t dx:2;
  int8_t dy:2;
};

#define BALLSIZE 2

void printBall(const struct _ball *const ball, DRAWTYP m);
void moveBall(struct _ball *const ball);

#endif