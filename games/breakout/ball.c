#include <stdint.h>
#include "ball.h"
#include "lcd.h"

void printBall(const struct _ball *const ball, DRAWTYP m)
{
    int8_t j,i;
    for(j = 0; j <=BALLSIZE; j++)
        for(i = -BALLSIZE+j; i <= BALLSIZE-j; i++)
        {
            MK3_LCD_PIXEL_AT(ball->x+i, ball->y+j, m);
            __asm volatile ("nop \n"); // wait a tick
            if(j)
            {
                MK3_LCD_PIXEL_AT(ball->x+i, ball->y-j, m);
                __asm volatile ("nop \n"); // wait a tick
            }
        }
}

void moveBall(struct _ball *const ball)
{
    printBall(ball, MODE_RESET);
    ball->x += ball->dx;
    ball->y += ball->dy;
    printBall(ball, MODE_SET);
}
