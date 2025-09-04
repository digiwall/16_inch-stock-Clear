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

/** GPIOA Init */
void GPIOA_Init(void);
/** GPIOA Toggle */
void GPIOA_Toggle(void);
/** Delay */
void Delay(void);

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
    GPIOA_Init();

    RCM_SwitchClockStruct_T switchClockStruct;
    /**  switch HIRC as master clock  */
    switchClockStruct.clockSource = RCM_MCLKSRC_HIRC;
    /**  Close the current clock if switching succeed.In this case,Close the HXT clock */
    switchClockStruct.currentClkState = RCM_CLOCK_STATE_OFF;
    /**  Disable interrupt when switching is completed */
    switchClockStruct.intEnable = DISABLE;
    /**  Automatic switching */
    switchClockStruct.mode = RCM_SWITCH_MODE_AUTO;
    RCM_ConfigClockSwitch(&switchClockStruct);

    GPIO_Config_T gpioConfig;

    /** LED2 GPIO configuration */
    gpioConfig.mode = GPIO_MODE_OUT_OD;
    gpioConfig.pin = BOARD_LED2_GPIO_PIN;
    gpioConfig.speed = GPIO_SPEED_10MHz;
    GPIO_Config(BOARD_LED2_GPIO_PORT, &gpioConfig);

    /** LED3 GPIO configuration */
    gpioConfig.pin = BOARD_LED3_GPIO_PIN;
    GPIO_Config(BOARD_LED3_GPIO_PORT, &gpioConfig);

    /** Turn LED2 on */
    GPIO_ClearBit(BOARD_LED2_GPIO_PORT, BOARD_LED2_GPIO_PIN);
    /** Turn LED3 off */
    GPIO_SetBit(BOARD_LED3_GPIO_PORT, BOARD_LED3_GPIO_PIN);

    while(1)
    {
        Delay();

        GPIOA_Toggle();
    }
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
void Delay(void)
{
    volatile uint32_t delay = 0xffff5;

    while(delay--)
    {}
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
void GPIOA_Init(void)
{
    GPIO_Config_T gpioConfig;

    gpioConfig.mode = GPIO_MODE_OUT_OD;
    gpioConfig.speed = GPIO_SPEED_10MHz;
    gpioConfig.pin = GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_Config(GPIOA, &gpioConfig);
}

/*!
 * @brief       Toggles the specified GPIOA
 *
 * @param       GPIO_PIN_1 | GPIO_PIN_2
 *
 * @retval      None
 *
 * @note
 */
void GPIOA_Toggle(void)
{
    GPIO_Toggle(GPIOA, GPIO_PIN_1 | GPIO_PIN_2);
}
