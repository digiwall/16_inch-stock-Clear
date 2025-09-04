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
#include "apm32f00x_tmr2.h"
#include "apm32f00x_fmc.h"

/** Key Init */
void KeyInit(void);
/** LED Init */
void LedInit(void);
/** TMR1 Init */
void TMR1Init(void);
/** TMR2 Init */
void TMR2Init(void);
/** Key process */
BOOL KeyPressed(void);
/** delay */
void Delay(uint32_t count);

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
    TMR2Init();

    while(1)
    {
        if(KeyPressed())
        {
            TMR2_Enable();
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
 * @brief       Key Init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void KeyInit(void)
{
    Board_KeyInit(BOARD_KEY1, DISABLE);
}

/*!
 * @brief       TMR2 Init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void TMR2Init(void)
{
    TMR2_OCConfig_T ocConfigStruct;

    /** Set counter = 0XFF, divider = 4 */
    TMR2_ConfigTimerBase(4, 0XFF);

    /** CH3 output one pulse */
    ocConfigStruct.channel = TMR2_CHANNEL_3;
    ocConfigStruct.count = 0X7F;
    ocConfigStruct.mode = TMR2_OC_MODE_PWM1;
    ocConfigStruct.OCxOutputState = TMR2_OC_OUTPUT_ENABLE;
    ocConfigStruct.OCxPolarity = TMR2_OC_POLARITY_HIGH;
    TMR2_ConfigOutputCompare(&ocConfigStruct);

    TMR2_EnableSinglePulseMode();

    /**  Enable TMR2  */
    TMR2_Enable();
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
 * @brief       Check Key is pressed
 *
 * @param       None
 *
 * @retval      Return TRUE if key has been pressed.If not return FALSE
 *
 * @note
 */
BOOL KeyPressed(void)
{
    BOOL ret = FALSE;

    if(Board_ReadKeyState(BOARD_KEY1) == BIT_RESET)
    {
        Delay(0x7fff);
        if(Board_ReadKeyState(BOARD_KEY1) == BIT_RESET)
        {
            while(Board_ReadKeyState(BOARD_KEY1) == BIT_RESET);

            ret = TRUE;
        }
    }

    return ret;
}
