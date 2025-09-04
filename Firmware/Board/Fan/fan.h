
//////////////////////Button///////////
//SW4  LIGHT   button 	PULL_DOWN  PIN20 PD3 change to PIN19  PD2
//SW5  TIMER   button		PULL_DOWN  PIN11 PB5
//SW3  POWER	 button		PULL_DOWN  PIN16 PC6
//SW2  SPEED++ button   PULL_DOWN  PIN14 PC4
//SW1  SPEED-- button   PULL_DOWN  PIN13 PC3
///////////////////////////////////////

//////////////IR RECEIVER PIN////////////
//IR1  	 IR    	  PULL_DOWN  PIN5  PA1

//////////////Light out pin////////////
//light  light    Push pull  PIN1  PD4

//////////////Charging Pin////////////
//led5
//Charging led    Push pull  PIN3  PD6
//Greeen led     push pull  pin_15  PC5

//////////////////ADC////////////////
//DC+  charge signal input    		PuLL Down  PIN12  PB4
//BT+  ADC    		                PuLL Down  PIN19  PD2 change to PIN20 PD3
////////////////Oscillation/////////////
//FAN  PWM    		        Push pull  PIN_10  PA3
//FAN  Oscillation    		Push pull  PIN_2  PD5
///////////////////////////////////////////////////
//problem : buttom mode full speed theke speed down button er maddhome low speed gele pothom bar kaj kore na


/////////////////////////////////////
#ifndef __FAN__H_
#define __FAN__H_

#include "main.h"
#include "apm32f00x_gpio.h"
#include "apm32f00x.h"
#include "apm32f00x_usart.h"
#include "apm32f00x_iwdt.h"
#include "stdio.h"
#include "Timer/time.h"
#include "Battery/battery.h"
#include "utility/utility.h"
#include "apm32f00x_tmr2.h"
#include "main.h"



//extern const uint16_t speed_array[9]; //for speed 9
//extern const uint16_t smps_speed_array[9]; //for speed 9

extern const uint16_t speed_array[5]; //for speed 5
extern const uint16_t smps_speed_array[5]; //for speed 5


#define MOSFET_N   1 
/////////////////////////
#define Autoshutdown 15       					//300    //        
#define LoadDisconnectDelay  2         // Load diconnect delay

////////////////////////////////////////////////////////
////////////////////////Sleep Time//////////////////////////

#define Sleep_delay           10
////////////////////////////////////////////////////////////
//----------------------------------Fan Timer-------------------------------

#define fan_timer 1800 // 30min = 30*60 = 1800s

//----------------------------------Indicator and Light-------------------------------------

#define Light_Port  		   GPIOD
#define Light_pin  		   	 GPIO_PIN_4
#define Light_ON		   		(Light_Port->DOUT |= (uint8_t)Light_pin);    //GPIOx->ODR |= (uint8_t)PortPins;
#define Light_OFF		   		(Light_Port->DOUT &= (uint8_t)(~Light_pin)); //GPIOx->ODR &= (uint8_t)(~PortPins);
#define Light_Toggle      (Light_Port->DOUT ^= (uint8_t)Light_pin);    //GPIOx->ODR &= (uint8_t)(~PortPins);

#define Charge_signal_Port  		   GPIOD
#define Charge_signal_pin  		   	 GPIO_PIN_6
#define Charge_signal_ON		   		(Charge_signal_Port->DOUT |= (uint8_t)Charge_signal_pin);     //GPIOx->ODR |= (uint8_t)PortPins;
#define Charge_signal_OFF		   		(Charge_signal_Port->DOUT &= (uint8_t)(~Charge_signal_pin));  //GPIOx->ODR &= (uint8_t)(~PortPins);
#define Charge_signal_Toggle      (Charge_signal_Port->DOUT ^= (uint8_t)Charge_signal_pin);     //GPIOx->ODR &= (uint8_t)(~PortPins);

//Defining led pins for indication
#define LED_GREEN_Port  	GPIOC
#define LED_GREEN_pin  		GPIO_PIN_5
#define LED_GREEN_ON		 (LED_GREEN_Port->DOUT |= (uint8_t)LED_GREEN_pin );   //GPIOx->ODR |= (uint8_t)PortPins;
#define LED_GREEN_OFF		 (LED_GREEN_Port->DOUT &= (uint8_t)(~LED_GREEN_pin ));//GPIOx->ODR &= (uint8_t)(~PortPins);
#define LED_GREEN_Toggle (LED_GREEN_Port->DOUT ^= (uint8_t)LED_GREEN_pin);    //GPIOx->ODR &= (uint8_t)(~PortPins);


/////////////////////////Charge Detector//////////////////////////////////////////

#define Charging_input_Port           GPIOB
#define Charging_input_pin        		GPIO_PIN_4

/////////////////////////////////Fan Parameters/////////////////////////////////////////////////////

#define FAN_FULL           1000                       // the actual value used by the Timer2 routines for 100% pwm duty cycle

//#define FAN_PWMscale     10                         // FAN_maxPWMduty/100 = 1000/100 = 10


//-----------------------------------
//#define FAN_MAX          100                        // the value for pwm duty cyle 0-100%
//#define FAN_MIN          36                         // the value for pwm duty cyle 0-100% (below this value the current running in the system is = 0)
//#define rmtFAN_START     40                         // the value for pwm duty cyle 0-100%
//#define btnFAN_START     68

