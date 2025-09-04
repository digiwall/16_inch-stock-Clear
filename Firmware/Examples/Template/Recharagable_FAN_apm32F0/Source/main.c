/*!
 * @file        main.c
 *
 * @brief       Main program body
 *
 * @version     V1.0.1
 *
 * @date        2022-04-11
 *
 * @attention
 *
////////////////////////Fan post production supprot  ///////////////////////////////////


//Problem 1, DATE:- : Light driving transistor is failed while battery terminal is reversed by user.
Solution: Flywheel diode across the led light on circuit has been removed:

//Problem 2: 

 */
#include "main.h"
#include "Board.h"
#include "apm32f00x_gpio.h"
#include "apm32f00x_misc.h"
#include "apm32f00x_eint.h"
#include "apm32f00x_iwdt.h"
#include "apm32f00x_rcm.h"
#include "apm32f00x_wupt.h"
#include "apm32f00x_tmr1.h"
#include "apm32f00x_fmc.h"

// Place in a RAM section that is NOT initialized after reset
 static uint32_t powerUpFlag1;

//----------------------------Remote---------------------------------------------------  
uint8_t  Remotaddress  = 0;

/////////////////////////////////////////////////////////////////////////////

uint16_t TimerVlaueMilis = 0;

//////////////Natural Mode/////////////////////////

uint16_t milis_step = 0 ;

//----------------------------Button or Remote ---------------------------------------
uint8_t  KeyValue = 0;  // button key button value
BOOL     RMT_POWER = 0;  // Remot power button
//////////////////////////////////////////////////////////
//---------SMPS DETECTOR-----------
BOOL SMPS_detector = 0; // AC LINE SMPS DETECTION

//-------------------------------------	
uint16_t pauseDelay;  // 
uint16_t pulseCount;
uint8_t  pauseDelay_Flag;

/////////////////////////////////////////////////////////
uint16_t   BatVolt = 0; //Battery voltage variable

//////////////////////////////////////////////

 BOOL btn_blink_flag = 0;
 uint16_t btn_blink_time = 0;
 uint8_t up_bt_blink_lock = 0;
 uint8_t down_bt_blink_lock = 0; 
	
//////////////////////////////////////////////

/** Timer tick */
volatile uint32_t tick = 0;
//uint16_t miliseconds = 0;
uint16_t seconds = 0;

/** TMR2 Init */
void TMR2Init(void);

/* USART Init */
void USART_Init(void);

void TMR4Init(void);
void PowerSpeedMode(void);
void NaturalMode(void);
void BtnSpeedUpDown(void);
void BtnTimer(void);

/** IWDT init */
void IWDTInit(uint32_t freqHIRC);
/** LIRC frequency Measurment */
uint32_t LIRCMeasurment(void);

void initializer(void){

	  Serial_begin(9600);
		uint32_t temp = 0;
	
	  ADC_GPIO_Init(BAT_VOLTS_CHAN);	  
		
		btn_init();	
	  indicator_init();
	  LED_GREEN_OFF;
	  fan_init();
		
	  Remote_Init();
		TMR4Init();	
	  
	  charge_detector_interupt_init();
		
		battery_sec = 0;
	
	  int Bat_volt_timout = 0;
	
    while((getBATvolt() < 12) && (Bat_volt_timout < 150)){ //for debug on 25/05/26
				second_tick();
			  Bat_volt_timout++;
        //delay_ms(20);			
    }
		 
    key.Timer_step = 0;
		Sleepflag = FALSE;
		KeyValue = 0;
		init = FALSE;	
    battery_sec = 0;
		sleep_delay.sec = 0;
		sleepDelay = 0;
}


///////////////////////Exceptional Handaling while hanging and sleep mode/////////////////////////////////

void ResetProc(void) //Due to  booting issue after fist sleep during external interupt.
{
    uint8_t i;

    // ? Check if the reset cause was IWDT (Independent Watchdog Timer reset)
    if(RCM_ReadResetFlag(RCM_RTS_FLAG_IWDTRST) == SET)
     {
        //?? Blink LED2 and LED3 alternately 20 times (visual indication)
        //for(i = 0; i < 20; i++)
        //{
           //delay_ms(100); // Some delay loop
           //SerialPrint_NumString("IWDTREST", 1 );
        //}
			 
			 if(read_flash2(FMC_WatchDog_reset_detection) == 1){
			 
			     wirte_flash2(FMC_Power_onoff_memory, 1); 
				 
				   wirte_flash2(FMC_WatchDog_reset_detection, 0); // Watchdog Reset Detection for fast button press 
			 }
			 
			 else{                                              // Check button debouncing for hanging
				  if(read_flash2(FMC_Power_onoff_memory) == 1){
					     wirte_flash2(FMC_Power_onoff_memory, 0);	
					}
					else{
					     wirte_flash2(FMC_Power_onoff_memory, 1);	
					}			       		 
			 }			  
        // ? Clear the IWDT reset flag after handling
        RCM_ClearResetFlag(RCM_RTS_FLAG_IWDTRST);
     }
}


