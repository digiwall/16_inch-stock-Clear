/*!
 * @file        main.c
 *
 * @brief       Main program body
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

#include "main.h"
#include "Board.h"
#include "apm32f00x_gpio.h"

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
    SCB->VTOR = VECT_USR_APP_TAB_OFFSET;
    
    LED_Init();

    while(1)
    {
        Delay();
        GPIO_Toggle(BOARD_LED2_GPIO_PORT, BOARD_LED2_GPIO_PIN);
    }
}

/*!
 * @brief       LED Init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void LED_Init(void)
{
    GPIO_Config_T gpioConfig;
    
    /* LED2 GPIO configuration */
    gpioConfig.mode = GPIO_MODE_OUT_OD;
    gpioConfig.pin = BOARD_LED2_GPIO_PIN;
    gpioConfig.speed = GPIO_SPEED_10MHz;
    GPIO_Config(BOARD_LED2_GPIO_PORT, &gpioConfig);
    
    gpioConfig.pin = BOARD_LED3_GPIO_PIN;
    GPIO_Config(BOARD_LED3_GPIO_PORT, &gpioConfig);

    /* Turn LED2 on */
    GPIO_ClearBit(BOARD_LED2_GPIO_PORT, BOARD_LED2_GPIO_PIN);
    
    /* Turn LED3 off */
    GPIO_SetBit(BOARD_LED3_GPIO_PORT, BOARD_LED3_GPIO_PIN);
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
