

#include "fan.h"

extern BOOL SMPS_detector;
extern BOOL btn_blink_flag;
extern uint16_t btn_blink_time;

device fan, light;
FAN_keypad key;

//____________________Speed Array_______________________



//const uint16_t speed_array[9] = {260, 325, 395, 475, 560, 595, 665, 760 , 1000}; //Speed 9

//const uint16_t smps_speed_array[9] = {230, 290, 360, 430, 500,575,650,745, 1000};// Speed 9

const uint16_t speed_array[5] = {340, 490, 630,  760,  1000}; //Speed 5

const uint16_t smps_speed_array[5] = {290,  430,  575, 740, 1000}; //Speed 5

uint8_t BTN_Step = 0;
uint8_t hold_BTN_Flag = 0;
volatile BOOL lowVoltShutdown = 0;


BOOL AnyChange = 0;
uint8_t keyStep = 0;
uint8_t speed_status = 0;
			
void indicator_init(void){

    GPIO_Config_T gpioConfig;
    /** Light GPIO configuration */
    gpioConfig.mode  = GPIO_MODE_OUT_PP;	   
    gpioConfig.pin 	 = Light_pin;
    gpioConfig.speed = GPIO_SPEED_10MHz;
    GPIO_Config(Light_Port, &gpioConfig);	

	  gpioConfig.mode  = GPIO_MODE_OUT_PP;	   
    gpioConfig.pin 	 = LED_GREEN_pin;
    gpioConfig.speed = GPIO_SPEED_10MHz;
    GPIO_Config(LED_GREEN_Port, &gpioConfig);
}


void charge_detector_interupt_init(void){

    GPIO_Config_T gpioConfig;

    /** LED2 GPIO configuration */
    gpioConfig.mode  = GPIO_MODE_IN_FLOATING;
	  gpioConfig.intEn = GPIO_EINT_ENABLE;
    gpioConfig.pin 	 = Charging_input_pin;
	  EINT_Config(EINT_PORT_B, EINT_TRIGGER_RISING);	  
    GPIO_Config(Charging_input_Port, &gpioConfig);
	  NVIC_EnableIRQRequest(EINTB_IRQn, 0X01);

	
	  //Charging LED Indicator pIN
    gpioConfig.mode  = GPIO_MODE_OUT_PP;	   // chargint 
    gpioConfig.pin 	 = Charge_signal_pin;
    gpioConfig.speed = GPIO_SPEED_10MHz;
    GPIO_Config(Charge_signal_Port, &gpioConfig);	
}



//fan intialize
void fan_init(void){
	
	GPIO_Config_T fan_config;

  //FAN GPIO configuration 
  fan_config.mode  = GPIO_MODE_OUT_PP;
  fan_config.pin 	 = Fan_pin;
  fan_config.speed = GPIO_SPEED_10MHz;
  GPIO_Config(Fan_Port, &fan_config);
	 
	fan_shutdown();
	fan.status = 0x00;
	BTN_Step = 0;
	key.PS_step = 0;
			
	TMR2_Disable();
}


// must enable fan funtion befor fan start
void fan_enable(void){
	
	if(BatVolt > BAT_VOLT_FAN_OFF){
		
			TMR2_OCConfig_T ocConfigStruct;	   

     /** Set counter = 0XFF, divider = 2 */
     TMR2_ConfigTimerBase(0, 1000);
	
     /** Select channen1 3 */
     ocConfigStruct.channel = TMR2_CHANNEL_3;
	
     ocConfigStruct.count = 0X00;
	
     ocConfigStruct.mode = TMR2_OC_MODE_PWM1;
	
     ocConfigStruct.OCxOutputState = TMR2_OC_OUTPUT_ENABLE;
     ocConfigStruct.OCxPolarity = TMR2_OC_POLARITY_HIGH;
     TMR2_ConfigOutputCompare(&ocConfigStruct);

     TMR2_Enable();	
  }
}


