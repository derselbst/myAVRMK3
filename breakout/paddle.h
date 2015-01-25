#ifndef PADDLE_H_INCLUDED
#define PADDLE_H_INCLUDED

#include <stdint.h>
#include "../MK3_2560_LCD.h"

typedef uint8_t position;

struct _paddle
{
  position x;
  const position y;// = 60;
  uint8_t len; // lenght of the paddle in pixel
};

// height of padde of 2 pixel
#define PADDLEHEIGHT 2


void movePaddle(struct _paddle *const paddle);
void printPaddle(const struct _paddle *const paddle, DRAWTYP m);

#endif