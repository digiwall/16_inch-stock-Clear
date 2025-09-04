#include "delay.h"


void delay_us(unsigned int  value)
{
register unsigned int loops =  ((RCM_GetMasterClockFreq()/24000000)*value) ;
	
	while(loops)
	{	
		__ASM("nop");
		loops--;
	};
}


void delay_ms(unsigned int  value)
{
	while(value)
	{
		delay_us(2400);
		value--;
	};
}

void delay_ms1(unsigned int  value)
{
    volatile uint32_t delay = 0xffff5;

    while(delay--)
    {}
}