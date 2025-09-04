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
#include <stdio.h>

/** ADC Channel definition */
#define ADC_CH0_CHANNEL             ADC_CHANNEL_0
#define ADC_CH1_CHANNEL             ADC_CHANNEL_1
#define ADC_CH2_CHANNEL             ADC_CHANNEL_2

/** ADC GPIO port and pin */
#define ADC_CH0_GPIO_PIN            GPIO_PIN_5
#define ADC_CH1_GPIO_PIN            GPIO_PIN_6
#define ADC_CH2_GPIO_PIN            GPIO_PIN_4
#define ADC_CH_GPIO_PORTC           GPIOC

/** USART1 definition */
#define DEBUG_USART                 USART1
#define DEBUG_USART_CLK             RCM_PERIPH_USART1
#define DEBUG_USART_BAUDRATE        115200

/** LED Init */
void Led_Init(void);
/** ADC Continuous Scan Init */
void ADC_ContinuousScanInit(void);
/** ADC GPIO Config */
void ADC_GPIOConfig(void);
/** ADC Poll */
void ADC_PollForConversion(void);
void Delay(void);
void USART_Init(uint32_t baudRate);
void USART_SendString(USART_T *usart, uint8_t *str);

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
    Led_Init();
    
    /** ADC Continuous Scan Init*/
    ADC_ContinuousScanInit();
    
    USART_Init(DEBUG_USART_BAUDRATE);
    
    while(1)
    {
        ADC_PollForConversion();
        Board_LedToggle(BOARD_LED2);
        Delay();
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
void Led_Init(void)
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
void ADC_Calibration(void)
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
 * @brief       ADC GPIO Config
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void ADC_GPIOConfig(void)
{
    GPIO_Config_T gpioConfig;
    
    gpioConfig.intEn = GPIO_EINT_DISABLE;
    gpioConfig.mode = GPIO_MODE_IN_FLOATING;
    gpioConfig.pin = ADC_CH0_GPIO_PIN | ADC_CH1_GPIO_PIN | ADC_CH2_GPIO_PIN;
    GPIO_Config(ADC_CH_GPIO_PORTC, &gpioConfig);
}

/*!
 * @brief       ADC Continuous Scan Init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void ADC_ContinuousScanInit(void)
{
    ADC_Config_T adcConfig;

    /** ADC GPIO configuration */
    ADC_GPIOConfig();

    ADC_SetMode(ADC_MODE_SINGLE_END);

    /** ADC configuration */
    ADC_ConfigStructInit(&adcConfig);
    adcConfig.div = ADC_DIV_4;
    adcConfig.channel = ADC_CH2_CHANNEL;
    adcConfig.convMode = ADC_CONV_MODE_CONTINUOUS;
    adcConfig.scanMode = ADC_SCAN_MODE_ENABLE;
    adcConfig.interrupt = ADC_INT_CC;
    ADC_Config(&adcConfig);

    /** ADC Calibration */
    ADC_Calibration();

    ADC_Enable();
    ADC_StartConversion();
}

/*!
 * @brief       ADC poll for conversion
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void ADC_PollForConversion(void)
{
    uint16_t voltage = 0;
    uint16_t adcArray[10] = {0};
    uint8_t str[100];
    uint8_t bufferIndex;

    if(ADC_ReadStatusFlag(ADC_FLAG_CC) == SET)
    {
        ADC_ClearStatusFlag(ADC_FLAG_CC);

        for(bufferIndex = 0; bufferIndex < 10; bufferIndex++)
        {
            adcArray[bufferIndex] = ADC_ReadBufferData((ADC_BUFFER_IDX_T)(bufferIndex));
        }
        
        for(bufferIndex = 0; bufferIndex < 3; bufferIndex++)
        {
            /**  voltage(mV) =  adcData * (3300mV / 4095) */
            voltage = (adcArray[bufferIndex] * 3300) / 4095;
            sprintf((char*)str, "ADC Channel %d Value:%d\r\n", bufferIndex,voltage);
            USART_SendString(DEBUG_USART,str);
        }
        USART_SendString(DEBUG_USART,(uint8_t*)"\r\n");
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
    volatile uint32_t delay = 0xffff5;

    while(delay--)
    {}
}

/*!
 * @brief       USART Init
 *
 * @param       baudRate
 *
 * @retval      None
 *
 * @note
 */
void USART_Init(uint32_t baudRate)
{
    USART_Config_T usartConfig;
    
    RCM_EnableAPBPeriphClock(DEBUG_USART_CLK);
    
    /**  BaudRate */
    usartConfig.baudRate = baudRate;
    /**  No interrupt */
    usartConfig.interrupt = USART_INT_NONE;
    /**  Enable receiver */
    usartConfig.mode = USART_MODE_TX_RX;
    /**  Parity disable */
    usartConfig.parity = USART_PARITY_NONE;
    /**  One stop bit */
    usartConfig.stopBits = USART_STOP_BIT_1;
    /**  Word length is 8bit */
    usartConfig.wordLength = USART_WORD_LEN_8B;
    /**  USART1 configuration */
    USART_Config(DEBUG_USART, &usartConfig);
    
    /**  Enable USART */
    USART_Enable(DEBUG_USART);
}

/*!
 * @brief       USART Send byte
 *
 * @param       usart
 *
 * @param       data
 *
 * @retval      None
 *
 * @note
 */
void USART_SendByte(USART_T *usart,uint8_t data)
{
    USART_TxData8(usart, data);
    while(USART_ReadStatusFlag(usart, USART_FLAG_TXBE) == RESET)
    {
        
    }
}

/*!
 * @brief       USART Receive byte
 *
 * @param       usart
 *
 * @retval      receive data
 *
 * @note
 */
uint8_t USART_ReceiveByte(USART_T *usart)
{
    uint8_t revData;
    
    while(USART_ReadStatusFlag(usart, USART_FLAG_RXBNE) == RESET)
    {
        
    }
    revData = USART_RxData8(usart);
    
    return revData;
}

/*!
 * @brief       USART Send string
 *
 * @param       usart
 *
 * @param       str
 *
 * @retval      None
 *
 * @note
 */
void USART_SendString(USART_T *usart, uint8_t *str)
{
    while(*str != '\0')
    {
        USART_SendByte(usart, *str);
        str++;
    }
}
