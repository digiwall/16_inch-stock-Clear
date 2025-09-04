
//////////////////////Button///////////
//SW4  LIGHT   button 	PULL_DOWN  PIN20 PD3
//SW5  TIMER   button		PULL_DOWN  PIN11 PB5
/////////////SW3  POWER	 button		PULL_DOWN  PIN16 PC6//////////////////////
//SW2  SPEED++ button   PULL_DOWN  PIN14 PC4
//SW1  SPEED-- button   PULL_DOWN  PIN13 PC3
///////////////////////////////////////


#include "delay/delay.h"
#include "apm32f00x_gpio.h"
#include "apm32f00x.h"
#include "apm32f00x_misc.h"
#include "apm32f00x_eint.h"
#include "apm32f00x_iwdt.h"

#define BTN_pullUPmode   	0 

/////////////////////////Button////////////////////////////////////////////////////////
#define Timer_Button_Port           GPIOB
#define btnTimer        					  GPIO_PIN_5

#define Light_Button_Port           GPIOD
#define btnLight        					  GPIO_PIN_2

#define RMT_btn_power_button_Port   GPIOC
#define btnInc                      GPIO_PIN_4
#define btnDecrement                GPIO_PIN_3
#define btnPOWER                    GPIO_PIN_6


#define  btnLightValue   27         //rmtLight
#define  btnValTimer     30 
#define  btnSpeedUp      33
#define  btnSpeedDown    34 

#define  btnPowerValue   31

#define Btn_TimeOut   	 190      // 30ms for debouncing 

//#define CLICKLONG      1000  		// 1s for long click     
//#define CLICKSHORT     75    		// 75ms for short click 

/////////////////////////////Button UP AND Button stage Operation///////////////////////////
#define BtnDownStep1       1          // remote power/speed key state
#define BtnDownStep2       2          // remote Natural  state
#define BtnDownStep3       3          // remote Natural  state

#define rmtLightMode       0b00000100           // remote light state
#define btn_fanMode        0b00001000           //mode detection flag in button key operation

extern BOOL BTNpower_key;

void btn_init(void);
void button(uint8_t* KeyValue);
void btn_power_button(void);

// for next development//////////////////////
/*typedef enum
{
  Idle       = 0,
  Debouncing,
  Pressed,
  Waiting
} BtnState;


typedef struct
{
  uint16_t      pin;     // GPIO pin 
  GPIO_TypeDef *port;    // GPIO port 
  GPIO_PinState pol;     //polarity: SET active high, RESET active low 
  BtnState      state;   // current state 
  uint32_t      tick;    // time of press 
  Key           kShort;  // key enum value for short press 
  Key           kLong;   // key enum value for long press 
} Button;

#define CLICKDEBOUNCE 30    // 30ms for debouncing   
#define CLICKLONG     1000  // 1s for long click     
#define CLICKSHORT    75    // 75ms for short click 

#define BUTTONS 2

Button buttons[BUTTONS] =
{
  {
    .pin    = ENC1_BTN,
    .port   = ENC1_BTN_GPIO,
    .pol    = GPIO_PIN_SET,
    .kShort = ClickVKey,
    .kLong  = LongVKey
  },
  {
    .pin    = ENC2_BTN,
    .port   = ENC2_BTN_GPIO,
    .pol    = GPIO_PIN_SET,
    .kShort = ClickIKey,
    .kLong  = LongIKey
  }
};

Key serveButton( Button *b )
{
  Key key      = NoKey;
  uint32_t now = HAL_GetTick(); // What's the time?
  bool btn     = b->pol == HAL_GPIO_ReadPin( b->port, b->pin );

  switch( b->state )
  {
  case Idle:
    if( btn )
    {
      // The button has been pressed /
      // Remember when /
      b->tick  = now;
      //and move to Debouncing state /
      b->state = Debouncing;
    }
    break;

  case Debouncing:
    if( btn )
    {
      if( now - b->tick > CLICKDEBOUNCE )
        // Real press 
        b->state = Pressed;
    }
    else
    {
      //It was a bounce, start over /
      b->state = Idle;
    }
    break;

  case Pressed:
    if( btn )
    { // Still pressed, is this a long press?/
      if( now - b->tick > CLICKLONG )
      {
        // A long press has been detected /
        key      = b->kLong;
        // Wait for release /
        b->state = Waiting;
      }
    }
    else
    { // Released, is it a short press? /
      if( now - b->tick > CLICKSHORT )
      {
        //A short press has been detected /
        key      = b->kShort;
      }
      // Return to idle /
      b->state = Idle;
    }
    break;

  case Waiting:
    if( !btn )
    {
      b->state = Idle;
    }
    break;
  }
  return key;
}*/