int main(void)
{	
    
	 //IWDTInit(LIRCMeasurment());
	
	 //IWDTInit(128000);
	
	  //IWDT_Enable();
	  IWDT->KEYWORD = IWDT_KEY_ENABLE;
	  IWDT->KEYWORD = IWDT_KEY_ACCESS;
    //IWDT_EnableWriteAccess();
    /**
        IWDT timeout = 0.5S = (Counter Reload Value * DIV) / freqLIRC;

        Counter Reload Value = (freqLIRC * 0.5) / DIV
                             = freqLIRC / (3 * DIV)
    */
    //IWDT_SetDivider(IWDT_DIV_128);          // Clock Divider
	  IWDT->PSC = (uint32_t)IWDT_DIV_128;
    //IWDT_SetReloadCounter(99);  // fro .5s= freqLIRC/(2*IWDT_DIV_256) =2*256 = 512, for  .3s  = (1/.3)*256, . for .4 = (1/.4)*256
	  IWDT->CNTRLD = (uint32_t)99;
    //IWDT_ReloadCounter();
	  IWDT->KEYWORD = IWDT_KEY_REFRESH;
	
	 //delay_ms(100);	  //Delay
	
	 initializer(); // initialization
	  
	 //NVIC_SystemReset(); // Standard Cortex-M3 software reset;
   	
   ///////// System Software Reset On first power-up, reset once  //////////////
   // if (powerUpFlag1 != 0xDEA) {
       // powerUpFlag1 = 0xDEA;  // Set marker
        //delay_ms(200);         // Let power stabilize
			  //SerialPrint_NumString("NVIC_SystemReset 1", powerUpFlag1 );
        //NVIC_SystemReset();        // ARM Cortex-M1 reset         // Reset once
    //}	
		
		//////////////////////////////////////////////////////////////////////
		
	 // On second boot: powerUpFlag is set, skip reset
   // powerUpFlag = 0;  // Clear flag so it won't reset again next time

    flash_init1(); // Memory Initialization		
	
	  if(read_flash2(FMC_batt_status_memory)> 10){ // Check Battery
			 wirte_flash2(FMC_batt_status_memory, 0);	
		}
		
		///////////////Check Battery Voltage and charging input for disabling power LED/////////
		//if(GPIO_ReadInputBit(Charging_input_Port, Charging_input_pin) && (getBATvolt() < 80)){
		   	 //SerialPrint_NumString("Charge_detector 1:", Charge_detector ); 
	       //SerialPrint_NumString("getBATvolt():", getBATvolt() );
			   //wirte_flash2(FMC_Power_onoff_memory, 1);
		//}
		////////////////////////////////////////////////////////////////////////////////////////
		
		  lowVoltShutdown = read_flash2(FMC_batt_status_memory); // read battery voltage status     		
			//Sleepflag = TRUE;
	    sleepRMTinit = FALSE;  //Remote interrupt initialization flag
	    remotSleepflag = TRUE; //Remote sleep mode flag
			LED_GREEN_OFF;         // ensure power indicator off
	    Charge_signal_OFF;
			init = FALSE;
	    Light_OFF;
	    fan_shutdown();	

      //wirte_flash2(0, 2222);
		  //wirte_flash2(1, 3333);
		  //wirte_flash2(2, 4444);
		
		  ResetProc();	// Check i watchdog reset
		
		 if(read_flash2(FMC_Power_onoff_memory) == 0){      //Power Onstate
		      KeyValue = btnPowerValue;
			    wirte_flash2(FMC_Power_onoff_memory, 1);
			    SerialPrint_NumString("Power_onoff_1_first", read_flash2(1) );
			    fan.status =  bitSet(fan.status, rmt_PS_Mode); 
			    lowVoltShutdown  = read_flash2(FMC_batt_status_memory);
					RMT_POWER = TRUE;
					down_bt_blink_lock = 0;
					up_bt_blink_lock	= 0; 
			     
		   }
		 else if(read_flash2(FMC_Power_onoff_memory) == 1){  //power off state
		      wirte_flash2(FMC_Power_onoff_memory, 0);
			    KeyValue = 0;		
          SerialPrint_NumString("Power_onoff_2_first", read_flash2(1) );
          fan.status =  bitClear(fan.status, rmt_PS_Mode);	
					fan_shutdown();
					key.PS_step = 0;
					key.Light = FALSE;
					BTN_Step = 0;
					hold_BTN_Flag = 0;
					Light_OFF;
					sleep_delay.sec = 0;	
					sleepDelay = 0;       			 

		 }
		 else {
		     KeyValue = btnPowerValue;                        //Power Onstate     
			   wirte_flash2(FMC_Power_onoff_memory, 1);
			   SerialPrint_NumString("FPower_onoff_3_first", read_flash2(1) );
			   fan.status =  bitSet(fan.status, rmt_PS_Mode);
			   lowVoltShutdown  = read_flash2(FMC_batt_status_memory);
			   RMT_POWER = TRUE;
         down_bt_blink_lock = 0;
         up_bt_blink_lock	= 0;
		 }   	 
		
		//delay_ms(10);	
		
  while(1)
  {
		
////////////////////////////////////////Re_Initializaiton with remote//////////////////////////////////////		
			  if(sleepRMTinit == TRUE){					
					  TMR4Init();
					  IWDTInit(LIRCMeasurment());
				    sleepRMTinit = FALSE;            					
				}		
////////////////////////////////////////////////////////////////////////////////////////////////////////////	
        IWDT_ReloadCounter();
//////////////////////////Memory Check//////////////////////////////////////////////////////////////////////
				//SerialPrint_NumString("Charge_detector 2:", Charge_detector );
				//SerialPrint_NumString("FMC_batt_status_memory", read_flash2(0));		
				//SerialPrint_NumString("FMC_Power_onoff_memory", read_flash2(1) );	
				//SerialPrint_NumString("FMC_batt_charge_cut_momory", read_flash2(2) );	
        
/////////////////////////////////////////Input SCAN/////////////////////////////////////////////////////////	
         //bat_offset = key.PS_step/3;
				 BatVolt = getBATvolt();//+bat_offset;	
         
				 ////exception handeling for voltage initialization
				 if (BatVolt > 158 && lowVoltShutdown == TRUE){
				     BatVolt = 110;
				 }else if(BatVolt > 150 && lowVoltShutdown == FALSE){
				    BatVolt = 120;
				 }	

          	
				 
				 //BatVolt = 12;				  
			   //BatVolt = 130;		   
			   SerialPrint_NumString("BatVolt1", BatVolt );
			   button(&KeyValue);
				
				 //if(read_flash2(FMC_Power_onoff_memory) == 1){
				     //KeyValue = btnPowerValue;					   
				 //}

		     if(KeyValue == FALSE){ 			
			      scan_remote(&Remotaddress, &KeyValue);
			   }					 

				 
				 //KeyValue = 31;
				 if(KeyValue){
					 
					   	Serial_print_string("Remote Key:");
							Serial_print_int(KeyValue);
							Serial_newline();						 
						 	SerialPrint_NumString("BatVolt", BatVolt );
					    //Charge_signal_Toggle;					 
					    
              sleepDelay = 0;		
					 
							if(KeyValue == btnSpeedDown){
								
								 if(key.PS_step == 0){								 
								    down_bt_blink_lock++;								    
							   }
								 
								 SerialPrint_NumString("key.PS_step", down_bt_blink_lock);
								 SerialPrint_NumString("down_bt_blink_lock", down_bt_blink_lock);
							   if(down_bt_blink_lock == 2){								 
										btn_blink_flag = 0;  // Power LED Blinking flag for pressing button
																		
							   }else if(down_bt_blink_lock == 1){
								   	btn_blink_flag = 1;					         
								 }
                 up_bt_blink_lock	= 0;	
							}
							
							if(KeyValue == btnSpeedUp){
								 SerialPrint_NumString("UP button", key.PS_step);
								// if(key.PS_step == 8){ // speed 8
                 if(key.PS_step == 4){									 
								    up_bt_blink_lock++;								
							   } 
								 if(up_bt_blink_lock == 1){
										btn_blink_flag = 0;  // Power LED Blinking flag for pressing button	
									 
								 }else if(up_bt_blink_lock == 0){
								    btn_blink_flag = 1;
								 }
								 down_bt_blink_lock = 1;							
							 }
							
							if(KeyValue != btnSpeedDown && KeyValue != btnSpeedUp){
								  btn_blink_flag = 1;  // Power LED Blinking flag for pressing button
							}	
               btn_blink_time = 0;	 // Power LED Blinking flag for pressing button				
															
				 }				 

				 //-----------------Power LED Blinking flag for pressing button --------------------------//
				 if(key.timerFlag == TRUE){ 
									btn_blink_flag = 0;
					        btn_blink_time = 0;
				  }
					 
				 if(btn_blink_flag == 1 && btn_blink_time < 2 ){
				      LED_GREEN_OFF;
					    btn_blink_time++;
					    //SerialPrint_NumString("btn_blink_time", btn_blink_time );
				 
				 }else if((fan.status & rmt_PS_Mode)&&(key.timerFlag != TRUE)){
					    LED_GREEN_ON;
				      btn_blink_flag  = 0;
					    if(btn_blink_time >= 2){btn_blink_time  = 0;}				 
				 }
				 
				 //-------------END Of Power LED Blinking flag for pressing button -------------------------//
				 
				 if(KeyValue == btnPowerValue){
						 RMT_POWER = TRUE;
             down_bt_blink_lock = 0;
             up_bt_blink_lock	= 0;					 
					}				 
//----------------------------------------------------------------------------------//
				lowVoltShutdown  = read_flash2(FMC_batt_status_memory);
  		  //SerialPrint_NumString("Save Value", read_flash(0) );								
									
//-------------Re_Initializaiton with chargeer ,power button----------------------//
				 
				 	//SerialPrint_NumString("KeyValue", KeyValue );
         if(init == TRUE){			 
					  //TMR4Init();
					 if(KeyValue == rmtPS && !(fan.status & rmt_PS_Mode)){
							initializer(); 
						  IWDTInit(LIRCMeasurment());
						  fan.status =  bitSet(fan.status, rmt_PS_Mode);						  
							//SerialPrint_NumString("INTIT", 1 );
							init = FALSE;
					   }else if(!(fan.status & rmt_PS_Mode) && (Charge_detector == TRUE)){						
						    initializer();
							  IWDTInit(LIRCMeasurment());
                init = FALSE;							 
						 }
					 }
					//SerialPrint_NumString("rmt_PS_Mode", fan.status & rmt_PS_Mode );			

				if(Charge_detector == TRUE || GPIO_ReadInputBit(Charging_input_Port, Charging_input_pin) ){
				   if(lowVoltShutdown == TRUE){
					    wirte_flash2(0, 0);	// Low voltage detection is disable when SMPS is detected.......	
					 } 					 
					  lowVoltShutdown = FALSE;
					  BatVolt = 130;
				 }					 
					
//------------------------------Timer--------------------------------//				 
			second_tick();
		  milis_tick(0);	     			 
				 
//-------------------------Controller---------------------------------//		

 IWDT_ReloadCounter();			 
					 
 if(lowVoltShutdown == FALSE) {	
	 
		      //--------------Remote and Button Value Processing---------------------/
					switch(KeyValue){		
						
            case rmtPS:  // POWER BUTTON
							        
											if((fan.status & rmt_PS_Mode)||(fan.status & btn_fanMode)){
										      fan.status =  bitClear(fan.status, rmt_PS_Mode);
													fan_shutdown(); // 
													key.PS_step = 0;
													key.Light = FALSE;
												  BTN_Step = 0;
												  hold_BTN_Flag = 0;
													Light_OFF;
												  sleep_delay.sec = 0;	
													sleepDelay = 0;
												  wirte_flash2(FMC_Power_onoff_memory, 0);  // Save Power ON state Status for next action
											}
											else{ 
													fan.status =  bitSet(fan.status, rmt_PS_Mode); 
                          wirte_flash2(FMC_Power_onoff_memory, 1);  // Save Power OFF state Status for next action										
										  }
											KeyValue = 0;	                      											
											
						break;						
						
						case rmtSpeed:	//fan speed					
									//key.NaturalFlag = !(uint8_t)key.NaturalFlag;
						      AnyChange = TRUE;
                  PowerSpeedMode();									
									KeyValue = 0;
                   						
						break;
						
						case btnValTimer:        //Fan Timer AND it is  define in button.h	
               							
							 if(((fan.status & rmtSpeedMode)||(fan.status & btn_fanMode))){
							 	    key.timerFlag = TRUE ;//!(uint8_t)key.timerFlag;
								    KeyValue = 0; //Reset button and remote value
								    GreenLed.miliSec = 0; // restet gree LED TIMER
                    btnTim_sec = 0;												
                    LED_GREEN_OFF;
									  if(key.Timer_step < 2){											
									     key.Timer_step++;											 
										}else{
									   key.Timer_step = key.Timer_step * 2;	
									  }									
							    }

								  if(key.Timer_step == 1){     // fan running timer for 0.5 hour 
								     TimerVlaueMilis = 1800;
								  }
								  if(key.Timer_step == 2){     //fan running timer for 2 hour
								     TimerVlaueMilis = 7200;
									}
									if(key.Timer_step == 4){     //fan running timer for 4 hour
								     TimerVlaueMilis = 14400;
								  }
									if(key.Timer_step == 8){     // fan running timer for 8 hour
								     TimerVlaueMilis = 28800;  
								  }
									if(key.Timer_step > 8){
								  	 key.Timer_step = 0;       // Reset timer step
                     key.timerFlag = FALSE;    // Reset timer flag
								  }
						break;
						
            default:
                //------------Check Timer Mode------------------									
						break;
					}
	      /////////////////////////////////Light Button, Speeed Up Button, Speed Down Button//////////////////////////
					
				if(((KeyValue == RMT_BTN_light)&&((fan.status & rmt_PS_Mode)||(fan.status & btn_fanMode)))){ // Light button
					
					if((hold_BTN_Flag == FALSE)){
						key.Light = !(uint8_t)key.Light;			 
						KeyValue = 0;
						if(key.Light == TRUE && lowVoltShutdown == FALSE){
									Light_ON;										 
	              }else{
									Light_OFF;
									sleepDelay = 0;
								}
						hold_BTN_Flag = TRUE;					
					}
				}
				else if((KeyValue == btnSpeedDown) && (fan.status & rmt_PS_Mode)){ //Down Button	
            AnyChange = TRUE;					
						BtnSpeedUpDown();            
				}
				else if((KeyValue == btnSpeedUp) && (fan.status & rmt_PS_Mode)){ // Up Button	
            AnyChange = TRUE;					
						BtnSpeedUpDown();            					
				}
				else{
						hold_BTN_Flag = FALSE;	/////Button Hold Flag									
				}
			
      /////////////////////////////////Execution//////////////////////////////			
			//--------------------------Button Timer Mode-----------------------------

			 BtnTimer();
			
       Fan_Load(fan.Speed);	 
}	
 


IWDT_ReloadCounter();


if(((BatVolt <= BAT_VOLT_FAN_OFF && lowVoltShutdown == FALSE && battery_sec > LoadDisconnectDelay))||(lowVoltShutdown == TRUE && battery_sec > LoadDisconnectDelay)){ //&& (!(fan.status & LowVoltFanOFF))){   // LOW Voltage Disconnect			
		   
	         
	        if(lowVoltShutdown == FALSE){
					    lowVoltShutdown = TRUE;
						   wirte_flash2(0, 1);
						  //delay_ms(2);
						  SerialPrint_NumString("after save", read_flash2(0) );
					 }
	         				 
					 fan_shutdown();
					 Light_OFF;
	
					 key.timerFlag = FALSE;	
					 key.Light = FALSE;
	
					 fan.status =  bitSet(fan.status, LowVoltFanOFF); // Set low voltage flag
					 battery_sec = 0;	
	         sleepMode();	
 }

 if(BatVolt >= BAT_VOLT_FAN_ON && lowVoltShutdown == TRUE){ // Low voltage dicconnect is disable
					   lowVoltShutdown = FALSE;
             wirte_flash2(0, 0);						
  }else if(BatVolt >= low_volt_signal){ //at low voltage (<11 vdc)battery second timer will be zero
							battery_sec = 0;
 }			
				
//--------------Charging AND Monitoring---------------------//				 
					if(Charge_detector == TRUE || GPIO_ReadInputBit(Charging_input_Port, Charging_input_pin)){	
               SMPS_detector = TRUE;
               				
							 Charge_signal_ON;
						   //SerialPrint_NumString("charge LED ON", 1 );
						   sleep_delay.sec = 0;	
							 sleepDelay = 0;
					  }else if((BatVolt <= (BAT_VOLT_FAN_OFF+5))|| (lowVoltShutdown == TRUE)){							    

									if(lowCharg_milis > 1000 && (RMT_POWER == TRUE)){				
				              Charge_signal_Toggle;       // Toggle Charging LED When low voltage detected	
                      lowCharg_milis	= 0;	 
				            }					    					
					  }else if((BatVolt > (BAT_VOLT_FAN_OFF+5))){
							
							   Charge_signal_OFF;						
						}		 
					 //SerialPrint_NumString("lowCharg_milis", lowCharg_milis );
					 
					if(!(GPIO_ReadInputBit(Charging_input_Port, Charging_input_pin))){
						    //lowCharg_milis = 0;
						    SMPS_detector = FALSE;
                Charge_detector = FALSE;
               // Charge_signal_OFF;	
						    SerialPrint_NumString("LVTShutDelay", sleepDelay  ); 
						    SerialPrint_NumString("battery_sec", battery_sec); 
						
								//if(((sleepDelay > Sleep_delay) && (!(fan.status & rmt_PS_Mode)))||((sleepDelay > lowVoltShutDelay) && (BatVolt < BAT_VOLT_FAN_OFF))){	
                   if(((sleepDelay > Sleep_delay) && (!(fan.status & rmt_PS_Mode))) || (((key.Light == FALSE) && !(fan.status & btn_fanMode))&&(sleepDelay > Autoshutdown))){				
										
									 
									  SerialPrint_NumString("Reset yes", 1 );									
									  
									  sleep_delay.sec = 0;
									  sleepDelay = 0;
										//Charge_detector = FALSE;
										Charge_signal_OFF;										
										delay_ms(2);
										sleepMode();
									  //WWDT_SoftwareReset();
								}else if((BatVolt >= BAT_VOLT_FAN_ON)){
								   Charge_signal_OFF;								
								}								
					 }	 
       //---------------WatchDog---------------------------
            IWDT_ReloadCounter();
       ////////////////////////////////////////////////////////////////////       
	}	
}


