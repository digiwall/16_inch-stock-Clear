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
#include "apm32f00x_eint.h"
#include "apm32f00x_tmr4.h"
#include "apm32f00x_rcm.h"
#include "apm32f00x_misc.h"

/** Timer tick */
volatile uint32_t tick = 0;

/** LED Init */
void LedInit(void);
/** TMR4 Init */
void TMR4Init(void);

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
    /** Master clock frequency = 48 / 8 = 6MHZ */
    RCM_ConfigHIRCDiv(RCM_HIRC_DIV_8);

    LedInit();
    TMR4Init();

    while(1)
    {
        if(tick >= 100)
        {
            tick = 0;

            Board_LedToggle(BOARD_LED2);
            Board_LedToggle(BOARD_LED3);
        }
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
 * @brief       TMR4 Init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void TMR4Init(void)
{
    /**  Divider = 7, counter = 0xff  */
    TMR4_ConfigTimerBase(7, 0XFF);
    /**  Enable update interrupt  */
    TMR4_EnableInterrupt(TMR4_INT_UPDATE);
    /**  Enable TMR4  */
    TMR4_Enable();

    NVIC_EnableIRQRequest(TMR4_IRQn, 0X01);
}

/*!
 * @brief       TMR4 interrupt service runtine
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void TMR4Isr(void)
{
    if(TMR4_ReadIntFlag(TMR4_INT_UPDATE) == SET)
    {
        TMR4_ClearIntFlag(TMR4_INT_UPDATE);

        tick++;
    }
}
