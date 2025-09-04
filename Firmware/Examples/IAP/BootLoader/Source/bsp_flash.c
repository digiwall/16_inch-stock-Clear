/*!
 * @file        bsp_flash.c
 *
 * @brief       Include IAP flash operation
 *
 * @version     V1.0.0
 *
 * @date        2022-04-24
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

#include "bsp_flash.h"

/*!
 * @brief       Unlocks Flash for write access
 *
 * @param       None
 *
 * @retval      None
 *
 */
void FLASH_IAP_Init(void)
{
    /** Unlock the flash memory */
    FMC_Unlock();

    /** Clear all FMC flags */
    FMC_ClearStatusFlag(FMC_FLAG_BUSY | FMC_FLAG_PE | FMC_FLAG_WPE | FMC_FLAG_OC);
}

/*!
 * @brief       Erase of all user flash area
 *
 * @param       Application : APP1 or APP2
 *
 * @retval      SUCCESS: user flash area successfully erased
 *              ERROR: error occurred
 */
uint32_t FLASH_IAP_Erase(APP_TypeDef Application)
{
    uint32_t address, addressEnd;

    if (Application == APP1)
    {
        address = USER_APP1_ADDRESS;
    }
    else
    {
        address = USER_APP2_ADDRESS;
    }

    if (Application == APP1)
    {
        addressEnd = USER_APP1_END_ADDRESS;
    }
    else
    {
        addressEnd = USER_APP2_END_ADDRESS;
    }

    while (address <= (uint32_t) addressEnd)
    {
        if (FMC_ErasePage(address) != FMC_STATE_COMPLETE)
        {
            /** Error occurred while page erase */
            return (ERROR);
        }

        address =address + FLASH_PAGE_SIZE;

    }

    return (SUCCESS);
}

/*!
 * @brief       Write the appoint data buffer in flash
 *
 * @param       address: start address for writing data buffer
 *
 * @param       data: pointer on data buffer
 *
 * @param       length: length of data buffer (unit is 32-bit word)
 *
 * @param       Application : APP1 or APP2
 *
 * @retval      SUCCESS: data successfully written to Flash memory
 *              ERROR: Error occurred while writing data in Flash memory
 *
 * @note        buffer data are 32-bit aligned
 */
uint32_t FLASH_IAP_Write(__IO uint32_t* address, uint32_t* data, uint16_t length, APP_TypeDef Application)
{
    uint32_t i = 0;
    uint32_t addressEnd;

    if (Application == APP1)
    {
        addressEnd = USER_APP1_END_ADDRESS - 4;
    }
    else
    {
        addressEnd = USER_APP2_END_ADDRESS - 4;
    }
    
    for (i = 0; i < length; i++)
    {
        if (*address > addressEnd)
        {
            return (ERROR);
        }

        /** The operation will be done by word and Check the written value*/
        if ((FMC_ProgramWord(*address, *(uint32_t*)(data + i)) != FMC_STATE_COMPLETE) \
            || (*(uint32_t*)*address != *(uint32_t*)(data + i)))
        {
            return (ERROR);
        }

        *address += 4;
    }

    return (SUCCESS);
}
