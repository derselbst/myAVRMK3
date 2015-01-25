#include <stdint.h>

#include "../MK3_2560_LCD.h"
#include "paddle.h"
#include "../common.h"
#include "../button.h"

void printPaddle(const struct _paddle *const paddle, DRAWTYP m)
{
  int8_t y,x;
  for(y=0; y<PADDLEHEIGHT; y++)
    for(x=-(paddle->len>>1); x<(paddle->len >> 1); x++)
      MK3_LCD_PIXEL_AT(paddle->x+x, paddle->y-y, m);
}  

void movePaddle(struct _paddle *const paddle)
{
  printPaddle(paddle, MODE_RESET);
  
  if(get_key_state(1<< JOYLEFT))
  {
    paddle->x-=1;
    printPaddle(paddle, MODE_SET);
  }
  else if(get_key_state(1<<JOYRIGHT))
  {
    paddle->x+=1;
    printPaddle(paddle, MODE_SET);
  }
  
  printPaddle(paddle, MODE_SET);
}
