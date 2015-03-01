#include <stdint.h>

#include "../MK3_2560_LCD.h"
#include "paddle.h"
#include "../common.h"
#include "../button.h"

void printPaddle(const struct _paddle *const paddle, DRAWTYP m)
{
    int8_t y,x;
    for(y=0; y<PADDLEHEIGHT; y++)
    {
        for(x=-(paddle->len/2); x<(paddle->len/2); x++)
        {
            MK3_LCD_PIXEL_AT(paddle->x+x, paddle->y-y, m);
            __asm volatile ("nop \n"); // wait a tick
        }
    }
}

void movePaddle(struct _paddle *const paddle, const struct _wall *const wall)
{
    printPaddle(paddle, MODE_RESET);

    if(get_key_state(1<< JOYLEFT) && ((paddle->x - paddle->len/2) > wall->left  + MOVEPIXEL))
    {
        paddle->x-=MOVEPIXEL;
    }
    else if(get_key_state(1<<JOYRIGHT) && ((paddle->x + paddle->len/2) < wall->right - MOVEPIXEL))
    {
        paddle->x+=MOVEPIXEL;
    }

    printPaddle(paddle, MODE_SET);
}
