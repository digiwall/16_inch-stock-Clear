

#include "button.h"




void btn_init(void)
{
	 GPIO_Config_T Button_Config;
	 Button_Config.mode = GPIO_MODE_IN_FLOATING;
	
	
	#if BTN_pullUPmode == 1
		GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT); 
		GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_IN_PU_NO_IT);
		GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT);
		GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_IN_PU_NO_IT);
		
	#else
		//GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT); 
		//GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT);
		//GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);
		//GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);
	
	  
    Button_Config.pin =  btnTimer;
    GPIO_Config(Timer_Button_Port, &Button_Config);
	
	
    Button_Config.pin =  btnLight;
    GPIO_Config(Light_Button_Port, &Button_Config);
		
		

    Button_Config.pin =  btnInc;
    GPIO_Config(RMT_btn_power_button_Port, &Button_Config);
		
		
    Button_Config.pin =  btnDecrement;
    GPIO_Config(RMT_btn_power_button_Port, &Button_Config);
		
		////////////POWER BUTTON INTERUPT ENABLE/////////////
		Button_Config.mode =  GPIO_MODE_IN_FLOATING;
		Button_Config.intEn =  GPIO_EINT_ENABLE;
		
		Button_Config.pin =  btnPOWER;
		//EINT_Config(EINT_PORT_C, EINT_TRIGGER_RISING);
		
    //NVIC_EnableIRQRequest(EINTC_IRQn, 0X01);
    GPIO_Config(RMT_btn_power_button_Port, &Button_Config);
		
	#endif	
}



void button(uint8_t* KeyValue){

   uint16_t loopbreak = 0;
	 
	 #if BTN_pullUPmode == 1
			if(!(GPIO_ReadInputPin(Button_Port, btnTimer))){
		 
				while(!(GPIO_ReadInputPin(Button_Port, btnTimer)) && loopbreak <Btn_TimeOut){ loopbreak++;  delay_ms(2);}
				*KeyValue = btnValTimer;
				//remote.packet1 = 0;
     
		 
		}else if(!(GPIO_ReadInputPin(Button_Port, btnLight))){
				while(!(GPIO_ReadInputPin(Button_Port, btnLight)) && loopbreak < Btn_TimeOut){ loopbreak++;  delay_ms(2);}
				*KeyValue = btnLightValue;
				//remote.packet1 = 0;

		}else if(!(GPIO_ReadInputPin(Button_Port, btnInc))){
				while(!(GPIO_ReadInputPin(Button_Port, btnInc)) && loopbreak < Btn_TimeOut){ loopbreak++;  delay_ms(2);}
				*KeyValue = btnSpeedUp;
				//remote.packet1 = 0;
    
		}else if(!(GPIO_ReadInputPin(Button_Port, btnDecrement))){
				while(!(GPIO_ReadInputPin(Button_Port, btnDecrement)) && loopbreak < Btn_TimeOut){ loopbreak++;  delay_ms(2);}
				*KeyValue = btnSpeedDown;
       // remote.packet1 = 0;
		}else{
				*KeyValue = 0;     
		}
	#else
		if((GPIO_ReadInputBit(Timer_Button_Port, btnTimer))){
		 
				while((GPIO_ReadInputBit(Timer_Button_Port, btnTimer)) && loopbreak <Btn_TimeOut){ loopbreak++;  IWDT_ReloadCounter();    delay_ms(8);}
				*KeyValue = btnValTimer;
		 
		}else if((GPIO_ReadInputBit(Light_Button_Port, btnLight))){
				while((GPIO_ReadInputBit(Light_Button_Port, btnLight)) && loopbreak < Btn_TimeOut){ loopbreak++;  IWDT_ReloadCounter();    delay_ms(8);}
				*KeyValue = btnLightValue;

		}else if((GPIO_ReadInputBit(RMT_btn_power_button_Port, btnInc))){
				while((GPIO_ReadInputBit(RMT_btn_power_button_Port, btnInc)) && loopbreak < Btn_TimeOut){ loopbreak++;  IWDT_ReloadCounter();      delay_ms(8);}
				*KeyValue = btnSpeedUp;
    
		}else if((GPIO_ReadInputBit(RMT_btn_power_button_Port, btnDecrement))){
				while((GPIO_ReadInputBit(RMT_btn_power_button_Port, btnDecrement)) && loopbreak < Btn_TimeOut){ loopbreak++; IWDT_ReloadCounter(); delay_ms(8);}
				*KeyValue = btnSpeedDown;
     
		}else if(BTNpower_key == TRUE){
		
		   //*KeyValue = btnPowerValue;
			 // BTNpower_key = FALSE;
		}		
		else{
				*KeyValue = 0;     
		}
		
	#endif
}

void btn_power_button(void){
	
	   uint16_t loopbreak = 0;
	
    if((GPIO_ReadInputBit(RMT_btn_power_button_Port, btnPOWER))){
				while((GPIO_ReadInputBit(RMT_btn_power_button_Port, btnPOWER)) && loopbreak < Btn_TimeOut){ loopbreak++; delay_ms(15);}
				//*KeyValue = btnPowerValue;
        BTNpower_key = TRUE;
		}

}