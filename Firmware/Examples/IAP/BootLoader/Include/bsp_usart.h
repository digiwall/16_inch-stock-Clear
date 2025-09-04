/*!
 * @file        bsp_usart.h
 *
 * @brief       Header for bsp_usart.c module
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

#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "main.h"

#define DEBUG_USART                         USART1
#define DEBUG_USART_CLK                     RCM_PERIPH_USART1
#define DEBUG_USART_BAUDRATE                115200


/** function declaration*/
void USART_Init(uint32_t baudRate);
void USART_SendByte(USART_T *usart,uint8_t data);
uint8_t USART_ReceiveByte(USART_T *usart);
#endif