void BtnTimer(void){
	
			if(key.timerFlag == TRUE){
 
				/*if(btnTim_sec > TimerVlaueMilis){						 
						 fan_shutdown();
						 GreenLed.miliSec	= 0;							
				}else if(GreenLed.miliSec > 1000){				
				     LED_GREEN_Toggle;	
             GreenLed.miliSec	= 0;	 
				} */
				SerialPrint_NumString("shut time", btnTim_sec );
				
			  if(btnTim_sec > TimerVlaueMilis){						 
						       fan.Speed = 0;    // Fan speed 0 initialize
									 fan_command(fan.Speed);
					         fan.status =  bitClear(fan.status, rmtSpeedMode);
					         fan.status =  bitClear(fan.status, FanRunMode);
									 //fan.status = 0x00;   // fan status 0 initialize
										BTN_Step = 0;
										hold_BTN_Flag = 0;
										key.timerFlag = FALSE;
										btnTim_sec = 0;
										key.Timer_step = FALSE;	    	
										Fan_OFF;
					
										key.PS_step = 0;
										fan.Speed	= 0;	                                                                                       
										fan.status =  bitClear(fan.status, rmtSpeedMode);
										fan_shutdown();	
										fan.status =  bitSet(fan.status, rmt_PS_Mode);
					
						 GreenLed.miliSec	= 0;	
             SerialPrint_NumString("fan.Speed", fan.Speed);
						 SerialPrint_NumString("TimerVlaueMilis", TimerVlaueMilis );
			  }
			  else{
			    if(GreenLed.miliSec > 2000 && pauseDelay_Flag == TRUE){       // pulse led off for 2000ms = 2 second.
			         pauseDelay_Flag = FALSE;
						   GreenLed.miliSec	= 0;
						   pulseCount =0;
						   LED_GREEN_ON;                                            //After 2 second delay, signal LED ON.
			    }
				  if(GreenLed.miliSec > 333 && pauseDelay_Flag == FALSE){	      // pulseing according to defined setting affter .333 second
						   GreenLed.miliSec	= 0;  // pulsing timer zero initialize
						   LED_GREEN_Toggle;      // LED Toggoling
						  
					   if((pulseCount >= key.Timer_step) && (key.Timer_step < 4)){ // 1st two steps of timer setting
					      pauseDelay_Flag = TRUE;
								LED_GREEN_OFF;
					    }
						 else if(pulseCount >= ((key.Timer_step*2)-1)){ // Last two step of timer setting
					           pauseDelay_Flag = TRUE;
								     LED_GREEN_OFF;
									  }					 
					    pulseCount++;	
				  }
			
			  }			
				//Serial_print_char('P');
				//Serial_print_int(key.Timer_step);
				//Serial_newline();	
				//Serial_print_char('t');
				//Serial_print_int(btnTim_sec);
				//Serial_newline();
			
		  }else if(key.timerFlag != TRUE){
				    
						fan.status =  bitClear(fan.status, key.timerFlag);
						
				    if((fan.status & rmt_PS_Mode) && ( btn_blink_flag == 0) ){
						   LED_GREEN_ON;
						}else{
						   LED_GREEN_OFF;
						}
				    
			}
}


