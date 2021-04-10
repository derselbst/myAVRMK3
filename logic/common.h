#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <stdbool.h>

#define KEY_DDR         DDRK
#define KEY_PORT        PORTK
#define KEY_PIN         PINK
#define KEY0            0
#define KEY1            1
#define KEY2            2
#define JOYUP           3
#define JOYLEFT         4
#define JOYPUSH         5
#define JOYRIGHT        6
#define JOYDOWN         7
#define KEY3            JOYUP
#define KEY4            JOYLEFT
#define KEY5            JOYPUSH
#define KEY6            JOYRIGHT
#define KEY7            JOYDOWN
#define ALL_KEYS        (1<<KEY0 | 1<<KEY1 | 1<<KEY2)

#define REPEAT_MASK     (1<<KEY1 | 1<<KEY2)       // repeat: key1, key2
#define REPEAT_START    50                        // after 500ms
#define REPEAT_NEXT     20                        // every 200ms


uint8_t findfirstbitset(uint8_t x);
bool onlyonebitset(uint8_t x);
void waitMs(uint16_t ms);


#endif // COMMON_H_INCLUDED
