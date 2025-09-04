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

This example describes how to use ADC peripheralto convert Channel 4(PD3) or
Channel 5(PD5) input voltage in Continuous conversion mode with interrupt.

&par Hardware Description

J2 and J3 should be shorted.
using ADC_CHANNEL_4(PD3) and ADC_CHANNEL_5(PD5).
using KEY1(PD6).

&par Software Description

After Initialization, when press the Key1, ADC choose the Channel 5(PD5). When reset the Key1 ADC choose the Channel 4(PD3).

&par Directory contents

  - ADC/ADC_Channel_2/Source/apm32f0xx_int.c     Interrupt handlers
  - ADC/ADC_Channel_2/Source/main.c                   Main program

&par IDE environment

  - MDK-ARM V5.36
  - EWARM V9.20.2.43979

&par Hardware and Software environment

  - This example runs on APM32F003 MINI Devices.
