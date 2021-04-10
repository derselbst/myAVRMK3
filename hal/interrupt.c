#include <avr/interrupt.h>
//#include <util/delay.h>

//#include "lcd.h"
#include "common.h"
#include "7segfont.h"
#include "time.h"
#include "led.h"

// the number of read tests to buttons
#define DEBOUNCE_TESTS 4

#undef DEBUG

// this interrupt is triggered twice: when a key is pressed and when its released
ISR(PCINT2_vect)
{
    static uint8_t oldmask = 0xFF; // 0xff due to pull up resistors

//    uint8_t keyPressed[DEBOUNCE_TESTS];

    uint8_t keyPressed;
    keyPressed = PINK ^ oldmask;
    LEDPORT = keyPressed;

#ifdef DEBUG
    MK3_LCD_CLEAR();
    char buf[30];
    sprintf(buf, "PINK:     0x%x",PINK);
    MK3_LCD_STRING_PX(0,0, buf);
    sprintf(buf, "oldmask:  0x%x",oldmask);
    MK3_LCD_STRING_PX(0,10, buf);
    sprintf(buf, "keyPd[0]: 0x%x",keyPressed[0]);
    MK3_LCD_STRING_PX(0,20, buf);
#endif

    // keyPressed[0] should contain the button that triggered the interrupt
    // since it was recorded just right after entering the ISR
    if (!keyPressed)
    {
        // no key was pressed?! thus its unclear what actually triggered the interrupt
        // return immediately
        // should never happen
        // this also prevent division by zero in the following
        return;
    }

    // check if keyPressed[0] has only one bit set to one
    if (!onlyonebitset(keyPressed))
    {
        // more than one bit set, unclear what actually triggered the interrupt
        return;
    }

    // debounce button: therefore check the state of the button 4 times
    /*    uint8_t i;
        for (i = 1; i < DEBOUNCE_TESTS; i++)
        {
            keyPressed[i] = PINK ^ oldmask;

             _delay_us(150);
        }

        uint16_t sum = 0;
        for (i = 0; i < DEBOUNCE_TESTS; i++)
        {
            sum += keyPressed[i];
        }

        // the following calculation could also be done using float, although this would be more expensive
        // thus calculate it in percents (sum*100)
        // keyPressed[0] because it contains the button that triggered the interrupt
        uint16_t probabilityforkeypressed = (sum * 100) / (DEBOUNCE_TESTS * keyPressed[0]);
        if (probabilityforkeypressed <= 50)
        {
            // key was probably not pressed
            return;
        }
    */

#ifdef DEBUG
    sprintf(buf, "probab: %d",probabilityforkeypressed);
    MK3_LCD_STRING_PX(0,30, buf);
#endif
    // was button pressed or released?
    // check if the bit of oldmask was already set or not
    if (oldmask & keyPressed)
    {
        // key was just pressed
        // clear bit from old mask
        oldmask &= ~keyPressed;
    }
    else
    {
        // fire event

#ifdef DEBUG
        MK3_LCD_STRING_PX(0,50, "EVENT!");
#endif

        // key was just released
        oldmask |= keyPressed;
    }

#ifdef DEBUG
    sprintf(buf, "newoldmask:  0x%x",oldmask);
    MK3_LCD_STRING_PX(0,40, buf);
#endif

}

// Interrupt-Routine
// Timer1 Ueberlauf
ISR (TIMER1_OVF_vect)
{
    static uint8_t index=0;
    static uint8_t wert=1;

    //output the character, at position of index in the font, on port-B
    PORTB=font7seg[index];
    //output the actual value on the LEDs
    LEDPORT=wert;

    //increase the counter by one
    if(wert==240)
    {
        index++;
        wert=0;
    
		//if the counter is higher as the count of characters in the font, set counter to the first charcater
		if(index==FONT7segLen)
		{
			index=0;
		}
	}
    time++;
    wert++;
	// start value of timer 1
    TCNT1 = 3036;

    tick=true;
}
