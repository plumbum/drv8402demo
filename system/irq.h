#ifndef _IRQ_H_
#define _IRQ_H_

#include <inttypes.h>

#define isrWDT          0
#define isrARMCore0     2
#define isrARMCore1     3
#define isrTIMER0       4
#define isrTIMER1       5
#define isrUART0        6
#define isrUART1        7
#define isrI2C0         9
#define isrSPI0         10
#define isrSPI1         11
#define isrSPP          11
#define isrPLL          12
#define isrRTC          13
#define isrEINT0        14
#define isrEINT1        15
#define isrEINT2        16
#define isrAD0          18
#define isrI2C1         19
#define isrTIMER2       26
#define isrTIMER3       27


void dummyISR(void) __attribute__ ((interrupt));
/*
static void timer0ISR(void) __attribute__ ((interrupt));
static void initVIC(void (*pDefaultISR)());
static void initTimer0(void (*pISR)(), void (*pCallback)());
*/

typedef void (funcISR_t)(void);

/**
 *  This function initialize the Vectored Interrupt Controller (VIC) to
 *    a known state. All interrupts are vectored to a default interrupt
 *    handler.
 *
 *  @param [in] pDefaultISR - function pointer to the default interrupt ISR.
 *  @return none
 */
void initVIC(funcISR_t* pDefaultISR);

void enableIRQFIQ(void);

int registerIRQ(funcISR_t* pISR, uint8_t inum, uint8_t slot);
int unregisterIRQ(uint8_t slot);

#endif /* _IRQ_H_ */

