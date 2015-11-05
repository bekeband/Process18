
#include <htc.h>
#include "delay.h"

uint16_t dly;

//delay time = (cntr*2)+3 (in us for 20MHz)
//FULL OPTIMIZATION of C compilator(10 level, enable post-pass optimisation)
void Delay(uint16_t cntr) {
	for (dly=0; dly!=cntr; dly++) {
		asm("nop");
		asm("nop");
		asm("nop");
	}
}

//delay time is ~ 1ms
void DelayMs (uint16_t e)
{
	while(--e) {
		Delay(DELAY_1000us);
	 }
}

