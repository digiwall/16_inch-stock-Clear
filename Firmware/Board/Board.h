/*!
 * @file        Board.h
 *
 * @brief       Header file for Board
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
#ifndef BOARD_H
#define BOARD_H

#define BOARD_APM32F003_MINI

#ifdef BOARD_APM32F003_MINI
#include "Board_APM32F003_MINI/Board_APM32F003_MINI.h"
#endif

#include "Usart/usart_app.h"
#include "Timer/time.h"
#include "delay/delay.h"
#include "Fan/fan.h"
#include "Battery/battery.h"
#include "apm32f00x_tmr4.h"
#include "Remote/remote.h"
#include "Button/button.h"
#include "ADC/adc.h"
#include "utility/utility.h"
#include "Remote/remote.h"
#include "Flash/flash.h"

#endif