//------------------------------------
//#define FAN_NATURAL_TIME 10                          // 10 SECOND
//#define FAN_MIN_NATURAL  40
//#define FAN_MAX_NATURAL  100                         // MAXIMUM oscillation speed
//------------------------------------


#define FAN_SpeedMAX      		 1000                           // the value for pwm duty cyle 0-100%
#define FAN_SpeedMIN      		 speed_array[0]                 // the value for pwm duty cyle 0-100% (below this value the current running in the system is = 0)
#define FAN_SpeedMIN_SMPS      smps_speed_array[0]
 

//const uint16_t speed_array[8] = {50, 60, 70, 80,90,100,110,160};
//-----------------------Button MODE Defination---------------------
#define btnFAN_START           speed_array[2]; //// for speed 5
#define SMPS_btnFAN_START      smps_speed_array[2]; //// for speed 5

//#define btnFAN_START           speed_array[4]; // for speed 9
//#define SMPS_btnFAN_START      smps_speed_array[4]; // for speed 9

/////////////////////////Remote mode defination//////////////////////////
#define rmtFAN_START      speed_array[0]                            					// the value for pwm duty cyle 0-100%

/////////////////////////////////Speed Step//////////////////////////////
#define Fan_SpeedStep     (FAN_SpeedMAX - FAN_SpeedMIN) / 8      // 2 step increment for FAN SPEED CALCUALATION

//---------------------------Remote Natural---------
//#define FAN_NATURAL_TIME 10000                         // 1000ms = 10 SECOND
//#define FAN_MIN_NATURAL  200
//#define FAN_MAX_NATURAL  1000                         // MAXIMUM oscillation speed//------------------------------------

///////////////////////////////////Light control pin//////////////////////////////////////////////////////
#define Light_Port  		   GPIOD
#define Light_pin  		   	 GPIO_PIN_4
#define Light_ON		   		(Light_Port->DOUT |= (uint8_t)Light_pin);   //GPIOx->ODR |= (uint8_t)PortPins;
#define Light_OFF		   		(Light_Port->DOUT &= (uint8_t)(~Light_pin));//GPIOx->ODR &= (uint8_t)(~PortPins);
#define Light_Toggle      (Light_Port->DOUT ^= (uint8_t)Light_pin);    //GPIOx->ODR &= (uint8_t)(~PortPins);

///////////////////////////////////Fan controle pin//////////////////////////////////////////////////////

#define Fan_Port  		        GPIOA
#define Fan_pin  		   	    	GPIO_PIN_3
#if MOSFET_N == 1
	#define Fan_ON		   				(Fan_Port->DOUT |= (uint8_t)Fan_pin);   //GPIOx->ODR |= (uint8_t)PortPins;
	#define Fan_OFF		   				(Fan_Port->DOUT &= (uint8_t)(~Fan_pin));//GPIOx->ODR &= (uint8_t)(~PortPins);
#else	
  #define Fan_ON		   				(Fan_Port->DOUT &= (uint8_t)(~Fan_pin));//GPIOx->ODR &= (uint8_t)(~PortPins);		
  #define Fan_OFF		   				(Fan_Port->DOUT |= (uint8_t)Fan_pin);   //GPIOx->ODR |= (uint8_t)PortPins; 
#endif



/////////////////////////////Button status defination//////////////////////////////////////////
#define rmt_PS_Mode        1          					// remote power/speed key state
#define rmtSpeedMode       2          					// remote Natural  state //rmtNatureMode      2 

#define rmtLightMode       0b00000100           // remote light state
#define btn_fanMode        0b00001000           //mode detection flag in button key operation

#define SpeedAdd           0b00010000           // speed inreament and decrement allocation flag
#define LowVoltFanOFF      0b00100000           // Battery low volt detection flag
#define FanRunMode         0b01000000           // fan run mode flag
#define TimerMode          0b10000000           // fan run mode flag

//-----------------------------------------------

////////////////////////////////
extern volatile BOOL lowVoltShutdown; 
extern uint8_t speed_status;
extern uint8_t keyStep;
extern BOOL AnyChange ;

///////////////////////////////Button status flag/////////////////////////////////////////
//fan.status To identify the reception status of the remote control 
//[7]:
//[6]:  FanRunMode
//[5]:  LOW_VOLT_FAN_OFF  
//[4]:  SpeedAdd 
//[3:]: btn_fanMode
//[2:]: remote light
//[1:]: remote Natural  state
//[0:]: remote power/speed key

///////////////////////////////Device Status()//////////////////////////////////////////
typedef struct 				// The fields are ordered to reduce memory over caused by struct-padding
{
	uint8_t  status;         // To identify the reception status of the remote control 
	uint16_t Speed;
	
}device;

//extern device fan, rmtLight;
extern device fan, light;

typedef struct {	
         uint8_t    PS_step;
				 uint8_t    Timer_step;
				 BOOL  Light ;
				 BOOL  speedUp;
				 BOOL  speedDown;
				 BOOL  timerFlag;
				 
}FAN_keypad;

extern FAN_keypad key;
extern uint8_t BTN_Step;
extern uint8_t hold_BTN_Flag;
extern uint16_t BatVolt;

/////////////////////////////////////////////////////////////////////////
void charge_detector_interupt_init(void);
void indicator_init(void);

void sleepMode(void);

void fan_init(void);

void fan_enable(void);

void fan_command(uint16_t pwm);

void fan_shutdown(void);

void Fan_Load(uint16_t pwm_duty);

#endif 