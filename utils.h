#ifndef _UTILS_H_
#define _UTILS_H_

#include <inttypes.h>
#include "config.h"

#define sysTimer() T1TC

extern const int power10[9];
char* ultoa(uint32_t val, char* dest);
char* ltoa(int32_t val, char* dest);


#define TUNE_FAC 0.96154

//#define delay_us(delayval) delay_us_( (uint32_t)( (FOSC*PLL_MUL) / (1000000/(delayval)) /2 ) ) 
#define delay_us(delayval) delay_us_( (uint32_t)( (FOSC*PLL_MUL) / (1000000/(delayval)) /4 * TUNE_FAC) ) 
#define delay_ms(delayval) delay_us_( (uint32_t)( (FOSC*PLL_MUL) / (1000/(delayval)) /4 * TUNE_FAC) ) 
void delay_us_(uint32_t delayval);


inline static void _PINSEL(const int bit, const int val)
{
    if(bit < 16)
        PINSEL0 = (PINSEL0 & ~(3<<(2*bit))) | ((val & 3) << (2*bit));
    else
        PINSEL1 = (PINSEL1 & ~(3<<(2*(bit-16)))) | ((val & 3) << (2*(bit-16)));
}

#endif /* _UTILS_H_ */

