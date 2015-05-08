#include <stdint.h>
#include "lcd.h"
#include "wall.h"

void printWall(const struct _wall *const wall, DRAWTYP m)
{
    uint8_t i;

    // draw left and right wall
    for (i=wall->top; i< LCD_MAXX; i++)
    {
        MK3_LCD_PIXEL_AT(wall->left, i, m);
        MK3_LCD_PIXEL_AT(wall->right, i, m);
    }

    // draw top wall
    for (i=wall->left+1; i< wall->right; i++)
    {
        MK3_LCD_PIXEL_AT(i, wall->top, m);
    }
}
