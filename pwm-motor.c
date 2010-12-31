#include "pwm-motor.h"

#include <config.h>
#include <stdint.h>
#include <irq.h>
#include "utils.h"
#include "sin.h"


static void timer3Irq(void) __attribute__ ((interrupt));

/*
 * Local state variables
 */
static int angle;

/*
 * Internal routines
 */

/**
 * PWM off, mark drv reset pin
 */
static inline void pwmReset(void)
{
    DRV_RESET_ON(); // Put driver in reset state
    _PINSEL(7, 0); // No PWM on pin
    _PINSEL(8, 0); // No PWM on pin
    _PINSEL(9, 0); // No PWM on pin
}


/**
 * Release drv reset pin
 */
static inline void pwmResetRelease(void)
{
    DRV_RESET_OFF();
}


/**
 * User routines
 */

void pwmInit(uint16_t period)
{
    // PINSEL0 |= (2<<(8*2)) | (2<<(7*2)); // P0.7 = MAT2.0
    pwmReset();
    angle = 0;

    FIO0CLR = PIN_PWM_A | PIN_PWM_B | PIN_PWM_C | PIN_PWM_D ; // PWM off states

    FIO0DIR |= PIN_PWM_A | PIN_PWM_B | PIN_PWM_C | PIN_PWM_D | PIN_RESET_ABn | PIN_RESET_CDn;

    TIMER2_TCR = 0x02; // Reset counter and prescaler
    TIMER2_CTCR = 0x00;
    TIMER2_PR = 0x00; // Prescaler
    TIMER2_MCR = (1<<10); // Reset on MR3
    TIMER2_EMR = 0x00;
    TIMER2_PWMCON = (1<<0) | (1<<1) | (1<<2); // Enable MAT2.0, MAT2.1 & MAT2.2 pwm output

    TIMER2_MR0 = 0;
    TIMER2_MR1 = 0;
    TIMER2_MR2 = 0;
    TIMER2_MR3 = period;

    /* Syncro counter */
    TIMER3_TCR = 0x02; // Reset counter and prescaler
    TIMER3_CTCR = 0x00;
    TIMER3_PR = 0x00; // Prescaler
    TIMER3_MCR = (1<<10) | (1<<9); // Reset & Interrupt on MR3
    TIMER3_MR3 = (PCLK / 1200);
#if 1
    TIMER3_MR0 = 10; // (PCLK / 1200)/8;
    TIMER3_PWMCON = (1<<0); // Enable MAT3.0 pwm output
    _PINSEL(21, 2); // Enable output
#else 
    TIMER3_MR0 = 0;
    TIMER3_PWMCON = 0; // Enable MAT3.0 pwm output
#endif

    TIMER3_IR = 0xFF;
    registerIRQ(&timer3Irq, 27, 7);
    TIMER3_TCR = 0x01; // Enable counting

    pwmResetRelease();
    //TIMER2_TCR = 0x01; // Enable counting
}

void pwmSetSpeed(int tickHz)
{
    TIMER3_MR3 = (PCLK / tickHz);
}

/**
 * Brake motors
 * Both power keys connect to ground, PWM off. Reset not needed.
 */
void pwmStop(void)
{
    _PINSEL(7, 0); // No PWM on pin
    _PINSEL(8, 0); // No PWM on pin
    _PINSEL(9, 0); // No PWM on pin
}

void pwmRun(uint16_t a, uint16_t b, uint16_t c)
{
    TIMER2_TCR = 0x02; // Reset counter and prescaler
    TIMER2_MR1 = a;
    TIMER2_MR0 = b;
    TIMER2_MR2 = c;
    _PINSEL(7, 2); // PWM pin on 
    _PINSEL(8, 2); // PWM pin on 
    _PINSEL(9, 2); // PWM pin on 
    TIMER2_TCR = 0x01; // Enable counting
}

void pwmSet(uint16_t a, uint16_t b, uint16_t c)
{
    TIMER2_TCR = 0x02; // Reset counter and prescaler
    TIMER2_MR1 = a;
    TIMER2_MR0 = b;
    TIMER2_MR2 = c;
    TIMER2_TCR = 0x01; // Enable counting
}

static void timer3Irq(void)
{
    FIO0SET = (1<<25);
    pwmSet(getsin(angle), getsin(angle+43), getsin(angle+85));
    angle = (angle+1) & 127;
    TIMER3_IR = 0xFF;
    VICVectAddr = 0;
    FIO0CLR = (1<<25);
}