void BtnSpeedUpDown(void){

                if((fan.status & rmtSpeedMode)){ // for jump to step 1 //(fan.status & rmt_PS_Mode)||(
							
										BTN_Step = 1;
										hold_BTN_Flag = 0;
										//SerialPrint_NumString("Non Start", fan.status & rmt_PS_Mode); 
								 }
					      if((hold_BTN_Flag == 0)&&(BTN_Step == 0)){ // step 0 which is execute at first start from shutting down
										hold_BTN_Flag = 1;
										BTN_Step = 1;									
										speed_status = 3;									
										fan_enable();	
										//key.PS_step = 4; // speed step 9
									  //keyStep = 4;		 // speed step 9
									
										key.PS_step = 2; // speed step 5
									  keyStep = 2;		 // speed step 5	
					       }
								
								//if(key.PS_step > 8){ //// speed step 9
								   // key.PS_step = 8;								
								//}
								
								if(key.PS_step > 4){ //// speed step 5
								    key.PS_step = 4;								
								}
								
					      if((hold_BTN_Flag == 0)&&(BTN_Step == 1)){ // step1 
									
									if(KeyValue == btnSpeedDown){		
						
										if((fan.Speed <= FAN_SpeedMIN) || (key.PS_step < 1) ){             // Fan Stop
												 fan.Speed = FAN_SpeedMIN;	                                   // add minimum speed
												 key.PS_step = 0;
											   keyStep = 0;
											
										}else{
											
											   speed_status = 4;                         											
												 key.PS_step--;
											   SerialPrint_NumString(" Down PS_step", key.PS_step);	
												 keyStep = key.PS_step;
										 }
												hold_BTN_Flag = 0;
												BTN_Step = 1;

									}										
									else if(KeyValue == btnSpeedUp){								  
											
											speed_status = 5;											
										  key.PS_step++;		

										 // if((fan.Speed > FAN_SpeedMAX) ||(key.PS_step > 8) ){	// speed step 9	
											if((fan.Speed > FAN_SpeedMAX) ||(key.PS_step > 4) ){												
										      fan.Speed = FAN_SpeedMAX;
													key.PS_step = 4;  // speed step 9
										  }
											BTN_Step = 1;
											hold_BTN_Flag = 0;
											keyStep = key.PS_step;
											SerialPrint_NumString("key.PS_step", key.PS_step);
									}									
						    }
								else if((KeyValue == btnSpeedUp)&&(hold_BTN_Flag == 0)){
							
												BTN_Step = 1;
												hold_BTN_Flag = 0;							
								}
								 
												
///////////////////////////////////////////previous function///////////////////
											//fan.status =  bitSet(fan.status, rmt_PS_Mode);
											  fan.status =  bitSet(fan.status, FanRunMode); 
											  fan.status =  bitSet(fan.status, btn_fanMode);
											//fan.status =  bitSet(fan.status, rmtSpeedMode);
											
											//KeyValue = 0;
								
//----------------------------End of Previous Function------------------------//											
											
										if((hold_BTN_Flag == 0)&&(BTN_Step == 2)){ //step 3												
												key.PS_step = 0;												
												BTN_Step = 0;
												hold_BTN_Flag = 1;
												fan.Speed = 0x00;    // Fan speed 0 initialize
												Fan_Load(fan.Speed);
												key.timerFlag = FALSE;
												btnTim_sec = 0;
												key.Timer_step = FALSE;												
							        }
											  KeyValue = FALSE;

			}

			
