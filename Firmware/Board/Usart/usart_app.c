  #include "usart_app.h"

  void Serial_begin(uint32_t buadrate){
	
	  USART_Config_T usartConfig;


    /**  BaudRate is 9600 */
    usartConfig.baudRate = buadrate;
    /**  No interrupt */
    usartConfig.interrupt = USART_INT_NONE;
    /**  Enable RX and TX */
    usartConfig.mode = USART_MODE_TX_RX;
    /**  Parity disable */
    usartConfig.parity = USART_PARITY_NONE;
    /**  One stop bit */
    usartConfig.stopBits = USART_STOP_BIT_1;
    /**  Word length is 8bit */
    usartConfig.wordLength = USART_WORD_LEN_8B;
    /**  USART1 configuration */
    USART_Config(USART1, &usartConfig);

    /**  Enable USART1 */
    USART_Enable(USART1);
	
	}
	

void SerialPrint_NumString(char string[], uint32_t number){
	
  Serial_print_string (string);	
	Serial_print_char (' ');
	Serial_print_int (number);
	
  Serial_newline();
}

 void Serial_print_char (char value)
 {
	
	 USART_TxData8(USART1, value);
	 while (USART_ReadStatusFlag(USART1, USART_FLAG_TXBE) == RESET); //wait for sending
 }
 
 
	
void Serial_print_int (uint32_t number) //Funtion to print int value to serial monitor 
 {
	 uint16_t count = 0;
	 char digit[5] = "";
	 
	 while (number != 0) //split the int to char array 
	 {
		 digit[count] = number%10;
		 count++;
		 number = number/10;
	 }
	 
	 while (count !=0) //print char array in correct direction 
	 {
		//UART1_SendData8(digit[count-1] + 0x30);
		USART_TxData8(USART1, digit[count-1] + 0x30);
		while (USART_ReadStatusFlag(USART1, USART_FLAG_TXBE) == RESET); //wait for sending 
		count--; 
	 }
 }
 
 
 void Serial_newline(void)
 {
	 
	 USART_TxData8(USART1, 0x0a);
	 while (USART_ReadStatusFlag(USART1, USART_FLAG_TXBE) == RESET); //wait for sending 
 }
 

 
 
 void Serial_print_string (char string[])
 {
	 char i=0;

	 while (string[i] != 0x00)
	 {
		
	   USART_TxData8(USART1,string[i]);
	   while (USART_ReadStatusFlag(USART1, USART_FLAG_TXBE) == RESET); //wait for sending 
		 i++;
	}
 }
 
 
 
	

 
