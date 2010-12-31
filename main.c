#include <irq.h>
#include "config.h"
#include "hw.h"

#include "utils.h"
#include "terminal.h"
#include "uart.h"

#include "pwm-motor.h"

int main(void)
{
	sysInit();
    initVIC(dummyISR);

    FIO0DIR = PIN_LED |
        (1<<25);
    FIO0CLR = 0xFFFFFFFF;
    FIO0SET = 0x00000000;

    tInit(1);
	tPuts(EOLN "DRV8402 demo" EOLN);
	tPuts("(c) 2009-2011, Ivan A-R <ivan@tuxotronic.org>" EOLN);
 	tPuts("http://tuxotronic.org/" EOLN);

    tPuts(EOLN "Terminal ready." EOLN);
    tPuts("Type 'help' to show short help" EOLN EOLN);

    pwmInit(128);
    pwmRun(0, 0, 0);
    enableIRQFIQ();

    int speed = 1000;
    for(;;)
    {
        if(speed < 33000)
        {
            pwmSetSpeed(speed);
            if(speed < 4000)
                speed += 2;
            else if(speed < 15000)
                speed += 4;
            else
                speed += 8;
        }
        else
            LED_ON();
        delay_ms(1);
        /*
        pwmSet(getsin(a), getsin(a+43), getsin(a+85));
        a++;
        a &= 127;
        delay_us_(d);
        if(d>1200) d-=1;
            else LED_ON();
        */
        //tProcess();
        
    }

	return 0;	
}

/**
 * Dummy default interrupt handler (ISR).
 */
void dummyISR(void)
{
  //do something... for example print some serious error message
}


