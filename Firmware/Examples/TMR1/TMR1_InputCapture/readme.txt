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

This example describes how to configure the TMR1 peripheral to capture the internal clock source.

&par Hardware Description

using LED2(PB5).
using LED3(PB4).
using COC(PC4) -> TMR1_CH1N(PC3).

&par Software Description

PC4£¨COCpin£© should be conneted to PC3£¬then LED2 and LED3 will alternate twinkle.

&par Directory contents

  - TMR1/TMR1_InputCapture/Source/apm32f0xx_int.c     Interrupt handlers
  - TMR1/TMR1_InputCapture/Source/main.c                   Main program

&par IDE environment

  - MDK-ARM V5.36
  - EWARM V9.20.2.43979

&par Hardware and Software environment

  - This example runs on APM32F003 MINI Devices.
