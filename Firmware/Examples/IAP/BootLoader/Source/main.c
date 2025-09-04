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
#include <stdio.h>

uint32_t JumpAddress;
pFunction Jump_To_Application;


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
    SCB->VTOR = VECT_USR_BOOT_TAB_OFFSET;
    
    LED_Init();
    
    USART_Init(DEBUG_USART_BAUDRATE);
    
    /** Configure the Flash to re-program the Flash */
    FLASH_IAP_Init();
    
    /** Display Select menu */
    Select_Menu();

    /** Jump to user application 1 */
    SendString(">> Jump to user application 1\r\n");
    Jump_to_App(APP1);
    
    while(1)
    {

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

    /* LED3 GPIO configuration */
    gpioConfig.pin = BOARD_LED3_GPIO_PIN;
    GPIO_Config(BOARD_LED3_GPIO_PORT, &gpioConfig);

    /* Turn LED2 on */
    GPIO_ClearBit(BOARD_LED2_GPIO_PORT, BOARD_LED2_GPIO_PIN);
    /* Turn LED3 off */
    GPIO_ClearBit(BOARD_LED3_GPIO_PORT, BOARD_LED3_GPIO_PIN);
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
 * @brief       Jump to user application
 *
 * @param       Application : APP1 or APP2
 *
 * @retval      None
 *
 * @note
 */
void Jump_to_App(uint8_t Application)
{
    uint32_t address;

    /** Lock the Program memory */
    FMC_Lock();


    if(Application == APP1)
    {
        address = USER_APP1_ADDRESS;
    }
    else
    {
        address = USER_APP2_ADDRESS;
    }

    /** Jump to user application */
    JumpAddress = *(__IO uint32_t *) (address + 4);
    Jump_To_Application = (pFunction) JumpAddress;

    /** Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t *) address);

    /** Jump to application */
    Jump_To_Application();
}
