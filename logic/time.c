#include <stdio.h>
#include <avr/interrupt.h>
#include "time.h"
#include "common.h"
#include "lcd.h"
#include "button.h"
#include "menu.h"

volatile struct _time time={0,0,0,0};
extern volatile bool tick;

static void printTime(const struct _time time)
{
    char strTime[14];
    sprintf(strTime, "%02u:%02u:%02u Uhr",time.hour,time.minute,time.sec);
    MK3_LCD_STRING_PX(10,LCD_MAXY-20, strTime);
}

static void setClock()
{
	const struct action timesetmenu[]=
	{
		{"JoyPUSH: zurueck", (void(*)(void))NULL, (1 << JOYPUSH)},
		{"", (void(*)(void))NULL, 0}
	};
	printMenu(timesetmenu);

	struct _time tempTime;
	cli();
	tempTime.minute = time.minute;
	tempTime.sec    = time.sec;
	tempTime.hour   = time.hour;
	tempTime.wday   = time.wday;
	sei();
	printTime(tempTime);
	unsigned char * varSize=&tempTime.hour;

	while(!get_key_short(timesetmenu[0].button_mask))
	{
		if(get_key_state(1<<JOYUP))
		{
			*varSize+=1;
			printTime(tempTime);
		}
		if(get_key_short(1<<JOYDOWN))
		{
			*varSize-=1;
			printTime(tempTime);
		}
		if(get_key_short(1<<JOYRIGHT))
		{
			if(varSize==&tempTime.hour)
			{
				varSize=&tempTime.minute;
			}
			else if(varSize==&tempTime.minute)
			{
				varSize=&tempTime.sec;
			}
		}
		if(get_key_short(1<<JOYLEFT))
		{
			if(varSize==&tempTime.minute)
			{
				varSize=&tempTime.hour;
			}
			else if(varSize==&tempTime.sec)
			{
				varSize=&tempTime.minute;
			}
		}
	}

	cli();
	time.minute = tempTime.minute < 60 ? tempTime.minute : 0;
	time.sec    = tempTime.sec < 60 ? tempTime.sec : 0;
	time.hour   = tempTime.hour < 24 ? tempTime.hour : 0;
	time.wday   = tempTime.wday < 7 ? tempTime.wday : 0;
	sei();
}

void timeLoop()
{
	const struct action timemenu[]=
	{
		{"Key1: setclock", &setClock, (1 << KEY0)},
		{"Key3: Hauptmenü", (void (*)(void))NULL, (1 << KEY2)},
		{"", (void (*)(void))NULL, 0}
	};
    printMenu(timemenu);

	// due to tick we cant use getUserInput() here
    while(!get_key_short(timemenu[1].button_mask))
    {
        if(tick)
        {
            // uncritical, no need to disable interrupts here
            tick=false;

            printTime(time);
        }

		if(get_key_short(timemenu[0].button_mask))
		{
			timemenu[0].task();
			printMenu(timemenu);
		}
    }
    __asm__ __volatile__ ( "nop\nnop\nnop\n");

}
