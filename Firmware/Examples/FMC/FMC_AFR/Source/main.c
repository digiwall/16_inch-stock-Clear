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
#include "apm32f00x_tmr1.h"
#include "apm32f00x_rcm.h"
#include "apm32f00x_fmc.h"

/** LED Init */
void LedInit(void);
/** TMR1 Init */
void TMR1Init(void);
/** GPIO Alternate function config */
void AFConfig(void);
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
    AFConfig();

    LedInit();
    TMR1Init();

    while(1)
    {
        Delay(0x7ffff);
        Board_LedToggle(BOARD_LED2);
        Board_LedToggle(BOARD_LED3);
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
 * @brief       TMR1 Init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void TMR1Init(void)
{
    TMR1_OCConfig_T ocConfigStruct;
    TMR1_TimeBaseConfig_T timeBaseConfig;

    /** Up-counter */
    timeBaseConfig.cntMode = TMR1_CNT_MODE_UP;
    /** Set counter = 0xff */
    timeBaseConfig.count = 0XFF;
    /** Set divider = 47.So TMR1 clock freq ~= 48/(47 + 1) = 1MHZ */
    timeBaseConfig.divider = 47;
    /** Repetition counter = 0x0 */
    timeBaseConfig.repetitionCount = 0;
    TMR1_ConfigTimerBase(TMR1, &timeBaseConfig);

    /** Select channen1 1 */
    ocConfigStruct.channel = TMR1_CHANNEL_1;
    /** Set compare value = 0x7f */
    ocConfigStruct.count = 0x7f;
    /** PWM1 mode */
    ocConfigStruct.mode = TMR1_OC_MODE_PWM1;
    /** Idle State is reset */
    ocConfigStruct.OCxIdleState = TMR1_OC_IDLE_RESET;
    /** Idle State is reset */
    ocConfigStruct.OCxNIdleState = TMR1_OC_IDLE_RESET;
    /** Enable CH1N ouput */
    ocConfigStruct.OCxNOutputState = TMR1_OC_OUTPUT_ENABLE;
    /** Enable CH1 ouput */
    ocConfigStruct.OCxOutputState = TMR1_OC_OUTPUT_ENABLE;
    /** CH1N polarity is high */
    ocConfigStruct.OCxNPolarity = TMR1_OC_POLARITY_HIGH;
    /** CH1 polarity is high */
    ocConfigStruct.OCxPolarity = TMR1_OC_POLARITY_HIGH;
    TMR1_ConfigOutputCompare(TMR1, &ocConfigStruct);

    /** Set dead time */
    TMR1_SetDeadTime(TMR1, 100);
    /** Disable output when CHx output closed */
    TMR1_SelectOffStateInRunMode(TMR1, TMR1_OSRM_DISABLE);
    /** Disable output when CHxN output closed */
    TMR1_SelectOffStateInIdleMode(TMR1, TMR1_OSIM_DISABLE);

    /** Enable PWM output */
    TMR1_EnableOutputPWM(TMR1);

    /**  Enable TMR1  */
    TMR1_Enable(TMR1);
}

/*!
 * @brief       GPIO Alternate function config.
 *              Set PC6 alternate function = TMR1_CH1.
 *              Set PC3 alternate function = TMR1_CH1N.
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void AFConfig(void)
{
    FMC_AFRConfig_T AFRConfig;

    /** Set PC6 alternate function = TMR1_CH1 */
    AFRConfig.AFR0 = FMC_OB_AFR_ENABLE;
    AFRConfig.AFR1 = FMC_OB_AFR_DISABLE;
    AFRConfig.AFR3 = FMC_OB_AFR_DISABLE;
    AFRConfig.AFR4 = FMC_OB_AFR_DISABLE;
    AFRConfig.AFR5 = FMC_OB_AFR_DISABLE;
    /** Set PC3 alternate function = TMR1_CH1N */
    AFRConfig.AFR7 = FMC_OB_AFR_ENABLE;

    FMC_Unlock();
    FMC_EraseOptionByte();
    FMC_ConfigOptionByteAFR(&AFRConfig);
    FMC_Lock();
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
