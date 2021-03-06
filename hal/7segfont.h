#ifndef SEGFONT_H_INCLUDED
#define SEGFONT_H_INCLUDED

#include <stdint.h>

static const uint8_t font7seg [] =
{
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b01110111, // A
    0b01111100, // B
    0b00111001, // C
    0b01011110, // D
    0b01111001, // E
    0b01110001, // F
    0b01000000, // -
    0b01001001,  // 3 waagerechte
    0b10111111, // 0.
    0b10000110, // 1.
    0b11011011, // 2.
    0b11001111, // 3.
    0b11100110, // 4.
    0b11101101, // 5.
    0b11111101, // 6.
    0b10000111, // 7.
    0b11111111, // 8.
    0b11101111, // 9.
    0b11110111, // A.
    0b11111100, // B.
    0b10111001, // C.
    0b11011110, // D.
    0b11111001, // E.
    0b11110001, // F.
    0b11000000, // -.
    0b11001001,  // 3 waagerechte.
};

#endif // SEGFONT_H_INCLUDED
