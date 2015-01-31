#include <avr/io.h>
#include <stdio.h>
#include "setup.h" // init()
#include "common.h" // waitMS()
#include "gameoflife.h" // :D
#include "MK3_2560_LCD.h" // LCD_LIGHT()
#include "time.h" // timeLoop()
#include "menu.h" // printMenu()
#include "lcd.h"
#include "led.h"

void breakout();

int main(void)
{
    init();

    PORTD=0b00000001;   //rote led an
#ifdef HAVE_LEDADDON
    LEDADDONPORT= (1 << 1);   //rot an
#endif

    MK3_INIT_LCD();
    waitMs(100);
    MK3_LCD_CLEAR();
    waitMs(500);

    MK3_LCD_LIGHT(true);

    PORTD=0b00000011;   //rote und gelbe LED an
#ifdef HAVE_LEDADDON
    LEDADDONPORT=(1 << 3);    // mach blau an
#endif

    const struct action mainmenu[]=
    {
        {"Key1: Licht an/aus", &toogleLCDLight, (1 << KEY0)},
        {"Key2: Disp. dunkel", &toogleLCDDisp, (1 << KEY1)},
        {"Key3: GameOfLife", &GameOfLife, (1 << KEY2)},
        {"JoyUp: Uhr", &timeLoop, (1 << JOYUP)},
        {"JoyLEFT: GofL Set", &GameOfLifeSet,(1 << JOYLEFT)},
        {"JoyRIGHT: Breakout", &breakout,(1 << JOYRIGHT)},
        {"", (void (*)(void))NULL, 0} // terminal
    };

    printMenu(mainmenu);
    PORTD=0b00000100;
#ifdef HAVE_LEDADDON
    LEDADDONPORT=(1 << 2);
#endif

    getUserInput(mainmenu, (1 << JOYPUSH));


    return 0;
}
