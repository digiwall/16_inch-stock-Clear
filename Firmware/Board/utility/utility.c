/*
 * utility.c
 *
 *  Created on: Apr 10, 2022
 *      Author: USER
 */

#include "utility.h"

 uint8_t highByte(uint16_t value){

	 return (uint8_t)(value>>8);
 }

 uint8_t lowByte(uint16_t value){

	 return (uint8_t)(value & 0xFF);
 }

 // set bit or clear bit in specific position of intiger value

uint8_t bitWrite(uint8_t value, uint8_t bit_num, uint8_t bit_value){


	 if(bit_value != 0){

		 value |= 1 << bit_num; // bit set
	  }

	 if(bit_value != 1){

		 value &= ~(1 << bit_num);//bit clear
	 }

   return value;
 }

 uint8_t bitSet(uint8_t value, uint8_t bit_state){

		 value |= bit_state; // bit set
		 
   return value;
 }
 
 uint8_t bitClear(uint8_t value, uint8_t bit_state){

		 value &= (uint8_t)(~bit_state);//bit clear
		 
						//Serial_print_char('K');
						//Serial_print_int((uint8_t)(bit_state));
						//Serial_newline();
						//Serial_print_int((uint8_t)(value));
						//Serial_newline();
						

   return value;
 }

 // to find bit in specific position of value
 BOOL bitRead(uint8_t value, uint8_t find_bit){

	 	    //uint8_t leftshift  = 1 << find_bit;
				
	 	    //uint8_t bitread = find_bit & value;
				
	 	    //bitread = bitread >> find_bit;
	 	    //return bitread && 0x01;
			if(find_bit & value){
			
			  return 1;
			}
		  else{
		    return 0;
		  }
 }




