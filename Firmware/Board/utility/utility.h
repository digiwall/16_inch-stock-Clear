/*
 * utility.h
 *
 *  Created on: Apr 10, 2022
 *      Author: USER
 */

#ifndef INC_UTILITY_H_
#define INC_UTILITY_H_


#include "stdio.h"
#include "apm32f00x.h"


 uint8_t highByte(uint16_t value); // find  Most significant  byte of 16 bit word

 uint8_t lowByte(uint16_t value);// find least significant byte of 16 bit word

// find specific bit
 uint8_t bitClear(uint8_t value, uint8_t bit_state);
 
 uint8_t bitSet(uint8_t value, uint8_t bit_state);
 
 BOOL bitRead(uint8_t value, uint8_t bit);

 uint8_t bitWrite(uint8_t value, uint8_t bin_num, uint8_t bit_value);// bit set or clear in specific bit

#endif /* INC_UTILITY_H_ */


//(GPIOD->ODR &= (uint8_t)(~LED));