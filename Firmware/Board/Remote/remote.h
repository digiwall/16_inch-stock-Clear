 /* Website: https://faruklab.com
 * Author: Md. Faruk Hossain
   Mob: 01927421257
 */
// The IR NEC decoding function utilizes timer0 to measure the burst and
// pause lengths of the signal. The timer is automatically started and
// stopped or reset by the IR receiver via a pin change interrupt. The
// measured lengths are interpreted according to the NEC protocol and the
// transmitted code is calculated accordingly. The program was tested with
// the TSOP4838, but it should also work with other 38kHz IR receivers
// (note different pinout if necessary).
//
// ------+         +-----+ +-+ +---+ +---+ +-+
//       |         |     | | | |   | |   | | |     bit0:  562.5us
//       |   9ms   |4.5ms| |0| | 1 | | 1 | |0| ...
//       |         |     | | | |   | |   | | |     bit1: 1687.5us
//       +---------+     +-+ +-+   +-+   +-+ +-
//
// The output of the IR reciever is inverted, a burst is indicated by a
// LOW signal, a pause by a HIGH signal. IR message starts with a 9ms
// leading burst followed by a 4.5ms pause. Afterwards 4 data bytes are
// transmitted, least significant bit first. A "0" bit is a 562.5us burst
// followed by a 562.5us pause, a "1" bit is a 562.5us burst followed by
// a 1687.5us pause. A final 562.5us burst signifies the end of the
// transmission. The four data bytes are in order:
// - the 8-bit address for the receiving device,
// - the 8-bit logical inverse of the address,
// - the 8-bit command and
// - the 8-bit logical inverse of the command.
// The Extended NEC protocol uses 16-bit addresses. Instead of sending
// an 8-bit address and its logically inverse, first the low byte and
// then the high byte of the address is transmitted.
// If the key on the remote controller is kept depressed, a repeat code
// will be issued consisting of a 9ms leading burst, a 2.25ms pause and
// a 562.5us burst to mark the end. The repeat code will continue to be
// sent out at 108ms intervals, until the key is finally released.
// For this application the repeat code will be ignored because it's not
// needed here.
//


//choose 24MHz Of HSI As the master clock source




#ifndef __REMOTE_H_
#define __REMOTE_H_

#include "delay/delay.h"
#include "apm32f00x_gpio.h"
#include "apm32f00x_misc.h"
#include "apm32f00x_eint.h"
#include "apm32f00x.h"
#include <stdio.h>


#define RemotePin_GPIO_PORT    GPIOA
#define RemotePin_GPIO_PIN     GPIO_PIN_1


typedef enum
{
    RemotePin
	
}Remote_pin_T;



//#define remote_pin (GPIOB->IDR & GPIO_PIN_5)

#define rise_fall_min 1075    // 1222 Is the standard value , 10% less

#define rise_fall_max 1550   // 1222 Is the standard value 15% more

#define pack_distance_min 32000    // 34000 Is the standard value , 6% less(2700)

#define pack_distance_max 36000   // 36000 Is the standard value 6% more(2700)

#define packet_bit_num 9  // total bit in packet

//////////////////////////Walton Remote Button/////////////////////////
#define rmtPS         31  // New ON/OFF
#define rmtSpeed		  29	//rmtNatural  29 // New Speed
#define rmtTimer      30	//#define rmtLight    30 // New Timer
//#define rmtLight    27  // New Light
#define RMT_BTN_light 27  //

///////////////////////////////////////////////////////////////////////

#define remote_init     0b00000001      // remote signal interrupt flag
#define decode_enable   0b00000100      // recived signal decoding flag
#define keyRepeat       0b00001000      // Key repeat

#define RemoteBoot      0b10000000      // Key repeat
#define RepeatTimeOver  40000           // Remort key repeat time over

#define sync   1                        // Synchronous flag

///////////////////////////////////////////////////////////////////////////
//remote.status To identify the reception status of the remote control 
//[7]:
//[6]: 
//[5]:   
//[4]: 
//[3:]: Key repeat 
//[2:]: recived signal decoding flag
//[1:]: remote signal synchronizing flag 
//[0:]: remote signal interrupt flag 

/////////////////////////////////////////////////////////////////////////
typedef struct 				// The fields are ordered to reduce memory over caused by struct-padding
{
  uint8_t  keyCnt;      		// repeated key while pressing button of remote for long time.
	uint8_t  status;         // To identify the reception status of the remote control 
  uint16_t packet1;   		  // key value store in integer number system	
	uint16_t packet2;   		  // key value store in integer number system
	uint16_t fall_fall_cnt;  // falling edge to falling edge interrupt timer counter
	
}ir;



/////////////////////////////////////////////////
extern  ir remote;

///////////Remote receive////////////////
/** Key button */
void Remote_Init(void);
uint8_t Remote_ReadpinState(Remote_pin_T pin_num);


//////////////////////////////////////////////////

uint8_t MSBtoLSB(uint8_t number);

//////////////////////////////////////////////////


////////////////////////////////////////////////////
//RmtAddress: To get remote address
//RmtKeyValue: To get keyPressed value

void scan_remote(uint8_t* RmtAddress, uint8_t* RmtKeyValue);

#endif
