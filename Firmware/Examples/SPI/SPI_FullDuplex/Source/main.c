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
#include "apm32f00x_misc.h"
#include "apm32f00x_eint.h"
#include "apm32f00x_spi.h"
#include "apm32f00x_usart.h"
#include "apm32f00x_rcm.h"

#define DATA_BUF_SIZE       (32)

uint8_t rxDataBufSPI[DATA_BUF_SIZE] = {0};
uint8_t rxDataBufUSART[DATA_BUF_SIZE] = {0};

/** LED Init */
void LedInit(void);
/** SPI Init */
void SPIInit(void);
/** USART Init */
void USARTInit(void);
/** delay */
void Delay(uint32_t count);

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
    uint8_t i;
    BOOL state = TRUE;
    
    LedInit();
    SPIInit();
    USARTInit();
    
    Delay(0x5000);
    
    for(i = 0; i < DATA_BUF_SIZE; i++)
    {
        /** Wait transmit buffer is empty */
        while(SPI_ReadStatusFlag(SPI_FLAG_TXBE) == RESET);
        
        /** Write data in SPI transmit data register */
        SPI_TxData(i);
        /** Write data in USART1 transmit data register */
        USART_TxData8(USART1, ~i);
        
        /** Wait data is received by UART1 */
        while(USART_ReadStatusFlag(USART1, USART_FLAG_RXBNE) == RESET);
        rxDataBufUSART[i] = USART_RxData8(USART1);
        
        /** Wait data is received by SPI */
        while(SPI_ReadStatusFlag(SPI_FLAG_RXBNE) == RESET);
        rxDataBufSPI[i] = SPI_RxData();
    }
    
    /** Verify data */
    for(i = 0; i < DATA_BUF_SIZE; i++)
    {
        if(rxDataBufUSART[i] != i)
        {
            state = FALSE;
            break;
        }
        
        if(rxDataBufSPI[i] != (uint8_t)(~i))
        {
            state = FALSE;
            break;
        }
    }
    
    while(1)
    {
        /** Data is ok then turn off LED2 */
        if(state == TRUE)
        {
            Board_LedOff(BOARD_LED2);
        }
        else
        {
            Board_LedToggle(BOARD_LED2);
            Board_LedToggle(BOARD_LED3);
            Delay(0x7ffff);
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
 * @brief       SPI Init   
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void SPIInit(void)
{
    SPI_Config_T spiConfig;
    
    /**  Baudrate divider is 32  */
    spiConfig.baudrateDiv = SPI_BAUDRATE_DIV_32;
    /**  2 line full duplex  */
    spiConfig.direction = SPI_DIRECTION_2LINES_FULLDUPLEX;
    /**  LSB first  */
    spiConfig.firstBit = SPI_FIRST_BIT_LSB;
    /**  No interrupt  */
    spiConfig.interrupt = SPI_INT_NONE;
    /**  Slave mode  */
    spiConfig.mode = SPI_MODE_SLAVE;
    /**  Polarity is high  */
    spiConfig.polarity = SPI_CLKPOL_HIGH;
    /**  Software select slave enable  */
    spiConfig.slaveSelect = SPI_SSC_ENABLE;
    /**  Phase is 2 edge  */
    spiConfig.phase = SPI_CLKPHA_2EDGE;
    /**  SPI config  */
    SPI_Config(&spiConfig);
    
    SPI_Enable();
}

/*!
 * @brief       USART Init   
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void USARTInit(void)
{
    USART_Config_T usartConfig;
    USART_SyncClockConfig_T usartSyncClockConfig;
    
    /**  Enable USART Clock  */
    usartSyncClockConfig.enable = USART_CLKEN_ENABLE;
    /**  Enable last bit clock  */
    usartSyncClockConfig.lastBitClock = USART_LBCP_ENABLE;
    /**  Clock phase is 2 edge */
    usartSyncClockConfig.phase = USART_CLKPHA_2EDGE;
    /**  Clock polarity is high */
    usartSyncClockConfig.polarity = USART_CLKPOL_HIGH;
    /**  Synchronous communication clock configuration */
    USART_ConfigSyncClock(USART1, &usartSyncClockConfig);
    
    /**  BaudRate is 9600 */
    usartConfig.baudRate = 9600;
    /**  No interrupt */
    usartConfig.interrupt = USART_INT_NONE;
    /**  Enable RX and TX */
    usartConfig.mode = USART_MODE_TX_RX;
    /**  Parity disable */
    usartConfig.parity = USART_PARITY_NONE;
    /**  One stop bit */
    usartConfig.stopBits = USART_STOP_BIT_1;
    /**  Word length is 8bit */
    usartConfig.wordLength = USART_WORD_LEN_8B;
    /**  USART1 configuration */
    USART_Config(USART1, &usartConfig);
    
    /**  Enable USART1 */
    USART_Enable(USART1);
}

/*!
 * @brief       Delay   
 *
 * @param       count:  delay count
 *
 * @retval      None
 *
 * @note
 */
void Delay(uint32_t count)
{
    volatile uint32_t delay = count;
    
    while(delay--);
}
