#include <stdbool.h>
#include <stdint.h>

#include "../common.h"
#include "../MK3_2560_LCD.h"
#include "../led.h"
#include "../button.h"
#include "ball.h"
#include "paddle.h"
#include "wall.h"

static void collisionWall(struct _ball *const ball, const struct _wall *const wall)
{
    if(ball->x-BALLSIZE == wall->left+1)
    {
        // collision with left wall
        ball->dx=1;
    }
    else if(ball->x+BALLSIZE == wall->right-1)
    {
        // collision with right wall
        ball->dx=-1;
    }

    if(ball->y-BALLSIZE == wall->top+1)
    {
        ball->dy=1;
    }
}

static bool collisionPaddle(struct _ball *const ball, const struct _paddle *const paddle)
{
    // ball must be above paddle and between both ends of paddle
    if(ball->y+1+BALLSIZE == paddle->y && ((ball->x >= paddle->x+paddle->len) && (ball->x <= paddle->x+paddle->len)))
    {
        // collision with paddle
        ball->dy = -1;
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
    MK3_LCD_CLEAR();
    struct _paddle paddle= {LCD_MAXX/2, 60, 32};
    struct _ball ball= {LEDPORT, LEDPORT & 0x1F, -1, -1};
    const struct _wall wall= {0,0,LCD_MAXX-1};

    printWall(&wall, MODE_SET);
    printPaddle(&paddle, MODE_SET);
    printBall(&ball, MODE_SET);

    while(!lost(&ball) && !get_key_press((1<<KEY2) | (1 << KEY1) | (1 << KEY0)))
    {

        collisionPaddle(&ball, &paddle);
        collisionWall(&ball, &wall);
        movePaddle(&paddle, &wall);
        moveBall(&ball);

        waitMs(100);
    }

    MK3_LCD_STRING_PX(0,LCD_MAXY-2*9, "Button3: Hauptmenu");
    while (!get_key_press( 1 << KEY2 ))
    {
    }
}
