/*!
 * @file        readme.txt
 *
 * @brief       This file is routine instruction
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

&par Example Description

This example describes how to use ADC peripheral
to scan multi channel input voltage in continuous scan mode with polling.

&par Hardware Description
J2 and J3 should be shorted.
using ADC_CHANNEL_0(PC5).
using ADC_CHANNEL_1(PC6).
using ADC_CHANNEL_2(PC4).
using USART1 TX(PD5).
using LED2(PB5).
using LED3(PB4).

&par Software Description

After Initialization, PC5/PC6/PC4 can be connected to the VDD or GND. The value of ADC channel will always be printed on USART1(PD5).

&par Directory contents

  - ADC/ADC_ContinuousConversion/Source/apm32f0xx_int.c     Interrupt handlers
  - ADC/ADC_ContinuousConversion/Source/main.c                   Main program

&par IDE environment

  - MDK-ARM V5.36
  - EWARM V9.20.2.43979

&par Hardware and Software environment

  - This example runs on APM32F003 MINI Devices.
