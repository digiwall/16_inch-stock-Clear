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
#include "apm32f00x_iwdt.h"
#include "apm32f00x_rcm.h"
#include "apm32f00x_wupt.h"
#include "apm32f00x_tmr1.h"

/** Key Init */
void KeyInit(void);
/** delay */
void Delay(uint32_t count);
/** Led Init */
void LedInit(void);
/** Process when reset occured */
void ResetProc(void);
/** Key process */
BOOL KeyPressed(void);
/** IWDT init */
void IWDTInit(uint32_t freqHIRC);
/** LIRC frequency Measurment */
uint32_t LIRCMeasurment(void);

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
    uint32_t freqLIRC;

    LedInit();

    ResetProc();

    freqLIRC = LIRCMeasurment();
    IWDTInit(freqLIRC);

    while(1)
    {
        if(KeyPressed())
        {
            while(1);
        }
        IWDT_ReloadCounter();
    }
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

    if(RCM_ReadResetFlag(RCM_RTS_FLAG_IWDTRST) == SET)
    {
        for(i = 0; i < 20; i++)
        {
            Delay(0x7ffff);
            Board_LedToggle(BOARD_LED2);
            Board_LedToggle(BOARD_LED3);
        }

        RCM_ClearResetFlag(RCM_RTS_FLAG_IWDTRST);
    }
}

/*!
 * @brief       IWDT init.Set IWDT timeout = 0.5s
 *
 * @param       freqHIRC:    LIRC frequency in HZ
 *
 * @retval      None
 *
 * @note
 */
void IWDTInit(uint32_t freqLIRC)
{
    IWDT_Enable();

    IWDT_EnableWriteAccess();
    /**
        IWDT timeout = 0.5S = (Counter Reload Value * DIV) / freqLIRC;

        Counter Reload Value = (freqLIRC * 0.5) / DIV
                             = freqLIRC / (2 * DIV)
    */
    IWDT_SetDivider(IWDT_DIV_256);
    IWDT_SetReloadCounter(freqLIRC / 512);

    IWDT_ReloadCounter();
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
