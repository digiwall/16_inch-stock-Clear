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

    This demo is based on the APM32F003 mini board, it shows how to use IWDT peripheral.
    The IWDT time out is set to 0.5s.If KEY1 is pressed the IWDT will generate a reset.

&par Example Description

This example describes how to use IWDT peripheral.

&par Hardware Description

J2 and J3 Should be shorted.
using KEY1(PD6).

&par Software Description

The IWDT time out is set to 0.5s.If KEY1 is pressed the IWDT will generate a reset.

&par Directory contents

  - IWDT/IWDT/Source/apm32f0xx_int.c     Interrupt handlers
  - IWDT/IWDT/Source/main.c                   Main program

&par IDE environment

  - MDK-ARM V5.36
  - EWARM V9.20.2.43979

&par Hardware and Software environment

  - This example runs on APM32F003 MINI Devices.
