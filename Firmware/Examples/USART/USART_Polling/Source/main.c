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

#include "main.h"
#include "Board.h"
#include "apm32f00x_gpio.h"
#include "apm32f00x_misc.h"
#include "apm32f00x_eint.h"
#include "apm32f00x_usart.h"
#include "apm32f00x_rcm.h"

#define DATA_BUF_SIZE       (32)

uint8_t rxDataBufUSART1[DATA_BUF_SIZE] = {0};
uint8_t txDataBufUSART2[DATA_BUF_SIZE] = {0};

/** LED Init */
void LedInit(void);
/** USART Init */
void USARTInit(void);
/** Delay */
void Delay(uint32_t count);
/** Data buffer init */
void DataBufInit(void);
/** Compares two buffers */
BOOL BufferCompare(uint8_t *buf1, uint8_t *buf2, uint8_t size);

/*!
 * @brief       Main program
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
int main(void)
{
    uint8_t i;
    BOOL state = TRUE;

    DataBufInit();
    LedInit();
    USARTInit();
	
	
    /** LED2 GPIO configuration */
	  GPIO_Config_T gpioConfig;
    gpioConfig.mode = GPIO_MODE_OUT_PP;
    gpioConfig.pin = BOARD_LED2_GPIO_PIN;
    gpioConfig.speed = GPIO_SPEED_10MHz;
    GPIO_Config(BOARD_LED2_GPIO_PORT, &gpioConfig);

    for(i = 0; i < DATA_BUF_SIZE; i++)
    {
        /** Wait until end of transmission */
        //while(USART_ReadStatusFlag(USART2, USART_FLAG_TXBE) == RESET);
        //USART_TxData8(USART2, txDataBufUSART2[i]);

        /** Wait until end of reception */
        //while(USART_ReadStatusFlag(USART1, USART_FLAG_RXBNE) == RESET);
       // rxDataBufUSART1[i] = USART_RxData8(USART1);
    }

    /** Verify data */
   // state = BufferCompare(rxDataBufUSART1, txDataBufUSART2, DATA_BUF_SIZE);

    while(1)
    {
        /** Data is ok then turn off LED2 */
        if(state == TRUE)
        {
            //Board_LedOff(BOARD_LED2);
        }
        else
        {
           // Board_LedToggle(BOARD_LED2);
            //Board_LedToggle(BOARD_LED3);
            //Delay(0x7ffff);
        }
				Delay(0x7ffff);
				GPIO_Toggle(BOARD_LED2_GPIO_PORT, BOARD_LED2_GPIO_PIN);
				//Board_LedToggle(BOARD_LED2);
				USART_TxData8(USART1, 49);				
				
    }
}

/*!
 * @brief       Led Init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void LedInit(void)
{
    Board_LedInit(BOARD_LED2);
    Board_LedInit(BOARD_LED3);

    Board_LedOn(BOARD_LED2);
    Board_LedOn(BOARD_LED3);
}

/*!
 * @brief       USART Init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void USARTInit(void)
{
    USART_Config_T usartConfig;

    /**  BaudRate is 9600 */
    usartConfig.baudRate = 9600;
    /**  No interrupt */
    usartConfig.interrupt = USART_INT_NONE;
    /**  Enable receiver */
    //usartConfig.mode = USART_MODE_TX;
    /**  Parity disable */
    usartConfig.parity = USART_PARITY_NONE;
    /**  One stop bit */
    usartConfig.stopBits = USART_STOP_BIT_1;
    /**  Word length is 8bit */
    usartConfig.wordLength = USART_WORD_LEN_8B;
    /**  USART1 configuration */
    //USART_Config(USART1, &usartConfig);

    /**  Enable transmitter */
    usartConfig.mode = USART_MODE_TX;
    /**  USART2 configuration */
    USART_Config(USART1, &usartConfig);

    /**  Enable USART1 */
    USART_Enable(USART1);
    /**  Enable USART2 */
   // USART_Enable(USART2);
}

/*!
 * @brief       Delay
 *
 * @param       count:  delay count
 *
 * @retval      None
 *
 * @note
 */
void Delay(uint32_t count)
{
    volatile uint32_t delay = count;

    while(delay--);
}

/*!
 * @brief       Data buffer init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void DataBufInit(void)
{
    uint8_t i;

    for(i = 0; i < DATA_BUF_SIZE; i++)
    {
        txDataBufUSART2[i] = 0xa0 + i;
        rxDataBufUSART1[i] = 0;
    }
}

/*!
 * @brief       Compares two buffers
 *
 * @param       buf1:    First buffer to be compared
 *
 * @param       buf1:    Second buffer to be compared
 *
 * @param       size:    Buffer size
 *
 * @retval      Return TRUE if buf1 = buf2. If not then return FALSE
 *
 * @note
 */
BOOL BufferCompare(uint8_t *buf1, uint8_t *buf2, uint8_t size)
{
    uint8_t i;

    for(i = 0; i < size; i++)
    {
        if(buf1[i] != buf2[i])
        {
            return FALSE;
        }
    }

    return TRUE;
}
