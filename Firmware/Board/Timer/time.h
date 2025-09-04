

#ifndef __MILIS_H_
#define __MILIS_H_ 



#include "apm32f00x_usart.h"
#include "stdio.h"
#include "apm32f00x_tmr4.h"
#include "apm32f00x_eint.h"
#include "apm32f00x_misc.h"


#define lightTim_rst  1
#define natureTim_rst 2
#define btnTimer_rst  3



/////////////////////////////////////////////////////////////////////////

extern volatile uint16_t sleepDelay, battery_sec, btnTim_sec;
extern volatile uint16_t lowCharg_milis;

typedef struct {	
          uint16_t sec;				 
}second;


extern  second btnTim, battery, sleep_delay;

typedef struct {	
         uint32_t miliSec;				 
}Time_mili;


extern Time_mili natural,GreenLed;

///////////////Timer//////////////////////////

extern volatile uint16_t miliseconds;      


void TMR4Init(void);



void second_tick(void);
void milis_tick(uint8_t reset);

#endif
