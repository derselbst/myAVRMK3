#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "MK3_2560_LCD.h"
#include "menu.h"
#include "button.h"

void printMenu(const struct action a[])
{
	MK3_LCD_CLEAR();
	uint8_t i;
	for(i = 0; a[i].msg[0] /*check if the string contains at least one letter*/; i++)
	{
		MK3_LCD_STRING_PX(0, i*9, a[i].msg);
	}
}

void getUserInput(const struct action a[], const uint8_t breakButton)
{
	uint8_t i=0;
	while(!get_key_press(breakButton))
	{
		if(get_key_press( a[i].button_mask ))
		{
			if(a[i].task)
			{
				a[i].task();
			}
			printMenu(a);
		}

		i++;
		if (!(a[i].msg[0]))
		{
			i=0;
		}
	}
}
