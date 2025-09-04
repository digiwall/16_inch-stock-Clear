
#include "utility.h"
#include "battery.h"
#include "time.h"


#ifndef __FAN__H_
#define __FAN__H_


#define LED_GREEN  		 GPIO_PIN_3
#define LED_GREEN_ON		 (GPIOD->ODR |= (uint8_t)LED_GREEN );   //GPIOx->ODR |= (uint8_t)PortPins;
#define LED_GREEN_OFF		 (GPIOD->ODR &= (uint8_t)(~LED_GREEN ));//GPIOx->ODR &= (uint8_t)(~PortPins);
#define LED_GREEN_Toggle	 (GPIOD->ODR ^= (uint8_t)LED_GREEN);    //GPIOx->ODR &= (uint8_t)(~PortPins);


void fan_init(void);

void fan_enable(void);

void fan_command(uint16_t pwm);

void fan_shutdown(void);

void Fan_Load(uint16_t pwm_duty);

#endif 