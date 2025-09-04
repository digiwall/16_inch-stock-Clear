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
#include "apm32f00x_rcm.h"
#include "apm32f00x_fmc.h"

/** LED Init */
void LedInit(void);
/** Key Init */
void KeyInit(void);
/** Delay */
void Delay(uint32_t count);
/** Key pressed */
BOOL KeyPressed(void);
/** Clock output init */
void ClockOutputInit(void);
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
    uint8_t count = 0;
    RCM_SwitchClockStruct_T switchClockStruct;

    LedInit();
    KeyInit();
    ClockOutputInit();

    while(1)
    {
        if(KeyPressed())
        {
            switch(count)
            {
                case 0:
                    /**  switch HXT as master clock  */
                    switchClockStruct.clockSource = RCM_MCLKSRC_HXT;
                    /**  Close the current clock if switching succeed.In this case,Close the HIRC clock */
                    switchClockStruct.currentClkState = RCM_CLOCK_STATE_OFF;
                    /**  Disable interrupt when switching is completed */
                    switchClockStruct.intEnable = DISABLE;
                    /**  Manual switching */
                    switchClockStruct.mode = RCM_SWITCH_MODE_MANUAL;
                    RCM_ConfigClockSwitch(&switchClockStruct);

                    Board_LedOn(BOARD_LED2);
                    Board_LedOff(BOARD_LED3);
                    break;

                case 1:
                    /**  switch HIRC as master clock  */
                    switchClockStruct.clockSource = RCM_MCLKSRC_HIRC;
                    /**  Close the current clock if switching succeed.In this case,Close the HXT clock */
                    switchClockStruct.currentClkState = RCM_CLOCK_STATE_OFF;
                    /**  Disable interrupt when switching is completed */
                    switchClockStruct.intEnable = DISABLE;
                    /**  Automatic switching */
                    switchClockStruct.mode = RCM_SWITCH_MODE_AUTO;
                    RCM_ConfigClockSwitch(&switchClockStruct);

                    Board_LedOn(BOARD_LED3);
                    Board_LedOff(BOARD_LED2);
                    break;

                default:
                    break;
            }

            count++;
            count &= 0X01;
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
 * @brief       Clock output init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void ClockOutputInit(void)
{
    GPIO_Config_T gpioConfig;

    /**  GPIO configuration  */
    gpioConfig.mode = GPIO_MODE_OUT_PP;
    gpioConfig.pin = GPIO_PIN_4;
    gpioConfig.speed = GPIO_SPEED_10MHz;
    GPIO_Config(GPIOC, &gpioConfig);

    /**  master clock freq = 48MHZ / 2 = 24MHZ  */
    RCM_ConfigHIRCDiv(RCM_HIRC_DIV_2);

    /**  master clock as clock output  */
    RCM_ConfigClockOutput(RCM_COCCLK_MASTER);
    RCM_EnableClockOutput();
}
/*!
 * @brief       Delay
 *
 * @param       count: delay count
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
