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

This example describes how to use BUZZER peripheral.

&par Hardware Description

J2 and J3 should be shorted.
using Buzzer(PD4).
using KEY1(PD6).

&par Software Description

Each time the key1 button is pressed the BUZZER frequency changes and a corresponding
led will be switched on.
When Buzzer output clock frequency is 1khz,LED2 is on and LED3 is off.
When Buzzer output clock frequency is 2khz,LED2 is off and LED3 is on.
When Buzzer output clock frequency is 4khz,LED2 is on and LED3 is on.

The waveform can be show be displayed using an oscilloscope by conneceting to PD4.

&par Directory contents

  - BUZZER/BUZZER/Source/apm32f0xx_int.c     Interrupt handlers
  - BUZZER/BUZZER/Source/main.c                   Main program

&par IDE environment

  - MDK-ARM V5.36
  - EWARM V9.20.2.43979

&par Hardware and Software environment

  - This example runs on APM32F003 MINI Devices.
