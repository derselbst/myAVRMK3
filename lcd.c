#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lcd.h"
#include "MK3_2560_LCD.h"

void plot(uint8_t x, uint8_t y)
{
    MK3_LCD_PIXEL_AT(x, y, MODE_SET);
}

void drawline(point p0, point p1)
{
    int16_t dx = abs(p1.x - p0.x), sx = p0.x < p1.x ? 1 : -1;
    int16_t dy = -abs(p1.y - p0.y), sy = p0.y < p1.y ? 1 : -1;
    int16_t err = dx + dy, e2; /* error value e_xy */

    for (;;)   /* loop */
    {
        plot(p0.x, p0.y);
        if (p0.x == p1.x && p0.y == p1.y)
        {
            break;
        }

        e2 = 2 * err;
        if (e2 > dy) /* e_xy+e_x > 0 */
        {
            err += dy;
            p0.x += sx;
        }
        if (e2 < dx) /* e_xy+e_y < 0 */
        {
            err += dx;
            p0.y += sy;
        }
    }
}

void toogleLCDLight()
{
	static bool licht = true;
	if (licht)
	{
		licht=false;
		MK3_LCD_LIGHT(licht);
	}
	else
	{
		licht = true;
		MK3_LCD_LIGHT(licht);
	}
}

void toogleLCDDisp()
{
    static bool display = false;
	if (display)
	{
		display=false;
		MK3_LCD_DARK(display);
	}
	else
	{
		display = true;
		MK3_LCD_DARK(display);
    }
}