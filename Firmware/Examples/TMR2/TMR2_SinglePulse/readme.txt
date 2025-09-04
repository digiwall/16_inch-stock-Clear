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

This example describes how to use the TMR2 to generate a Single pulse Mode.

&par Hardware Description

using LED2(PB5).
using LED3(PB4).
using TMR2_CH3(PD2).
using KEY1(PD6).

&par Software Description

Each time KEY1 pressed, a pulse would be generated.
The pulse can be detected using an oscilloscope by conneceting to PD2.

&par Directory contents

  - TMR2/TMR2_SinglePulse/Source/apm32f0xx_int.c     Interrupt handlers
  - TMR2/TMR2_SinglePulse/Source/main.c                   Main program

&par IDE environment

  - MDK-ARM V5.36
  - EWARM V9.20.2.43979

&par Hardware and Software environment

  - This example runs on APM32F003 MINI Devices.
