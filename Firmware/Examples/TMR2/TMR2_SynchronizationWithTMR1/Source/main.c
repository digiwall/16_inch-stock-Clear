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
#include "apm32f00x_tmr2.h"

/** LED Init */
void LedInit(void);
/** TMR1 Init */
void TMR1Init(void);
/** TMR2 Init */
void TMR2Init(void);
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
    TMR1Init();
    TMR2Init();

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
    TMR1_TimeBaseConfig_T timeBaseConfig;

    /** Up-counter */
    timeBaseConfig.cntMode = TMR1_CNT_MODE_UP;
    /** Set counter = 47 */
    timeBaseConfig.count = 47;
    /** Set divider = 0 */
    timeBaseConfig.divider = 0;
    /** Repetition counter = 0x0 */
    timeBaseConfig.repetitionCount = 0;
    TMR1_ConfigTimerBase(TMR1, &timeBaseConfig);

    /** The update event is selected as trigger output */
    TMR1_ConfigMasterMode(TMR1, TMR1_MASTER_MODE_UPDATE);

    /**  Enable TMR1  */
    TMR1_Enable(TMR1);
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

    /** Set counter = 0XFF, divider = 0 */
    TMR2_ConfigTimerBase(0, 0XFF);
    /** IT1 connected to TMR1 TRGOUT  */
    TMR2_ConfigInputTrigger(TMR2_ITC_TMR1);
    /** Slave mode is External clock mode 1  */
    TMR2_ConfigSlaveMode(TMR2_SLAVE_MODE_EXTERNAL1);

    /** CH2 output wave that period = 256us, duty = 1 */
    ocConfigStruct.channel = TMR2_CHANNEL_2;
    ocConfigStruct.count = 0X7F;
    ocConfigStruct.mode = TMR2_OC_MODE_PWM1;
    ocConfigStruct.OCxOutputState = TMR2_OC_OUTPUT_ENABLE;
    ocConfigStruct.OCxPolarity = TMR2_OC_POLARITY_HIGH;
    TMR2_ConfigOutputCompare(&ocConfigStruct);

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
