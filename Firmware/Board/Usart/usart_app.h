
 
 
#ifndef USART_APP_H
#define USART_APP_H

#include "stdio.h"
#include "apm32f00x.h"
#include "apm32f00x_usart.h"
#include "apm32f00x_gpio.h"


 
 void Serial_begin(uint32_t);
 void Serial_print_int (uint32_t); //pass integer value to print it on screen
 void Serial_print_char (char); //pass char value to print it on screen 
 void Serial_print_string (char[]); //pass string value to print it 
 void Serial_newline(void); //move to next line
 BOOL Serial_available(void); //check if input serial data available return 1 is yes 
 char Serial_read_char(void); //read the incoming char byte and return it 
 void SerialPrint_NumString(char string[], uint32_t number);
 
  //#ifdef __cplusplus
 //}
 //#endif

 #endif 
