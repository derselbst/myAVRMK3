#include <stdint.h>
#include <stdbool.h>

#ifdef F_CPU
#if defined(MyAppCpp) || !defined(GeneratedBySisy)
 void waitMs(uint16_t ms)
 {
   while(ms--)
   {{ \
	__asm__ __volatile__ ( \
		" ldi r24,%[lo]"     "\n\t" \
		" ldi r25,%[hi]"     "\n\t" \
		" waitMs2%=:"        "\n\t" \
		"  sbiw r24,1"       "\n\t" \
		" brne waitMs2%="    "\n\t" \
		: \
		: [lo] "M" (0xFF & (F_CPU/(1000*4))), [hi] "M" ((F_CPU/(1000*4))/0x100)  \
		: "r24","r25" \
	); \
   }}
 }
#else
 extern void waitMs(uint16_t);
#endif
#else
	#warning F_CPU nicht definiert
#endif


bool onlyonebitset(uint8_t x)
{
    return (x && !(x & (x - 1)));
}

// find and return the number of the first bit set to one
// one-based counting - zero if none is set
uint8_t findfirstbitset(uint8_t x)
{
    if (x == 0)
    {
        return 0;
    }

    uint8_t i = 1, pos = 1;

    // Iterate through bits of n till we find a set bit
    // i&n will be non-zero only when 'i' and 'n' have a set bit
    // at same position
    while (!(i & x))
    {
        // try next bit by shifting right
        i = i << 1;

        // increment position
        ++pos;
    }

    return pos;
}
