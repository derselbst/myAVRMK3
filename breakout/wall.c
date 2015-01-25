#include <stdint.h>
#include "../MK3_2560_LCD.h"
#include "wall.h"

void printWall(const struct _wall *const wall, DRAWTYP m)
{
  uint8_t i;
  for (i=wall->top; i< LCD_MAXX; i++)
  {
    MK3_LCD_PIXEL_AT(wall->left, i, m);
    MK3_LCD_PIXEL_AT(wall->right, i, m);
  }
  
  for (i=wall->left+1; i< wall->right; i++)
  {
    MK3_LCD_PIXEL_AT(i, wall->top, m);
  }
}