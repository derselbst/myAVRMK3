#ifndef PADDLE_H_INCLUDED
#define PADDLE_H_INCLUDED

#include <stdint.h>
#include "../MK3_2560_LCD.h"
#include "wall.h"

typedef uint8_t position;

struct _paddle
{
  position x; // this is the middle of the paddle
  const position y;// = 60; // this is the upper bound of the paddle used for collision detection
  uint8_t len; // lenght of the paddle in pixel
};

// height of padde of 2 pixel
#define PADDLEHEIGHT 2


void movePaddle(struct _paddle *const paddle, const struct _wall *const wall);
void printPaddle(const struct _paddle *const paddle, DRAWTYP m);

#endif
