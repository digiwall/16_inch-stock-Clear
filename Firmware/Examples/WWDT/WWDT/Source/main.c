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
#include "apm32f00x_wwdt.h"
#include "apm32f00x_rcm.h"
#include "apm32f00x_eint.h"

/** WWDT counter */
#define WWDT_COUNTER_VALUE          (110)
/** WWDT window value */
#define WWDT_WINDOW_VALUE           (90)

/** Enable/disable refresh counter */
static BOOL s_enableRefresh = TRUE;

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

    ResetProc();
    /**
        WWDT Clock = 48MHZ;
        Watchdog Window     = (WWDT_COUNTER_VALUE - 63) * 1 step
                            = (110 - 63) * (12288 / 48)
                            = 12.032ms

        Non Allowed Window  = (WWDT_COUNTER_VALUE - WWDT_WINDOW_VALUE) * 1 step
                            = (110 - 90) * (12288 / 48)
                            = 5.12ms
    */
    WWDT_Config(WWDT_COUNTER_VALUE, WWDT_WINDOW_VALUE);

    while(1)
    {
        if(s_enableRefresh == TRUE)
        {
            while(WWDT_ReadCounter() >= WWDT_WINDOW_VALUE);
            WWDT_SetCounter(WWDT_COUNTER_VALUE);
        }
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
    Board_LedOff(BOARD_LED3);
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
    s_enableRefresh = FALSE;

    EINT_ClearIntFlag(EINT_PORT_D);
}

/*!
 * @brief       Reset process
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void ResetProc(void)
{
    uint8_t i;

    if(RCM_ReadResetFlag(RCM_RTS_FLAG_WWDTRST) == SET)
    {
        for(i = 0; i < 10; i++)
        {
            Delay();
            Board_LedToggle(BOARD_LED2);
        }

        RCM_ClearResetFlag(RCM_RTS_FLAG_WWDTRST);
    }
}