void PowerSpeedMode(void){
							
							KeyValue = FALSE;		           

							if((fan.status & rmt_PS_Mode)||(fan.status & btn_fanMode)){ //check others mode	
							//SerialPrint_NumString("fan.status & rmtSpeedMode", fan.status & rmtSpeedMode);
							//SerialPrint_NumString("fan.status & btn_fanMode", fan.status & btn_fanMode);
										  if((!(fan.status & rmtSpeedMode))&&(!(fan.status & btn_fanMode))){												
												  fan.status =  bitSet(fan.status, rmtSpeedMode);												

													speed_status = 1;
													
													fan_enable();	

                          fan.status =  bitSet(fan.status, FanRunMode);												
										   }					
											 
			                
											//SerialPrint_NumString("remote up test", key.PS_step);	
											//if(key.PS_step > 0 && key.PS_step < 9){  //Speed Selection steps ( 1, 2, 3 ) of "power/speed" button//if(key.PS_step > 1 && key.PS_step < 10)//	speed 9										
											if(key.PS_step > 0 && key.PS_step < 5){    //Speed Selection steps ( 1, 2, 3 ) of "power/speed" button//if(key.PS_step > 1 && key.PS_step < 10)// speed 5													

												if(speed_status == 1){
												   key.PS_step--;												
												}

												// if(key.PS_step == 8 ){  // Maximum speed by "UP" button without remote then press by remote "speed" button therefore result is shutdown speed. //	speed 9	
												 if(key.PS_step == 4 ){  // Maximum speed by "UP" button without remote then press by remote "speed" button therefore result is shutdown speed. // speed 5
												    key.PS_step++;
												 }
												 
												// SerialPrint_NumString("speed_status", speed_status);	
												 speed_status = 2;												

												// keyStep = key.PS_step; // - 2;
												// if(keyStep > 8){keyStep = 8;}
												 //if(keyStep > 8){keyStep = 8;} //	speed 9	
												 if(keyStep > 4){keyStep = 4;}
												 //SerialPrint_NumString("key.PS_step", key.PS_step);												
											   //SerialPrint_NumString("Check",keyStep);
											  }
                       											
												
											
											//if(key.PS_step > 8 ){   //   if(key.PS_step >= 10 ){
												
											if(key.PS_step > 4 ){   //   if(key.PS_step >= 10 ){                       
                        /* // This portion has disabled by customer(fan department) 												
												key.PS_step = 0;
												speed_status = 0;
												fan.Speed	= 0;	                                                                                       
												fan.status =  bitClear(fan.status, rmtSpeedMode);
												fan_shutdown();	
												fan.status =  bitSet(fan.status, rmt_PS_Mode);
												/////////////////////////////////////////////////
												*/
												
												// this portion is enabled by customer
												   speed_status = 1;
												   key.PS_step = 1;
												
												///
												 												
											}else{
														
													 keyStep = key.PS_step++;
													 keyStep = key.PS_step; // - 2;
													 SerialPrint_NumString("else key.PS_step ", key.PS_step);													
																						
											}								

											
							}
							//SerialPrint_NumString("fan.Speed	", fan.Speed	 );
}


