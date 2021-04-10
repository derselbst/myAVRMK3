#include <stdio.h>
#include <avr/interrupt.h>
#include "time.h"
#include "common.h"
#include "lcd.h"
#include "button.h"
#include "menu.h"

// global clock in seconds
volatile uint32_t time=0;

// becomes true whenever the time has changed
volatile uint8_t tick=false;

static struct _time timeToStruct(const uint32_t time)
{
    struct _time tempTime;
	tempTime.sec    = time % 60u;
	tempTime.minute = (time / 60u) % 60u;
	tempTime.hour   = time / 60u / 60u;
    return tempTime;
}

static void printTimeStruct(const struct _time time)
{
    static char strTime[16];
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

	cli();
    uint32_t backupTime = time;
    sei();
	struct _time tempTime = timeToStruct(backupTime);
	printTimeStruct(tempTime);
	unsigned char * varSize=&tempTime.hour;

	while(!get_key_short(timesetmenu[0].button_mask))
	{
		if(get_key_press(1<<JOYUP))
		{
			*varSize+=1;
			printTimeStruct(tempTime);
		}
		if(get_key_short(1<<JOYDOWN))
		{
			*varSize-=1;
			printTimeStruct(tempTime);
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

	tempTime.minute = tempTime.minute < 60u ? tempTime.minute : 0u;
	tempTime.sec    = tempTime.sec < 60u ? tempTime.sec : 0u;
	tempTime.hour   = tempTime.hour < 24u ? tempTime.hour : 0u;
    
    backupTime = tempTime.hour * 60u * 60u;
    backupTime+= tempTime.minute * 60u;
    backupTime+= tempTime.sec;
    
	cli();
    time = backupTime;
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
            printTimeStruct(timeToStruct(time));
        }

        if(get_key_short(timemenu[0].button_mask))
        {
            timemenu[0].task();
            printMenu(timemenu);
        }
    }
    __asm__ __volatile__ ( "nop\nnop\nnop\n");

}
