/*!
 * @file        main.c
 *
 * @brief       Main program body
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

#include "main.h"
#include "Board.h"
#include "apm32f00x_gpio.h"
#include "apm32f00x_eint.h"
#include "apm32f00x_fmc.h"

#define FMC_ADDR          (0x7800)
#define FMC_PAGE_SIZE     (1024)

/** LED Init */
void LedInit(void);
/**  Delay  */
void Delay(void);

/*!
 * @brief       Main program
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
int main(void)
{
    uint32_t i;
    uint32_t addr = 0;
    uint32_t *p = NULL;
    BOOL state = TRUE;

    LedInit();

    /**  Unlock flash controler  */
    FMC_Unlock();
    /**  Erase page  */
    FMC_ErasePage(FMC_ADDR);

    /**  Program  */
    addr = FMC_ADDR;
    for(i = 0; i < FMC_PAGE_SIZE / 4; i++)
    {
        FMC_ProgramWord(addr, i);

        addr += 4;
    }
    /**  Lock flash controler  */
    FMC_Lock();

    /**  Read and verify data  */
    p = (uint32_t *)FMC_ADDR;
    for(i = 0; i < FMC_PAGE_SIZE / 4; i++)
    {
        if(p[i] != i)
        {
            state = FALSE;
            break;
        }
    }

    while(1)
    {
        if(state == TRUE)
        {
            Board_LedOff(BOARD_LED3);
        }
        else
        {
            Delay();
            Board_LedToggle(BOARD_LED2);
            Board_LedToggle(BOARD_LED3);
        }
    }
}

/*!
 * @brief       Led Init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void LedInit(void)
{
    Board_LedInit(BOARD_LED2);
    Board_LedInit(BOARD_LED3);

    Board_LedOn(BOARD_LED2);
    Board_LedOn(BOARD_LED3);
}

/*!
 * @brief       delay
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void Delay(void)
{
    volatile uint32_t delay = 0xfffff;

    while(delay--);
}