/*
// --------------------------------------------
// IWDT Prescaler Divider Definitions
// (matches your IWDT_DIV_T enum definitions)
// --------------------------------------------
#define IWDT_DIV_4     4
#define IWDT_DIV_8     8
#define IWDT_DIV_16    16
#define IWDT_DIV_32    32
#define IWDT_DIV_64    64
#define IWDT_DIV_128   128
#define IWDT_DIV_256   256

// Example LIRC frequency, measured using your previous function
  uint32_t lircFreq = 128000;  // Replace with your actual measured value

 Reload = (freqLIRC x timeout_s)/divider


*/

/*!
 * @brief       IWDT init.Set IWDT timeout = 0.5s
 *
 * @param       freqHIRC:    LIRC frequency in HZ
 *
 * @retval      None
 *
 * @note
 */
void IWDTInit(uint32_t freqLIRC)
{
    IWDT_Enable();

    IWDT_EnableWriteAccess();
    /**
        IWDT timeout = 0.5S = (Counter Reload Value * DIV) / freqLIRC;

        Counter Reload Value = (freqLIRC * 0.5) / DIV
                             = freqLIRC / (2 * DIV)
    */
    IWDT_SetDivider(IWDT_DIV_256);
    IWDT_SetReloadCounter(freqLIRC / 512);

    IWDT_ReloadCounter();
}

