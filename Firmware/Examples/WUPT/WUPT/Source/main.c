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
#include "apm32f00x_wupt.h"
#include "apm32f00x_tmr1.h"
#include "apm32f00x_rcm.h"

/** LED Init */
void LedInit(void);
/** Key init */
void KeyInit(void);
/** Measure the LIRC frequency */
uint32_t LIRCMeasurment(void);
/** WUPT init */
void WUPTInit(void);
/** Delay */
void Delay(void);
/** KeyProcess */
void KeyProc(void);

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
    WUPTInit();

    while(1)
    {
        KeyProc();
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
 * @brief       Measure the LIRC frequency
 *
 * @param       None
 *
 * @retval      LIRC frequency in HZ
 *
 * @note
 */
uint32_t LIRCMeasurment(void)
{
    uint16_t icValue[2];
    uint32_t freqLIRC;
    uint32_t freqMasterClk;
    TMR1_ICConfig_T icConfig;

    freqMasterClk = RCM_GetMasterClockFreq();

    WUPT_EnableLIRCMeasurement();

    icConfig.channel = TMR1_CHANNEL_1;
    icConfig.div = TMR1_IC_DIV_8;
    icConfig.filter = 0;
    icConfig.polarity = TMR1_IC_POLARITY_FALLING;
    icConfig.selection = TMR1_IC_SELECT_DIRECTTI;
    TMR1_ConfigInputCapture(TMR1, &icConfig);
    TMR1_Enable(TMR1);

    while(TMR1_ReadStatusFlag(TMR1, TMR1_FLAG_CH1CC) == RESET);
    icValue[0] = TMR1_ReadCompareCapture(TMR1, TMR1_CHANNEL_1);
    TMR1_ClearStatusFlag(TMR1, TMR1_FLAG_CH1CC);

    while(TMR1_ReadStatusFlag(TMR1, TMR1_FLAG_CH1CC) == RESET);
    icValue[1] = TMR1_ReadCompareCapture(TMR1, TMR1_CHANNEL_1);
    TMR1_ClearStatusFlag(TMR1, TMR1_FLAG_CH1CC);

    TMR1_DisableCompareCapture(TMR1, TMR1_CHANNEL_1);
    TMR1_Disable(TMR1);

    freqLIRC = (8 * freqMasterClk) / (icValue[1] - icValue[0]);

    WUPT_DisableLIRCMeasurement();

    return freqLIRC;
}

/*!
 * @brief       WUPT init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void WUPTInit(void)
{
    uint32_t freqLIRC = 0;

    freqLIRC = LIRCMeasurment();
    WUPT_CalibrationLIRC(freqLIRC);
    WUPT_Config(AWU_TIMEBASE_2S);

    NVIC_EnableIRQRequest(WUPT_IRQn, 0X01);
}

/*!
 * @brief       WUPT interrupt service routine
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void WUPTIsr(void)
{
    if(WUPT_ReadIntFlag() == SET)
    {
        Board_LedOn(BOARD_LED2);
    }
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
    uint8_t i;

    if(Board_ReadKeyState(BOARD_KEY1) == BIT_RESET)
    {
        while(Board_ReadKeyState(BOARD_KEY1) == BIT_RESET);

        for(i = 0; i < 10; i++)
        {
            Delay();
            Board_LedToggle(BOARD_LED2);
            Board_LedToggle(BOARD_LED3);
        }
        Board_LedOff(BOARD_LED2);
        Board_LedOff(BOARD_LED3);

        PMU_EnterHaltModeWFI();

        Board_LedOff(BOARD_LED2);
        Board_LedOn(BOARD_LED3);
    }
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
    volatile uint32_t delay = 0x7fff5;

    while(delay--);
}
