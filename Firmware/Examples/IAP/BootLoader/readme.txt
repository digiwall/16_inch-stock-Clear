/*!
 * @file        readme.txt
 *
 * @brief       This file is routine instruction
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

&par Example Description

The example aim to show how to configure a bootloader firmware to IAP.
When device connet to HyperTerminal right, a usart menu will show to user.
The menu as follow:

***************************************************************
*      APM32F00x In-Application Programming Application       *
*                                              (Version 1.0.0)*
***************************************************************

** Please select an operation item
*  1.Download Flash application 1 -----------------------> 1  *
*  2.Upload Flash application 1   -----------------------> 2  *
*  3.Jump to user application 1   -----------------------> 3  *
*  4.Download Flash application 2 -----------------------> 4  *
*  5.Upload Flash application 2   -----------------------> 5  *
*  6.Jump to user application 2   -----------------------> 6  *
***************************************************************

&par Hardware Description

  - USART1 configured as follow:
  - BaudRate = 115200
  - Word Length = USART_WordLength_8b
  - Stop Bit = USART_StopBits_1
  - Parity = USART_Parity_No
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled

&par Directory contents

  - IAP/BootLoader/Source/apm32f00x_int.c        Interrupt handlers
  - IAP/BootLoader/Source/main.c                      Main program
  - IAP/BootLoader/Source/bsp_usart.c            USART Configuration
  - IAP/BootLoader/Source/bsp_flash.c            Flash handlers
  - IAP/BootLoader/Source/common.c               Common functions
  - IAP/BootLoader/Source/menu.c                 UART Menu
  - IAP/BootLoader/Source/ymodem.c               Ymodem protocol handlers

&par IDE environment

  - MDK-ARM V5.36
  - EWARM V9.20.2.43979

&par Hardware and Software environment

  - This example runs on APM32F003 MINI Devices.
