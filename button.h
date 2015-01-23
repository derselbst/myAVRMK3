#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

uint8_t get_key_press( uint8_t key_mask );
uint8_t get_key_rpt( uint8_t key_mask );
uint8_t get_key_state( uint8_t key_mask );
uint8_t get_key_short( uint8_t key_mask );
uint8_t get_key_long( uint8_t key_mask );


#endif // BUTTON_H_INCLUDED