/*!
 * @brief       Measure the LIRC frequency
 *
 * @param       None
 *
 * @retval      LIRC frequency in HZ
    


 *
 * @note
 */
uint32_t LIRCMeasurment(void)
{
    uint16_t icValue[2];
    uint32_t freqLIRC;
    uint32_t freqMasterClk;
    TMR1_ICConfig_T icConfig;

    freqMasterClk = RCM_GetMasterClockFreq();

    WUPT_EnableLIRCMeasurement();

    icConfig.channel = TMR1_CHANNEL_1;
    icConfig.div = TMR1_IC_DIV_8;
    icConfig.filter = 0;
    icConfig.polarity = TMR1_IC_POLARITY_FALLING;
    icConfig.selection = TMR1_IC_SELECT_DIRECTTI;
    TMR1_ConfigInputCapture(TMR1, &icConfig);
    TMR1_Enable(TMR1);

    while(TMR1_ReadStatusFlag(TMR1, TMR1_FLAG_CH1CC) == RESET);
    icValue[0] = TMR1_ReadCompareCapture(TMR1, TMR1_CHANNEL_1);
    TMR1_ClearStatusFlag(TMR1, TMR1_FLAG_CH1CC);

    while(TMR1_ReadStatusFlag(TMR1, TMR1_FLAG_CH1CC) == RESET);
    icValue[1] = TMR1_ReadCompareCapture(TMR1, TMR1_CHANNEL_1);
    TMR1_ClearStatusFlag(TMR1, TMR1_FLAG_CH1CC);

    TMR1_DisableCompareCapture(TMR1, TMR1_CHANNEL_1);
    TMR1_Disable(TMR1);

    freqLIRC = (8 * freqMasterClk) / (icValue[1] - icValue[0]);

    WUPT_DisableLIRCMeasurement();

    return freqLIRC;
}