void fan_speed_cal(uint8_t keyStep_p){

	    if(SMPS_detector == FALSE) {

           	fan.Speed = speed_array[keyStep_p]; //Fan_SpeedStep;
						SerialPrint_NumString("Button Up Speed", fan.Speed);
            //SerialPrint_NumString("keyStep_p", keyStep_p);				
			    													
				}else{					

						fan.Speed = smps_speed_array[keyStep_p];
						SerialPrint_NumString("SMPS_fan.Speed", fan.Speed);					
			 }
   }
//////////////////////////////////pwm//////////////////////////////////

// fan speed variation command
void fan_command(uint16_t pwm_duty) { 

  if(speed_status == 1){	    
						if(SMPS_detector == FALSE) {										
								fan.Speed	 =  rmtFAN_START ;		      // Initialize fan speed at remote  power up
						}else{										
								fan.Speed	 =  FAN_SpeedMIN_SMPS;		  // Initialize fan speed at remote  power up 
								//SerialPrint_NumString("SMPS____", FAN_SpeedMIN_SMPS);
						}
	   }
   
	 if(speed_status == 2 &&  AnyChange == TRUE ){    //Remote		 
		  AnyChange = FALSE;
		  fan_speed_cal(keyStep);
		} 		
	 
	 if(speed_status == 3 ){

			if(SMPS_detector == FALSE) {										
						fan.Speed	 =  btnFAN_START ;		      // Initialize fan speed at remote  power up
				    //SerialPrint_NumString("Button mode____", fan.Speed);
			}else{										
					fan.Speed	 =  SMPS_btnFAN_START;		  // Initialize fan speed at remote  power up 
					 // SerialPrint_NumString("butt_BATT____", FAN_SpeedMIN_SMPS);
			}
		}
	 
	 if(speed_status == 4 &&  AnyChange == TRUE ){ // Down Button
		 
		  AnyChange = FALSE;
		 
		 /*
	   if(SMPS_detector == FALSE) {
						fan.Speed = speed_array[keyStep]; //Fan_SpeedStep;
						//SerialPrint_NumString("Button Down Speed", fan.Speed);
			    													
			}else{
					fan.Speed = smps_speed_array[keyStep];
		  }*/
		 
      fan_speed_cal(keyStep);			
		}
	 
		if(speed_status == 5 &&  AnyChange == TRUE ){
		 
		  AnyChange = FALSE;

			fan_speed_cal(keyStep);
		}	
				 
	 //SerialPrint_NumString("keyStep____", keyStep);  
   pwm_duty = fan.Speed;		
		
	//--------------------------------------------- 
  if (pwm_duty > FAN_SpeedMAX) {					                        // check limits of PWM duty cyle and set to PWM_MAX		

    pwm_duty = FAN_SpeedMAX;		
  }
  else if (pwm_duty < FAN_SpeedMIN && pwm_duty > 0 ) {				   // if pwm is less than PWM_MIN then set it to PWM_MIN
    
		 if(SMPS_detector == FALSE){
		    pwm_duty = FAN_SpeedMIN;                                 // Minimum Speed
		 }else{
		    pwm_duty = FAN_SpeedMIN_SMPS;
			  SerialPrint_NumString("Contineous____", pwm_duty);
		 } 		
  }
	

/*	
	if(BatVolt > 140 && pwm_duty < 900){
     pwm_duty = pwm_duty - 50;
		 //Serial_print_char('p');
		 //Serial_print_int(pwm_duty);	
		 //Serial_newline();
   } */
	
  if (pwm_duty <= FAN_SpeedMAX) {
	
	  #if MOSFET_N == 1                       // for N channel mosfet
        TMR2_SetCompareCapture(TMR2_CHANNEL_3, pwm_duty);         // pwm duty command
    #else	
        pwm_duty = FAN_SpeedMAX- pwm_duty;  // For P channel Mosfet
				TMR2_SetCompareCapture(TMR2_CHANNEL_3, pwm_duty);         // pwm duty command
		#endif	

  } 	
}


