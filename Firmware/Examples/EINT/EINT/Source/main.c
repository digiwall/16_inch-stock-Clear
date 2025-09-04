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

/** Delay */
void Delay(void);
/** Led Init */
void LedInit(void);
/** Key Init */
void KeyInit(void);
/** Process when reset occured */
void ResetProc(void);

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
    LedInit();
    KeyInit();

    while(1)
    {
    }
}

/*!
 * @brief       Led init
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
 * @brief       Key init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void KeyInit(void)
{
    Board_KeyInit(BOARD_KEY1, ENABLE);

    NVIC_EnableIRQRequest(EINTD_IRQn, 0X01);
}

/*!
 * @brief       Delay
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void Delay(void)
{
    volatile uint32_t delay = 0xffff5;

    while(delay--)
    {}
}

/*!
 * @brief       Key process
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void KeyProc(void)
{
    Board_LedToggle(BOARD_LED2);
    Board_LedToggle(BOARD_LED3);

    EINT_ClearIntFlag(EINT_PORT_D);
}
