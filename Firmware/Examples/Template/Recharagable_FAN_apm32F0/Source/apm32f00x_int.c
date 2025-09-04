/*!
 * @file        apm32f00x_int.c
 *
 * @brief       Main Interrupt Service Routines
 *
 * @version     V1.0.1
 *
 * @date        2022-04-11
 *
 * @attention
 *
 *  Copyright (C) 2018-2022 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be usefull and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */
 
 

#include "apm32f00x_int.h"
#include "Remote/remote.h"
#include "main.h"

BOOL Sleepflag = 0;
BOOL init = 0;
BOOL BTNpower_key = 0;
BOOL Charge_detector = 0; 
BOOL remotSleepflag = 0;
BOOL sleepRMTinit = 0;


static uint8_t tick = 0; // micros second

//////////////////////////////////////////////
//----------------------------Remote---------------------------------------------------  
static uint16_t RawVal = 0;		    //It is timer counter at the falling to falling edge 
static uint8_t  RmtKeyCnt = 0;	    //The number of key presses 
static uint8_t  pulseCount = 9;
static BOOL rmt_flag = FALSE;
static BOOL packet2 = FALSE;



//////////////////////////////////////////////////////////////

 /*!
  * @brief       This function handles NMI exception
  *
  * @param       None
  *
  * @retval      None
  *
  * @note
  */
 void NMI_Handler(void)
 {
 }

/*!
 * @brief       This function handles Hard Fault exception
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void HardFault_Handler(void)
{

}

/*!
 * @brief       This function handles SVCall exception
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void SVC_Handler(void)
{
}

/*!
 * @brief       This function handles PendSV_Handler exception
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void PendSV_Handler(void)
{
}

/*!
 * @brief       This function handles SysTick exception
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void SysTick_Handler(void)
{

	     
}


/*!
 * @brief       This function handles TMR4 exception
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void TMR4_IRQHandler(void)
{
				//TMR4Isr(); 
				tick++;
        //if(tick == 1 || tick == 4 || tick == 8 || tick == 12){				
				   remote.fall_fall_cnt = remote.fall_fall_cnt + 85;//255; //falling edge to falling edge conversion
	         //Serial_print_char('p');
				   //Serial_print_int(remote.fall_fall_cnt);
	         //Serial_newline();	
          //remote.fall_fall_cnt = 0;
		       TMR4->CNT = (uint8_t)(0);
				//}
        			
			  //Timer
		    //micros++;
	      if(tick >= 12){ miliseconds++;	 tick = 0;} // 12 micros = 1 milisecond	
				
				TMR4_ClearIntFlag(TMR4_INT_UPDATE);				
    
}

/*!
 * @brief       This function handles EINTD exception
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void EINTA_IRQHandler(void)
{
    //Remote();
	  
	 // remoteARRAY[list++] = TMR4->CNT + remote.fall_fall_cnt;
	  //remote.fall_fall_cnt = 0;
	  //TMR4->CNT = 0;
	  //if(list >= 60){list = 0;}
		
					//Serial_print_char('p');
				  //Serial_print_int(TMR4->CNT + remote.fall_fall_cnt);
	        //Serial_newline();	
          //remote.fall_fall_cnt = 0;	
	
	if(remote.status & remote_init){
		
		RawVal = TMR4->CNT + remote.fall_fall_cnt;		
		TMR4->CNT = 0;                   //Timer4 Counter zero initializing 
		remote.fall_fall_cnt = 0;   		  //remote.fall_fall_cnt zero initializaing
		
	  if(rmt_flag == TRUE){  						//Conversion start at Synchronous stage
		
      if((RawVal > rise_fall_min && RawVal < rise_fall_max) && (!(GPIOA->DIN & (uint8_t)0x00000002))){ // 2247 Is the standard value , 1687.5us + 560us between  20% more(2700) or less(1800)								 

						if(packet2 == FALSE){
						   remote.packet1 |=(uint16_t)1<<(pulseCount);  // Received 1
               
					  }else{
					     remote.packet2 |=(uint16_t)1<<(pulseCount);  // Received 1
					  }
					     pulseCount--;
							 								 
			}
			else{
						if(RawVal>rise_fall_min && RawVal<rise_fall_max){ // 2247 Is the standard value , 1687.5us + 560us between  20% more(2700) or less(1800)			 
               
						  pulseCount--;						
				    }		 
			}
				
			if(RawVal>pack_distance_min && RawVal<pack_distance_max){        //          

					packet2 = TRUE; // second packet converting flag
					pulseCount  = packet_bit_num + 2; // For Second packet with start bit and end bit
					remote.packet2 	= 0;
					
		  }else if(pulseCount == 0 && packet2 == TRUE){
					remote.status|=decode_enable;	//enable signal decoding
          rmt_flag = FALSE;
					packet2 = FALSE;
					pulseCount  = packet_bit_num;
					//Serial_print_char('p');
				  //Serial_print_int(remote.packet1);
					//Serial_print_char('Q');
					remote.packet2 = remote.packet2 >> 1;
				  //Serial_print_int(remote.packet2);
		  }//			
	  }			
		
	
		
	if(rmt_flag == FALSE){
		 
			if((RawVal > rise_fall_min && RawVal < rise_fall_max)&&(GPIOA->DIN & (uint8_t)0x00000002)){    // 4.5ms+9ms = 13500us(	10% more(12000) orless(14000)) 	                 	
		
				rmt_flag = TRUE;	// Synchronous bit has been captured 
        packet2	= FALSE;			
				//Serial_print_char('T');				
			}	
		}			
  }
	
	if (!(remote.status & remote_init)){  //intialize all veriable at first remote signal interrupt 0b00000001
		      TMR4->CNT = 0;
			    remote.fall_fall_cnt = 0;
					pulseCount = packet_bit_num;
					rmt_flag = FALSE;
					remote.packet1 = FALSE;
					remote.packet2 	= FALSE;
					remote.keyCnt  = 0;	
          packet2 = FALSE;					
          remote.status |= remote_init;
			    //TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
			    //TIM4_Cmd(ENABLE);						
          //Serial_print_char('k');
          //Serial_print_int(rmt_flag);
		}
	 EINT_ClearIntFlag(EINT_PORT_A);
	 
		if(remotSleepflag == TRUE){  // WakeUp Flag is true , this flag set in sleep mode
		    remotSleepflag = FALSE;  // This flag will be active once
        sleepRMTinit = TRUE;		 //	
		  }	
 }


void EINTC_IRQHandler(void)
{	
		btn_power_button();	
	  //if(Sleepflag == TRUE){
		    //Sleepflag = FALSE;
        //init = TRUE;			
		//}
		EINT_ClearIntFlag(EINT_PORT_C);
}

void EINTB_IRQHandler(void)
{
 	  if((GPIO_ReadInputBit(Charging_input_Port, Charging_input_pin))){	 

				Charge_detector = TRUE;
			  AnyChange = TRUE;
        init = TRUE;			
		}
		delay_ms(100);
		EINT_ClearIntFlag(EINT_PORT_B);
}