void fan_shutdown(void){   // Fan shutdown
      
      fan.Speed = 0x00;    // Fan speed 0 initialize
	    fan_command(fan.Speed);
			fan.status = 0x00;   // fan status 0 initialize
			BTN_Step = 0;
			hold_BTN_Flag = 0;
			key.timerFlag = FALSE;
			btnTim_sec = 0;
      key.Timer_step = FALSE;	
	    TMR2_SetCompareCapture(TMR2_CHANNEL_3, 0);
      // change 15/2/2023	
			Fan_OFF;	
}



void Fan_Load(uint16_t pwm_duty){

				//Serial_print_char('L');
				//Serial_print_int(BatVolt);	
				//Serial_newline();
				
				//Serial_print_char('S');
				//Serial_print_int(battery_sec);	
				//Serial_newline();
				
				//Serial_print_char('B');
				//Serial_print_int(battery_sec);	
				//Serial_newline();
			/*	
			if((BatVolt < BAT_VOLT_FAN_OFF)){ //&& (!(fan.status & LowVoltFanOFF))){   // LOW Voltage Disconnect			
				 key.timerFlag = FALSE;
         SerialPrint_NumString("battery_sec", battery_sec);
				
			   if(battery_sec > 10){		// battery low voltage disconnect timer 1minure = 60S	
					 
					 wirte_flash(0, 1);
					 
					 fan_shutdown();
					 Light_OFF;					 
					 key.Light = FALSE;
					 fan.status =  bitSet(fan.status, LowVoltFanOFF); // Set low voltage flag
					 battery_sec = 0;	
           //WWDT_SoftwareReset();	
           sleepMode();					 
				 }		 
			  }
			
		  if(BatVolt >= BAT_VOLT_FAN_OFF){ // Not needed
		      battery_sec = 0;
		    } */
				
				
			//SerialPrint_NumString("keyStep", keyStep );
			
			if((lowVoltShutdown == FALSE)&&(fan.status & LowVoltFanOFF)){  //(fan.status & FanRunMode)){  Load Reconnect
				   fan.status =  bitClear(fan.status, LowVoltFanOFF);	 //Clear low voltage flag
				   fan_init();					 
					 //fan_enable();					 
		    }
			
			if((fan.status & FanRunMode)&&(!(fan.status & LowVoltFanOFF) )){ // Fan 
				   
           fan_command(pwm_duty);
           sleepDelay = 0;				
        }  		
}

void sleepMode(void){
			Sleepflag = TRUE;
	    sleepRMTinit = FALSE;
	    remotSleepflag = TRUE;
			LED_GREEN_OFF;
	    Charge_signal_OFF;
			init = FALSE;
	    Light_OFF;
	    fan_shutdown();			
			delay_ms(100);
      wirte_flash2(FMC_Power_onoff_memory, 0); // 
	
   if(!(GPIO_ReadInputBit(Charging_input_Port, Charging_input_pin))){
		  SerialPrint_NumString("Sleep Mode On", 1 );
		  EINT_ClearIntFlag(EINT_PORT_B);
		  EINT_ClearIntFlag(EINT_PORT_A);
		  EINT_ClearIntFlag(EINT_PORT_C);
		  IWDT_ReloadCounter();
		  wirte_flash2(FMC_WatchDog_reset_detection, 1); // Watchdog Reset Detection for fast button press 
		  

		  TMR4_ClearIntFlag(TMR4_INT_UPDATE);		 	
			NVIC_DisableIRQ(TMR4_IRQn);	

		  delay_ms(50);
		  NVIC_EnableIRQRequest(EINTB_IRQn, 0X01);		  

		 		  RCM_ClearResetFlag(RCM_RTS_FLAG_IWDTRST);
		 		  RCM_ClearResetFlag(RCM_RTS_FLAG_WWDTRST);
      delay_ms(50);		 
			PMU_EnterHaltModeWFI();	//// Enter Halt Mode (sleep until next interrupt)
			//delay_ms(100);
			//NVIC_DisableIRQ(TMR4_IRQn);												
			//PMU_EnterHaltModeWFI(); //// Enter Halt Mode (sleep until next interrupt)

		 }else{
			 
		  SerialPrint_NumString("Power On", 1 );
			 
		}
}





