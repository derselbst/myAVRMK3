#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "lcd.h"
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

void getUserInput2(const struct action a[], const uint8_t breakButton, void(*alwaysTask)(void))
{
	uint8_t i=0;
	while(!get_key_press(breakButton))
	{
        if(alwaysTask)
        {
            alwaysTask();
        }
        
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

void getUserInput(const struct action a[], const uint8_t breakButton)
{
    getUserInput2(a, breakButton, NULL);
}
