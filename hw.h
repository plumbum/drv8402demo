#ifndef __HW_H__

#include "config.h"

/* Status LED */
#define PIN_LED (1<<26)

/* DRV */
#define PIN_PWM_A (1<<8)
#define PIN_PWM_B (1<<7)
#define PIN_PWM_C (1<<9)
#define PIN_PWM_D (1<<3)

#define PIN_RESET_ABn (1<<6)
#define PIN_RESET_CDn (1<<4)


/* Routines */
#define LED_ON()  FIO0SET = PIN_LED
#define LED_OFF() FIO0CLR = PIN_LED

#define DRV_RESET_ON()  FIO0CLR = PIN_RESET_ABn | PIN_RESET_CDn
#define DRV_RESET_OFF() FIO0SET = PIN_RESET_ABn | PIN_RESET_CDn

#endif /* __HW_H__ */

