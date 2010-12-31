#include "irq.h"

#include <LPC2103.h>

void initVIC(funcISR_t* pDefaultISR)
{
  VICIntSelect   = 0x00000000;         //all IRQs are assigned to IRQ (not FIQ)
  VICIntEnClear  = 0xFFFFFFFF;         //disable all interrupts
  VICSoftIntClear = 0xFFFFFFFF;         //clear all pending software interrupts
  VICProtection  = 0x00000000;         //VIC protection disabled, registers can
                                       // be accessed in User and Privileged mode
  VICVectAddr    = 0x00000000;         //clear any pending/active interrupts
  VICDefVectAddr = (uint32_t)pDefaultISR;  //set default ISR address for for non-vectored IRQs
  
  //reset/disable all vectored IRQ slots
  VICVectCntl0   = 0x00000000;
  VICVectCntl1   = 0x00000000;
  VICVectCntl2   = 0x00000000;
  VICVectCntl3   = 0x00000000;
  VICVectCntl4   = 0x00000000;
  VICVectCntl5   = 0x00000000;
  VICVectCntl6   = 0x00000000;
  VICVectCntl7   = 0x00000000;
  VICVectCntl8   = 0x00000000;
  VICVectCntl9   = 0x00000000;
  VICVectCntl10  = 0x00000000;
  VICVectCntl11  = 0x00000000;
  VICVectCntl12  = 0x00000000;
  VICVectCntl13  = 0x00000000;
  VICVectCntl14  = 0x00000000;
  VICVectCntl15  = 0x00000000;
}

void enableIRQFIQ(void)
{
   //enable interrupts (both IRQ and FIQ) 
   asm volatile ("mrs r3, cpsr       \n\t"                          
                 "bic r3, r3, #0xC0  \n\t"
                 "msr cpsr, r3       \n\t"
                 :
                 :
                 : "r3" );
}

int registerIRQ(funcISR_t* pISR, uint8_t inum, uint8_t slot)
{
    if(slot>=16)
        return -1;
    if(inum>=32)
        return -1;
    if(((&VICVectCntl0)[slot] & 0x20) != 0)
        return -1;
    VICIntSelect &= ~1<<inum;
    (&VICVectAddr0)[slot] = (uint32_t)pISR;
    (&VICVectCntl0)[slot] = inum | 0x20;
    VICIntEnable |= 1<<inum;
    return 0;
}

int unregisterIRQ(uint8_t slot)
{
    if(slot>=16)
        return -1;
    if(((&VICVectCntl0)[slot] & 0x20) == 0)
        return -1;
    uint8_t inum = (&VICVectCntl0)[slot] & 0x1F;
    VICIntEnable &= ~1<<inum;
    (&VICVectCntl0)[slot] = 0;
    (&VICVectAddr0)[slot] = 0;
    return 0;
}

#if 0
initTimer0(void (*pISR)(), void (*pCallback)())

  //initialize VIC for Timer0 interrupts
  VICIntSelect &= ~0x10;       //Timer0 interrupt is assigned to IRQ (not FIQ)
  VICVectAddr4  = (tU32)pISR;  //register ISR address
  VICVectCntl4  = 0x24;        //enable vector interrupt for timer0
  VICIntEnable  = 0x10;        //enable timer0 interrupt
  
/**
 * Dummy default interrupt handler (ISR).
 */
static void dummyISR(void)
{
  //do something... for example print some serious error message
}
#endif

