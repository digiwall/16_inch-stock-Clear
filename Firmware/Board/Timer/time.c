#include "time.h"

static uint16_t lastTime = 0;
static uint16_t currentTime = 0;
static uint16_t sec_diff = 0;

static uint16_t milis_lastTime = 0;
static uint16_t milis_currentTime = 0;
volatile uint16_t sleepDelay = 0; 
volatile uint16_t 	battery_sec = 0; 
volatile uint16_t  btnTim_sec = 0;

second btnTim, battery, sleep_delay; //instance of second struct
Time_mili natural,GreenLed;

volatile uint16_t lowCharg_milis = 0;

/////////////////////////////////////////////////////


volatile uint16_t miliseconds = 0;      // global variable storing milis value


///////////////////////////////////////////////////

void sec(void);

////////////////////////////////////////////////////////////////



void second_tick(void){
		
	  currentTime = miliseconds;		
		
		sec_diff = currentTime - lastTime;
		
	  if(lastTime  >= currentTime){		                    // it activates while "miliseconds" variable overflow
		  if(currentTime >= (lastTime - 64536)){//(1000 - (65536 - lastTime))){		// 1000ms to 1 second, 65536 is last value to go overflow of "miliseconds" variable		    
				lastTime = currentTime ;                        //store current time				
				sec();
		  }
	  }
		
		else if(sec_diff >= 985) // 1000ms to 1 second
		//if(sec_diff >= 985)
    {      
      lastTime = currentTime - sec_diff + 985; //(sec_diff - 985);               //store current time
			
			sec();
    } 
	
	/*
		sec_diff = currentTime - lastTime;
		if(sec_diff < 0 || lastTime > currentTime){ sec_diff = 0;}
		
	  if(lastTime  >= currentTime){		                    // it activates while "miliseconds" variable overflow
		  if(currentTime >= (lastTime - 64536)){//(1000 - (65536 - lastTime))){		// 1000ms to 1 second, 65536 is last value to go overflow of "miliseconds" variable		    
				lastTime = currentTime ;                        //store current time				
				sec();
		  }
	  }
		
		else if(sec_diff >= 985) // 1000ms to 1 second
		//if(sec_diff >= 985)
    {      
      lastTime = currentTime - sec_diff + 985; //(sec_diff - 985);               //store current time
			
			sec();
    } */	
	
}


 void sec(void){
				
				btnTim_sec++;		
			  battery_sec++;
	      sleep_delay.sec++;
	      sleepDelay++;		    
    }
		
		
void milis_tick(uint8_t reset){
	
	  uint8_t milis_diff = 0;

	  milis_currentTime = miliseconds;
		
		milis_diff = milis_currentTime - milis_lastTime;
		
		if(milis_diff >= 1)   // 1ms 
    {      
      milis_lastTime = milis_currentTime ;               //store current time
			
			natural.miliSec = natural.miliSec + milis_diff + 1;	
			GreenLed.miliSec = GreenLed.miliSec + milis_diff + 1;	
      lowCharg_milis = lowCharg_milis + milis_diff + 1;			
			
			if(reset == 1){GreenLed.miliSec = 0;}
			if(reset == 2){natural.miliSec = 0;}
    }
}




void TMR4Init(void)
{
    /**  Divider = 7, counter = 0xff  */
    TMR4_ConfigTimerBase(3, 0XFF);
    /**  Enable update interrupt  */
    TMR4_EnableInterrupt(TMR4_INT_UPDATE);
    /**  Enable TMR4  */
    TMR4_Enable();

    NVIC_EnableIRQRequest(TMR4_IRQn, 0X02);
	  
}


