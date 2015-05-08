#include <stdbool.h>
#include <stdint.h>

#include "../common.h"
#include "../MK3_2560_LCD.h"
#include "../led.h"
#include "../button.h"
#include "ball.h"
#include "paddle.h"
#include "wall.h"

// evaluate each operand exactly once, otherwise bad results if the operand has side effects
#define max(a,b) \
	    ({ \
			typeof (a) _a = (a); \
			typeof (b) _b = (b); \
			_a > _b ? _a : _b;   \
		})

#define min(a,b) \
	    ({ \
			typeof (a) _a = (a); \
			typeof (b) _b = (b); \
			_a < _b ? _a : _b;   \
		})

#define swap(a,b) \
		({ \
			typeof (a) _a = (a); \
			typeof (b) _b = (b); \
			(b) = _a;            \
			(a) = _b;            \
		})

/**
 * @brief checks if a collision in one dimension occurred
 *
 * checks if a collision in one dimensional space occurred
 *
 * @param upperBound the coordinate of the upper included bound
 * @param lowerBound the coordinate of the lower included bound
 * @param pos the currenz position of to object to test
 */
inline bool isCollision(uint8_t upperBound, uint8_t lowerBound, const uint8_t pos)
{
    if (upperBound < lowerBound)
    {
        swap(upperBound, lowerBound);
    }
    return lowerBound <= pos && pos <= upperBound; // lowerBound <= pos <= upperBound
}


static void collisionWall(struct _ball *const ball, const struct _wall *const wall)
{
    if(isCollision(wall->left + BALLSIZE + 1, wall->left, ball->x))
    {
        // collision with left wall
        ball->dx=1;
    }
    else if(isCollision(wall->right, wall->right - BALLSIZE - 1, ball->x))
    {
        // collision with right wall
        ball->dx=-1;
    }

    if(isCollision(wall->top + BALLSIZE + 1, wall->top, ball->y))
    {
        ball->dy=1;
    }
}

static bool collisionPaddle(struct _ball *const ball, const struct _paddle *const paddle)
{
    // ball must be above paddle and between both ends of paddle
    if(isCollision(paddle->y+PADDLEHEIGHT, paddle->y, ball->y + BALLSIZE + 1) && isCollision(paddle->x + paddle->len/2, paddle->x - paddle->len/2, ball->x))
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
    struct _ball ball= {LEDPORT%LCD_MAXX, LEDPORT %LCD_MAXY/*& 0x1F*/, -1, -1};
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
