
#include "battery.h"
#include "apm32f00x_gpio.h"
#include "main.h"

static uint8_t counter = 0;

volatile static uint16_t  batVolt = 0;

static uint32_t ADC_Val=0 ;


uint16_t getBATvolt(void){

    //------------------Battery Volt--------------------
	  if(counter <  AVG_NUM){	
				
		  ADC_Val += ADC_read(BAT_VOLTS_CHAN); // channel 
			counter++;
			//SerialPrint_NumString("ADC_Val", ADC_Val );
    }else{			
			//SerialPrint_NumString("ADC_Val sum", ADC_Val );
      //SerialPrint_NumString("COUNT", counter );			
			ADC_Val = ADC_Val/counter;
			
			batVolt = ((ADC_Val* 310)/4095);     //39.7k+10k------10k
			
      //batVolt = ((ADC_Val* 300)/4095);     //39.7k+12k------10k			
			//batVolt = ((ADC_Val* 350)/4095);   //68k/1
			batVolt = batVolt + BAT_volt_Offset;
			ADC_Val = 0;
			counter = 0;
    }

   return batVolt;
}



