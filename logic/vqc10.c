#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay_basic.h>
#include <util/parity.h>
#include "time.h"
#include "common.h"
#include "lcd.h"
#include "button.h"
#include "menu.h"

// given a xtal speed of 16MHZ this should make us wait for about 100 us
static uint16_t waitCycles = 533;
static char* stringToSend = "Hallo Welt";

static void send(unsigned char x)
{
//     char parBit = parity_even_bit(x);
    uint16_t sendbuf = 0; // init with zero, will be used for stop bits
    sendbuf |= x; // store in the actual data into lowest 8 bit
    sendbuf <<= 1; // make space for start bit
    sendbuf |= 0x01; // start bit
    
    enum { BITLEN = 8+1+2 };
    
    cli();
    for(unsigned char i=0; i < BITLEN; i++)
    {
        PORTG =
        (
            (
                ( sendbuf & 0x01 )
                != 0
            )
            ? 0xFFu
            : 0x00u
        );
        sendbuf >>= 1;
        
        // we need to send 1 bit every 128 us
        // because external VQC10 clock is 8 MHz
        // PRE0 is 1
        // T0 is believed to be 8 (given that I have the correct Z80 assembly document in my hand)
        // BR = 8000kHz / (128 * PRE0 * T0 ) =  7.8125 kBits/s = 7812.5 Bits/s
        // which makes 1 bit every 128 us
        
        _delay_loop_2(waitCycles);
    }
    sei();
}

void sendLoop()
{
    MK3_LCD_CLEAR();
    MK3_LCD_STRING_PX(10,LCD_MAXY-50, "Sending:");
    MK3_LCD_STRING_PX(10,LCD_MAXY-40, stringToSend);
    
    for(unsigned char i=0; stringToSend[i] != '\0' && i < 255; i++)
    {
        send(stringToSend[i]);
    }
    
    MK3_LCD_STRING_PX(10,LCD_MAXY-20, "Fertig");
    waitMs(500);
}

static void sendParity(unsigned char x)
{
    unsigned char isOdd = parity_even_bit(x);
    send( (isOdd << 7) | x);
}

static void sendParityLoop()
{
	MK3_LCD_CLEAR();
    MK3_LCD_STRING_PX(10,LCD_MAXY-50, "Sending with Parity:");
    MK3_LCD_STRING_PX(10,LCD_MAXY-40, stringToSend);
    
    for(unsigned char i=0; stringToSend[i] != '\0' && i < 255; i++)
    {
        sendParity(stringToSend[i]);
    }
    
    MK3_LCD_STRING_PX(10,LCD_MAXY-20, "Fertig");
    waitMs(500);
}

static void toogle()
{
    PORTG ^= 0xFF;
}


static void printWaitCycles()
{
    static char strTime[20];
    uint16_t duration = waitCycles * 3 / (F_CPU / 1000 / 1000);
    snprintf(strTime, sizeof(strTime), "%u Iter = %u us",waitCycles, duration);
    MK3_LCD_STRING_PX(5,LCD_MAXY-20, strTime);
}

static void setWaitCycles()
{
	static const struct action menu[]=
	{
		{"Key3: zurueck", (void (*)(void))NULL, (1 << KEY2)},
		{"", (void(*)(void))NULL, 0}
	};
	printMenu(menu);
    
    while(!get_key_short(menu[0].button_mask))
	{
		if(get_key_state(1<<JOYUP) || get_key_press(1<<KEY0))
		{
			waitCycles++;
		}
		if(get_key_state(1<<JOYDOWN) || get_key_press(1<<KEY1))
		{
			waitCycles--;
		}
		printWaitCycles();
        _delay_loop_2(533);
	}
}

static void changeString()
{
    if(stringToSend[0] == 'A')
    {
        stringToSend = "Hallo Welt";
    }
    else
    {
        stringToSend = "A";
    }
}

void vcq10Main()
{
	static const struct action menu[]=
	{
 		{"UP: Say Hi", sendLoop, (1 << JOYUP)},
		{"Down: Say Hi w Parity", &sendParityLoop, (1 << JOYDOWN)},
		{"Left: Set Wait Dur", &setWaitCycles, (1 << JOYLEFT)},
 		{"KEY1: Toggle PORTG", toogle, (1 << KEY0)},
 		{"KEY2: Change String", changeString, (1 << KEY1)},
		{"Key3: Hauptmenü", (void (*)(void))NULL, (1 << KEY2)},
		{"", (void (*)(void))NULL, 0}
	};
    printMenu(menu);
    
    getUserInput(menu, (1 << KEY2));
}
