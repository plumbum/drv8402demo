#include "utils.h"


/*
 * itoa functions
 */

const int power10[] = {
    1000000000,
    100000000,
    10000000,
    1000000,
    100000,
    10000,
    1000,
    100,
    10,
};

char* ultoa(uint32_t val, char* dest)
{
    int ipow;
    int power;
    char atmp;
    char prefix_flag = 0;
    for(ipow=0; ipow<(sizeof(power10)/sizeof(power10[0])); ipow++)
    {
        atmp = '0';
        power = power10[ipow];
        while(val >= power)
        {
            atmp++;
            val -= power;
        }
        if(atmp > '0')
            prefix_flag = -1;
        if(prefix_flag)
            *dest++ = atmp;
    }
    *dest++ = val+'0';
    *dest = 0;
    return dest;
}

char* ltoa(int32_t val, char* dest)
{
    if(val<0)
    {
        *dest++ = '-';
        return ultoa(-val, dest);
    }
    else
        return ultoa(val, dest);
}

/* 
   Precise Delay Functions for LPC2100
   Martin Thomas, 11/2004
*/
void delay_us_(uint32_t delayval)
{
	asm volatile (
		"L_LOOPUS_%=: 		\n\t" \
		"subs	%0, %0, #1 	\n\t" \
		"bne	L_LOOPUS_%=	\n\t" \
		:  /* no outputs */ : "r" (delayval)
	);
	
#if 0
	asm volatile (
		"ldr	r1,=%0 		\n\t" \
		".L_LOOPUS_%=: 		\n\t" \
		"subs	r1, r1, #1 	\n\t" \
		"bne	L_LOOPUS_%=	\n\t" \
		: "=w" (delayval)
		: "0" (delayval)
	);
#endif
}

