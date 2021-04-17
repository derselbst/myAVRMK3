#include <avr/io.h>
#include <avr/interrupt.h>

#include "common.h"

// debounced and inverted key state
// bit = 1: key pressed
volatile uint8_t key_state;

// contains all keys that were detected being pressed
volatile uint8_t key_press;

// key long press and repeat
volatile uint8_t key_rpt;

ISR( TIMER0_OVF_vect )                            // every 10ms
{
    static uint8_t ct0, ct1, rpt;
    uint8_t i;

    TCNT0 = (uint8_t)(int16_t)-(F_CPU / (1024.0 * 1024) + 0.5);  // preload for 10ms

    i = key_state ^ ~KEY_PIN;                       // key changed ?
    ct0 = ~( ct0 & i );                             // reset or count ct0
    ct1 = ct0 ^ (ct1 & i);                          // reset or count ct1
    i &= ct0 & ct1;                                 // count until roll over ?
    key_state ^= i;                                 // then toggle debounced state
    key_press |= key_state & i;                     // 0->1: key press detect

    if( (key_state & REPEAT_MASK) == 0 )            // check repeat function
        rpt = REPEAT_START;                         // start delay
    if( --rpt == 0 )
    {
        rpt = REPEAT_NEXT;                          // repeat delay
        key_rpt |= key_state & REPEAT_MASK;
    }
}

///////////////////////////////////////////////////////////////////
//
// check if a key has been pressed. Each pressed key is reported
// only once but will be cleared if this function returns true
//
uint8_t get_key_press( uint8_t key_mask )
{
    cli();                                          // read and clear atomic !
    key_mask &= key_press;                          // read key(s)
    // if key_mask is != 0, a key that we were asking for was pressed --> clear
    // all keys pressed since now
    // else: the key we were asking for was not pressed --> leave key_press as it is
    if(key_mask)
    {
        key_press=0;
    }
    // since key_mask in this case is zero, key_press^key_mask will always result in key_press
    //else
    //{
    //    key_press ^= key_mask;
    //}
    sei();
    return key_mask;
}

///////////////////////////////////////////////////////////////////
//
// check if a key has been pressed long enough such that the
// key repeat functionality kicks in. After a small setup delay
// the key is reported being pressed in subsequent calls
// to this function. This simulates the user repeatedly
// pressing and releasing the key.
//
uint8_t get_key_rpt( uint8_t key_mask )
{
    cli();                                          // read and clear atomic !
    key_mask &= key_rpt;                            // read key(s)
    key_rpt ^= key_mask;                            // clear key(s)
    sei();
    return key_mask;
}

///////////////////////////////////////////////////////////////////
//
// check if a key is pressed right now
//
uint8_t get_key_state( uint8_t key_mask )
{
    key_mask &= key_state;
    return key_mask;
}

///////////////////////////////////////////////////////////////////
//
uint8_t get_key_short( uint8_t key_mask )
{
    cli();                                          // read key state and key press atomic !
    return get_key_press( ~key_state & key_mask );
}

///////////////////////////////////////////////////////////////////
//
uint8_t get_key_long( uint8_t key_mask )
{
    return get_key_press( get_key_rpt( key_mask ));
}
