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

This example describes how to use I2C peripheral.

&par Hardware Description

J2 and J3 Should be opend.
This demo need two APM32F003 mini board, they are connected through I2C lines and GND.As follow:
                                    _______VDD_______
                                     |             |
                                    _|_           _|_
                                   |   |         |   |
                   pull up resistor|   |         |   |pull up resistor
                                   |___|         |___|
                                     |             |
               _________             |             |            _________
              |         |            |             |           |         |
              |         |__SDA(PB5)__|_____________|___________|         |
              |         |__SCL(PB4)________________|___________|         |
              |         |_____GND______________________________|         |
              |         |                                      |         |
              |_________|                                      |_________|
                Board 1                                          Board 2

using I2C_SCL(PB4).
using I2C_SDA(PB5).
using KEY1(PD6).

&par Software Description

It is a example of how to use the I2C software library to ensure the steps of an
I2C communication between slave Receiver/transmitter and master transmitter/receiver using interrupts.

&par Directory contents

  - I2C/I2C_TwoBoards_Master/Source/apm32f0xx_int.c     Interrupt handlers
  - I2C/I2C_TwoBoards_Master/Source/main.c                   Main program

  - I2C/I2C_TwoBoards_Slave/Source/apm32f0xx_int.c     Interrupt handlers
  - I2C/I2C_TwoBoards_Slave/Source/main.c                   Main program

&par IDE environment

  - MDK-ARM V5.36
  - EWARM V9.20.2.43979

&par Hardware and Software environment

  - This example runs on APM32F003 MINI Devices.
