#include "remote.h"
#include "apm32f00x_gpio.h"
#include "apm32f00x_eint.h"

ir remote; // object of ir variable

/*!
 * @brief       Remote GPIO and EINT configuration
 *
 * @param       pin_num:        Specifies the Key button to be configured
 *
 * @param       intEnable:  Enable or Disable external interrupt.
 *                          It can be ENABLE or DISABLE.
 *
 * @retval      None
 *
 * @note
 */

void Remote_Init(void){  

  GPIO_Config_T gpioConfig;
	
  //EINT_PORT_T eintPortTab = {EINT_PORT_A};

  gpioConfig.mode = GPIO_MODE_IN_PU;
  gpioConfig.pin =  GPIO_PIN_1;
  gpioConfig.intEn = GPIO_EINT_ENABLE;
 
  EINT_Config(EINT_PORT_A, EINT_TRIGGER_FALLING_RISING);

  GPIO_Config(GPIOA, &gpioConfig);
	
	NVIC_EnableIRQRequest(EINTA_IRQn, 0X01);
	
	remote.status = 0;
}



//////////////////////////////////////////////////
//Scan IR keyboard 
//RmtAddress: To get remote address
//RmtKeyValue: To get keyPressed value


void scan_remote(uint8_t* RmtAddress, uint8_t* RmtKeyValue){      
    uint8_t  address1, address2;        // address_1 is the first 8 bit of message and InvAddress_2is the second 8(inverting of first 8 bit) bit of message for basic NEC Protocol.		

	 
	 if (remote.status & decode_enable){ 
     ///////////////////get address from message//////////////
			//address    =   remote.received>>24;	     // Get the address code (u32 Of Recived Code shift right 24 position , Keep only the address code 8 position ) Unsigned 
			 address1   =   remote.packet1>>6;
			 address2   =   remote.packet2>>6;

	
		 // Serial_newline();
			
			//remote.packet2 = remote.packet2 & 0x7FF;
			//Serial_print_char('q');					
			//Serial_print_int (remote.packet1);		
		  //Serial_newline();
			//remote.packet2 =0;
			
			//////////////////////////////////////////////////////////						
		
	    if(address1 == address2){ 					   //For NEC  protocol (8 bit address code)	  
		
			*RmtAddress = address1;    //MSBtoLSB(address);       //  ir keyboard address
			*RmtKeyValue = remote.packet1 & 0x1F;    //MSBtoLSB(data);        //	ir keyboard value		
			
		   if((*RmtKeyValue == 31)&&(remote.packet2>>5==31)){ // unwanted signal generation from remote.
				 
				 *RmtKeyValue = 0;
		 
		   }
			
			 //Serial_print_char('A');					
			 //Serial_print_int (remote.packet1>>5);		
		   //Serial_newline();	

			 //Serial_print_char('B');					
			 //Serial_print_int (remote.packet2>>5);	
      //remote.status = 0;
			//remote.fall_fall_cnt = 0;
			/////////////////////////////////////////


			//Serial_print_char('V');				
			//Serial_print_int (*RmtKeyValue);		
			//Serial_newline();
			
			remote.keyCnt  = 0;
			remote.status = 0;
			remote.packet1 = 0;
			remote.packet2 = 0;
      remote.fall_fall_cnt = 0;	

			///////////////////////////////////////////
			
	    }else{		
	       *RmtKeyValue	= 0;
					remote.keyCnt  = 0;
					remote.status = 0;
					remote.packet1 = 0;
					remote.packet2 = 0;
					remote.fall_fall_cnt = 0;
			}
	}
	
	  //Serial_print_int (remote.fall_fall_cnt);
	if(remote.fall_fall_cnt > RepeatTimeOver){	// Reinit variable and disable timer interrupt
			remote.keyCnt  = 0;
			remote.status = 0;
			remote.packet1 = 0;
			remote.packet2 = 0;
      remote.fall_fall_cnt = 0;			
			//Serial_print_char('x');
			//TIM4_Cmd(DISABLE);
			//TIM4_ITConfig(TIM4_IT_UPDATE, DISABLE);
	}

 }






