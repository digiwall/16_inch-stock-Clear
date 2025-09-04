/*!
 * @file        bsp_usart.c
 *
 * @brief       Usart board support package body
 *
 * @version     V1.0.0
 *
 * @date        2022-04-24
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

#include "bsp_usart.h"
#include <stdio.h>

/*!
 * @brief       Redirect printf function
 *
 * @param       ch:  The characters that need to be send.
 *
 * @param       *f:  pointer to a FILE that can recording all information
 *              needed to control a stream
 *
 * @retval      The characters that need to be send.
 *
 */
int fputc(int ch, FILE* f)
{
    /** send a byte of data to the serial port */
    USART_TxData8(DEBUG_USART, (uint8_t)ch);

    /** wait for the data to be send  */
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);

    return (ch);
}

/*!
 * @brief       USART Init
 *
 * @param       baudRate
 *
 * @retval      None
 *
 * @note
 */
void USART_Init(uint32_t baudRate)
{
    USART_Config_T usartConfig;
    
    RCM_EnableAPBPeriphClock(DEBUG_USART_CLK);
    
    /**  BaudRate */
    usartConfig.baudRate = baudRate;
    /**  No interrupt */
    usartConfig.interrupt = USART_INT_NONE;
    /**  Enable receiver */
    usartConfig.mode = USART_MODE_TX_RX;
    /**  Parity disable */
    usartConfig.parity = USART_PARITY_NONE;
    /**  One stop bit */
    usartConfig.stopBits = USART_STOP_BIT_1;
    /**  Word length is 8bit */
    usartConfig.wordLength = USART_WORD_LEN_8B;
    /**  USART1 configuration */
    USART_Config(DEBUG_USART, &usartConfig);
    
    /**  Enable USART */
    USART_Enable(DEBUG_USART);
}

/*!
 * @brief       USART Send byte
 *
 * @param       usart
 *
 * @param       data
 *
 * @retval      None
 *
 * @note
 */
void USART_SendByte(USART_T *usart,uint8_t data)
{
    while(USART_ReadStatusFlag(usart, USART_FLAG_TXBE) == RESET);
    USART_TxData8(usart, data);
}

/*!
 * @brief       USART Receive byte
 *
 * @param       usart
 *
 * @retval      receive data
 *
 * @note
 */
uint8_t USART_ReceiveByte(USART_T *usart)
{
    uint8_t revData;
    
    while(USART_ReadStatusFlag(usart, USART_FLAG_RXBNE) == RESET);
    revData = USART_RxData8(usart);
    
    return revData;
}
