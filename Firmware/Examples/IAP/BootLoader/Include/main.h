/*!
 * @file        main.h
 *
 * @brief       Header for main.c module
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
#ifndef MAIN_H
#define MAIN_H

#include "apm32f00x.h"
#include "Board.h"
#include "apm32f00x_gpio.h"
#include "apm32f00x_misc.h"
#include "apm32f00x_usart.h"
#include "apm32f00x_rcm.h"
#include "apm32f00x_fmc.h"

#include "bsp_usart.h"
#include "bsp_flash.h"
#include "common.h"
#include "ymodem.h"
#include "menu.h"

#define VECT_USR_BOOT_TAB_OFFSET         0x0000

/*!
 * @brief    function hook
 */
typedef  void (*pFunction)(void);

/** extern variables*/
extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;

/** function declaration*/
void Delay(void);
void LED_Init(void);
void Jump_to_App(uint8_t Application);

#endif
