#include <avr/io.h>
#include <avr/interrupt.h>
#include "led.h"

static void initInterrupts()
{

    // Enable external interrupt INT0
    // EIMSK |= 1 << INT0;

    // The rising edge of INT0 generates asynchronously an interrupt request
    // EICRA |= (1 << ISC01) | (1 << ISC00);

    // enable Pin Change Interrupt 2 (contains PCINT23 to PCINT16)
//    PCICR |= 1 << PCIE2;

    // enable PCINT23 to PCINT16, which on the mk3 board corresponds to PORTK
//    PCMSK2 = 0xFF;


    //start value of the timers
    //to get preferably exact one second: start_value=2^16-(16000000/(PRESCALER)) with Prescaler == 256
    //general: time_between_two_overflows=(2^(bitlenght_of_time)*PreScaler)/System_Clock
    TCNT1 = 3036;

    // Overflow Interrupt erlauben
    TIMSK1 |= (1<<TOIE1);

    //-------------------------------------------------------------------------------
    // Timer 1 konfigurieren und starten
    TCCR1B = (1<<CS12); // Prescaler 256, da bit von Clock Select 2 gesetzt
    //-------------------------------------------------------------------------------

    TCCR0B = (1<<CS02)|(1<<CS00);         // divide by 1024
    TCNT0 = (uint8_t)(int16_t)-(F_CPU / 1024 * 10e-3 + 0.5);  // preload for 10ms
    TIMSK0 |= 1<<TOIE0;                   // enable timer interrupt

	// Enable global interrupts
    // this also set bit 7 of SREG to 1, indicating that Global Interrupts are enabled
    sei();
}

void init()
{
    //config port-B as output, (for the 7-segment-display)
    DDRB=0xFF;
    PORTB=0b00111111;   //gib schon mal auf der 7 seg anzeige ne null aus

    //3leds an port d
    DDRD=0xFF;
    PORTD=0x00;     //leds an port d aus

    // set PORTK as input
    DDRK = 0x00;
    //pull up resistors
    PORTK= 0xFF;

    //config port-L as output (for the LEDs)
    DDRL=0xff;
    LEDPORT=0x00;

    //config port-C as input whitout pullup (for LCD-Data)
    //port-C must be config as output while reading and output while writing
    DDRC=0x00;
    PORTC=0;

    //config port-A as output (for LCD-Control)
    //   bit0 = CS = chip-select low-activ
    //   bit1 = R/W = low=write
    //   bit2 = RS = low=control, high=data
    //   bit3 = E = writing edge H->L
    //   bit4 = Reset = low-aktiv
    DDRA=0xff;
    PORTA=0b1100011;

#ifdef HAVE_LEDADDON
    DDRJ=0xFF;
#endif
    LEDADDONPORT=0b00000000;

    initInterrupts();
}