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

This example describes how to use GPIO Alternate function and configure the TMR1 peripheral to generate PWM signal.

&par Hardware Description

using LED2(PB5).
using LED3(PB4).
using TMR1_CH1(PC6).
using TMR1_CH1N(PC3).

&par Software Description

PC6 is set to TMR1_CH1, while PC3 is set to TMR1_CH1N.
The waveform can be show be displayed using an oscilloscope by conneceting to PC6 and PC3.

&par Directory contents

  - FMC/FMC_AFR/Source/apm32f0xx_int.c     Interrupt handlers
  - FMC/FMC_AFR/Source/main.c                   Main program

&par IDE environment

  - MDK-ARM V5.36
  - EWARM V9.20.2.43979

&par Hardware and Software environment

  - This example runs on APM32F003 MINI Devices.
