#ifndef WALL_H_INCLUDED
#define WALL_H_INCLUDED

#include <stdint.h>
#include "lcd.h"

struct _wall
{
  uint8_t left;
  uint8_t top;
  uint8_t right;
};

void printWall(const struct _wall *const wall, DRAWTYP m);

#endif