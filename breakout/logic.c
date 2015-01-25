#include <stdbool.h>
#include <stdint.h>

#include "../common.h"
#include "../MK3_2560_LCD.h"
#include "ball.h"
#include "paddle.h"
#include "wall.h"

static void collisionWall(struct _ball *const ball, struct _wall *const wall)
{
  if(ball->x+BALLSIZE == wall->left+1)
  {
    // collision with left wall
    ball->dx=1;
  }
  else if(ball->x+BALLSIZE == wall->right-1)
  {
    // collision with right wall
    ball->dx=-1;
  }
  
  if(ball->y+BALLSIZE == wall->top+1)
  {
    ball->dy=1;
  }
}

static bool collisionPaddle(struct _ball *const ball, const struct _paddle *const paddle)
{
  if(ball->y+BALLSIZE == paddle->y-PADDLEHEIGHT)
  {
    // collision with paddle
    ball->dy = 1;
    return true;
  }
  return false;
}

static bool lost(const struct _ball *const ball)
{
  return ball->y > LCD_MAXY;
}

void breakout()
{
  struct _paddle paddle={LCD_MAXX/2, 60, 8};
  struct _ball ball={LCD_MAXX/2, LCD_MAXY/2, PORTK & 0x3, PORTK & 0xC};
  struct _wall wall={0,0,LCD_MAXX};
  
  printWall(&wall, MODE_SET);
  printPaddle(&paddle, MODE_SET);
  printBall(&ball, MODE_SET);
  
 while(!lost(&ball)) 
 {
   
  collisionPaddle(&ball, &paddle);
  collisionWall(&ball, &wall);
  movePaddle(&paddle);
  moveBall(&ball);
  
  waitMs(100);
}
}