/*void NaturalMode(void){

			if(((fan.status && rmt_PS_Mode)||(fan.status && btn_fanMode))  && key.NaturalFlag == TRUE){
			//if((fan.status & btn_fanMode)  && (key.NaturalFlag == TRUE)){
			
				  u16 n_duty = 0;
					key.PSN_step = 0;
					//----------------------Time duration calculation of Natural speed -------------------------
				 if(!(fan.status & rmtNatureMode)){
					n_duty = FAN_MAX_NATURAL - FAN_MIN_NATURAL;      //Total duty cycle for natural mode
				    
					if(n_duty <= FAN_FULL && FAN_NATURAL_TIME >= FAN_FULL){  // maximum duty cycle 100;					
					
					   milis_step = FAN_NATURAL_TIME/n_duty;          // increase/Decrease speed per milis_step 
						 
						 natural.miliSec = 0;                           // natural mode timer zero initialized
				 }else{				                                      //Excepton
				    milis_step = 1;
						natural.miliSec = 0;
				 }
				 
				 //---------------------Previous mode tracking of remote for smooth operation------------------------------

				 //if(fan.Speed < FAN_MAX_NATURAL){
				    //fan.status = bitSet(fan.status, SpeedAdd);				
				 //}
			   if(fan.Speed >= FAN_MAX_NATURAL && fan.Speed <= FAN_FULL){ // speed increasing fuction clearing
			      fan.status = bitClear(fan.status, SpeedAdd);
			   }//else{
					 
			     //fan.Speed = FAN_FULL;
					 //fan.status = bitSet(fan.status, SpeedAdd);
			   //}

			  }	
				fan.status =  bitSet(fan.status, rmtNatureMode); //rmtNatureMode 							
				
				//----------------------------Execution-----------------------------------
				if(natural.miliSec > milis_step ){						
						 natural.miliSec = 0;							 
						 if(fan.status & SpeedAdd){						 
								fan.Speed++;								
							  if(fan.Speed > FAN_MAX_NATURAL){							
							     fan.status = bitClear(fan.status, SpeedAdd);
							  }   
						  }
							else {						 
						    fan.Speed--;
							  if(fan.Speed <= FAN_MIN_NATURAL){							
							     fan.status = bitSet(fan.status, SpeedAdd);
							  }
						  }
				  }					
					//fan_command(fan.Speed); 			
					fan.status =  bitSet(fan.status, FanRunMode); //	

			}else if(key.NaturalFlag != TRUE){
						fan.status =  bitClear(fan.status, rmtNatureMode);
						//fan.status = bitClear(fan.status, SpeedAdd);
			}

}*/