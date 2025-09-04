
#include "adc.h"


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

    for (i = 0; i < 10; i++)
    {
        ADC_StartConversion();

        while (ADC_ReadStatusFlag(ADC_FLAG_CC) == RESET);

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
 * @param       channel : Specifies the channel
 *
 * @retval      None
 *
 * @note
 */
void ADC_GPIO_Init(ADC_CHANNEL_T channel)
{
    GPIO_Config_T gpioConfig;

    /** ADC GPIO configuration */
    gpioConfig.intEn = GPIO_EINT_DISABLE;
    gpioConfig.mode = GPIO_MODE_IN_FLOATING;

    
	  if (channel == ADC_CHANNEL_3)
    {
        gpioConfig.pin = GPIO_PIN_2;
        GPIO_Config(GPIOD, &gpioConfig);
    }
	  else if (channel == ADC_CHANNEL_4)
    {
        gpioConfig.pin = GPIO_PIN_3;
        GPIO_Config(GPIOD, &gpioConfig);
    }
    else if (channel == ADC_CHANNEL_5)
    {
        gpioConfig.pin = GPIO_PIN_5;
        GPIO_Config(GPIOD, &gpioConfig);
    }
}

/*!
 * @brief       ADC Init
 *
 * @param       channel : Specifies the channel
 *
 * @retval      None
 *
 * @note
 */
uint16_t ADC_read(ADC_CHANNEL_T channel)
{
    ADC_Config_T adcConfig;

    /** ADC GPIO configuration */
    //ADC_GPIO_Init(channel);

    ADC_SetMode(ADC_MODE_SINGLE_END);

    /** ADC configuration */
    ADC_ConfigStructInit(&adcConfig);
    adcConfig.channel = channel;
    adcConfig.convMode = ADC_CONV_MODE_CONTINUOUS;
    adcConfig.interrupt = ADC_INT_NONE;
    ADC_Config(&adcConfig);

    /** ADC Calibration */
    ADCCalibration();

    ADC_Enable();

    ADC_StartConversion();
	
	 while (ADC_ReadStatusFlag(ADC_FLAG_CC) == RESET){};

   ADC_ClearStatusFlag(ADC_FLAG_CC);

   return ADC_ReadData();  	
	  
}

/*!
 * @brief       ADCMeasure
 *
 * @param       channel : Specifies the channel
 *
 * @retval      None
 *
 * @note
 */
