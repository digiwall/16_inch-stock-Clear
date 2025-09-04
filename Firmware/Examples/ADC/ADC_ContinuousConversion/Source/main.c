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
#include "apm32f00x_adc.h"

/** ADC Channel definition */
#define ADC_CHANNEL         ADC_CHANNEL_3

/** ADC GPIO port and pin */
#define ADC_GPIO_PORT       GPIOD
#define ADC_GPIO_PIN        GPIO_PIN_2

/** LED Init */
void LedInit(void);
/** ADC Init */
void ADCInit(void);

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
	Serial_begin(9600);
    LedInit();
    ADCInit();

    while(1)
    {
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

    Board_LedOff(BOARD_LED2);
    Board_LedOff(BOARD_LED3);
}

/*!
 * @brief       ADC Calibration
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void ADCCalibration(void)
{
    uint8_t i;
    int8_t offset = 0;
    uint16_t adcData = 0;

    ADC_SetOffset(0);
    ADC_EnableCompensation();
    ADC_ClearStatusFlag(ADC_FLAG_CC);
    ADC_Enable();
    ADC_StartConversion();

    for(i = 0; i < 10; i++)
    {
        while(ADC_ReadStatusFlag(ADC_FLAG_CC) == RESET);
        ADC_ClearStatusFlag(ADC_FLAG_CC);
    }
    ADC_Disable();

    adcData = ADC_ReadData();
    offset = (int8_t)(0x800 - adcData);
    ADC_SetOffset(offset);

    ADC_DisableCompensation();
    ADC_ClearStatusFlag(ADC_FLAG_CC);
}

/*!
 * @brief       ADC Init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void ADCInit(void)
{
    ADC_Config_T adcConfig;
    GPIO_Config_T gpioConfig;

    /** ADC GPIO configuration */
    gpioConfig.intEn = GPIO_EINT_DISABLE;
    gpioConfig.mode = GPIO_MODE_IN_FLOATING;
    gpioConfig.pin = ADC_GPIO_PIN;
    GPIO_Config(ADC_GPIO_PORT, &gpioConfig);

    ADC_SetMode(ADC_MODE_SINGLE_END);

    /** ADC configuration */
    ADC_ConfigStructInit(&adcConfig);
    adcConfig.channel = ADC_CHANNEL;
    adcConfig.convMode = ADC_CONV_MODE_CONTINUOUS;
    adcConfig.interrupt = ADC_INT_CC;
    ADC_Config(&adcConfig);

    /** ADC Calibration */
    ADCCalibration();

    NVIC_EnableIRQRequest(ADC_IRQn, 0X01);
    ADC_Enable();
    ADC_StartConversion();
}

/*!
 * @brief       ADC interrupt service routine
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void ADCIsr(void)
{
    uint16_t adcData = 0;
    uint16_t voltage = 0;

    if(ADC_ReadIntFlag(ADC_INT_FLAG_CC) == SET)
    {
        ADC_ClearIntFlag(ADC_INT_FLAG_CC);

        adcData = ADC_ReadData();
			
			  SerialPrint_NumString("BatVolt", adcData );
        /**  voltage(mV) =  adcData * (3300mV / 4095) */
        voltage = (adcData * 3300) / 4095;

        /**  voltage greater than 3100mv */
        if(voltage > 3100)
        {
            Board_LedOn(BOARD_LED2);
            Board_LedOff(BOARD_LED3);
        }
        /**  voltage less than 800mv */
        else if(voltage < 800)
        {
            Board_LedOn(BOARD_LED3);
            Board_LedOff(BOARD_LED2);
        }
        /**  800mv <= voltage <= 3100 */
        else
        {
            Board_LedOn(BOARD_LED2);
            Board_LedOn(BOARD_LED3);
        }
    }
}
