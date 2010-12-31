#ifndef _PWM_MOTOR_H_
#define _PWM_MOTOR_H_

#include <hw.h>

/**
 * Init MCU hardware
 * @param pwmConf
 */
void pwmInit(uint16_t period);

void pwmRun(uint16_t a, uint16_t b, uint16_t c);
void pwmStop(void);

void pwmSet(uint16_t a, uint16_t b, uint16_t c);

void pwmSetSpeed(int tickHz);

#endif

