#include "apm32f00x_adc.h"
#include "apm32f00x_gpio.h"


/** ADC Init */

void ADC_GPIO_Init(ADC_CHANNEL_T channel);

uint16_t ADC_read(ADC_CHANNEL_T channel);