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

#define DATA_BUF_SIZE       (32)
/** Enable transmit and disable receive*/
#define WRITE_ONLY(x)        x->CTRL2_B.RXEN = RESET; x->CTRL2_B.TXEN = SET
/** Enable receive and disable transmit*/
#define READ_ONLY(x)         x->CTRL2_B.TXEN = RESET; x->CTRL2_B.RXEN = SET

uint8_t rxDataBufUSART1[DATA_BUF_SIZE] = {0};
uint8_t txDataBufUSART2[DATA_BUF_SIZE] = {0};

/** LED Init */
void LedInit(void);
/** USART Init */
void USART_Init(void);
/** USART Send Byte*/
void USART_SendByte(USART_T *usart,uint8_t data);
/* USART Receive Byte**/
uint8_t USART_ReceiveByte(USART_T *usart);
/** Delay */
void Delay(uint32_t count);
/** Data buffer init */
void DataBufInit(void);
/** Compares two buffers */
BOOL BufferCompare(uint8_t *buf1, uint8_t *buf2, uint8_t size);

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

    DataBufInit();
    LedInit();
    USART_Init();
    
    for(i = 0; i < DATA_BUF_SIZE; i++)
    {
        /** Wait until end of transmission */
        //USART_SendByte(USART2, txDataBufUSART2[i]);

       // rxDataBufUSART1[i] = USART_ReceiveByte(USART1);
    }

    /** Verify data */
    //state = BufferCompare(rxDataBufUSART1, txDataBufUSART2, DATA_BUF_SIZE);
		GPIO_Config_T gpioConfig;

    /** LED2 GPIO configuration */
    gpioConfig.mode = GPIO_MODE_OUT_PP;
    gpioConfig.pin = BOARD_LED2_GPIO_PIN;
    gpioConfig.speed = GPIO_SPEED_10MHz;
    GPIO_Config(BOARD_LED2_GPIO_PORT, &gpioConfig);

    while(1)
    {
        /** Data is ok then turn off LED2 */
        if(state == TRUE)
        {
            //Board_LedOff(BOARD_LED2);
        }
        else
        {
           // Board_LedToggle(BOARD_LED2);
            //Board_LedToggle(BOARD_LED3);
            
        }
				Delay(0x7ffff);
				GPIO_Toggle(BOARD_LED2_GPIO_PORT, BOARD_LED2_GPIO_PIN);
				//Board_LedToggle(BOARD_LED2);
				USART_SendByte(USART1, 2);
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
 * @brief       USART GPIO Init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void USART_GPIOInit(void)
{
    GPIO_Config_T gpioConfig;

    gpioConfig.mode = GPIO_MODE_IN_FLOATING;
    gpioConfig.speed = GPIO_SPEED_10MHz;
    gpioConfig.pin = USART1_TX_PIN;

    GPIO_Config(USART1_TX_GPIO_PORT, &gpioConfig);
    
    gpioConfig.pin = USART2_TX_PIN;

    GPIO_Config(USART2_TX_GPIO_PORT, &gpioConfig);
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
void USART_Init(void)
{
    USART_Config_T usartConfig;

    RCM_EnableAPBPeriphClock(RCM_PERIPH_USART1);
    RCM_EnableAPBPeriphClock(RCM_PERIPH_USART2);
    
    /**  GPIO Init */
    USART_GPIOInit();
    
    /**  BaudRate */
    usartConfig.baudRate = USART_BAUDRATE;
    /**  No interrupt */
    usartConfig.interrupt = USART_INT_NONE;
    /**  Enable receiver */
    usartConfig.mode = USART_MODE_TX;
    /**  Parity disable */
    usartConfig.parity = USART_PARITY_NONE;
    /**  One stop bit */
    usartConfig.stopBits = USART_STOP_BIT_1;
    /**  Word length is 8bit */
    usartConfig.wordLength = USART_WORD_LEN_8B;
    /**  USART1 configuration */
    USART_Config(USART1, &usartConfig);

    /**  Enable transmitter */
    usartConfig.mode = USART_MODE_RX;
    /**  USART2 configuration */
    USART_Config(USART2, &usartConfig);
    
    /**  Clear other functiom */
    USART1->CTRL3_B.CLKEN = BIT_RESET;
    USART1->CTRL3_B.LINEN = BIT_RESET;
    USART1->CTRL5_B.IRDAEN = BIT_RESET;
    USART1->CTRL5_B.SMEN = BIT_RESET;
    
    USART2->CTRL3_B.CLKEN = BIT_RESET;
    USART2->CTRL3_B.LINEN = BIT_RESET;
    USART2->CTRL5_B.IRDAEN = BIT_RESET;
    USART2->CTRL5_B.SMEN = BIT_RESET;

    /**  Enable Half Duplex */
    USART_EnableHalfDuplex(USART1);
    USART_EnableHalfDuplex(USART2);

    /**  Enable USART1 */
    USART_Enable(USART1);
    /**  Enable USART2 */
    //USART_Enable(USART2);
}

/*!
 * @brief       USART Send byte
 *
 * @param       usart
 *
 * @param       data
 *
 * @retval      None
 *
 * @note
 */
void USART_SendByte(USART_T *usart,uint8_t data)
{
    /** Enable transmit and disable receive*/
    WRITE_ONLY(usart);
    
    USART_TxData8(usart, data);
    while(USART_ReadStatusFlag(usart, USART_FLAG_TXBE) == RESET)
    {
        
    }
}

/*!
 * @brief       USART Receive byte
 *
 * @param       usart
 *
 * @retval      receive data
 *
 * @note
 */
uint8_t USART_ReceiveByte(USART_T *usart)
{
    uint8_t revData;
    
    while(USART_ReadStatusFlag(usart, USART_FLAG_TXC) == RESET)
    {
        
    }
    /** Enable receive and disable transmit*/
    READ_ONLY(usart);
    
    while(USART_ReadStatusFlag(usart, USART_FLAG_RXBNE) == RESET)
    {
        
    }
    revData = USART_RxData8(usart);
    
    return revData;
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

    while(delay--)
    {
        
    }
}

/*!
 * @brief       Data buffer init
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void DataBufInit(void)
{
    uint8_t i;

    for(i = 0; i < DATA_BUF_SIZE; i++)
    {
        txDataBufUSART2[i] = 0xa0 + i;
        rxDataBufUSART1[i] = 0;
    }
}

/*!
 * @brief       Compares two buffers
 *
 * @param       buf1:    First buffer to be compared
 *
 * @param       buf1:    Second buffer to be compared
 *
 * @param       size:    Buffer size
 *
 * @retval      Return TRUE if buf1 = buf2. If not then return FALSE
 *
 * @note
 */
BOOL BufferCompare(uint8_t *buf1, uint8_t *buf2, uint8_t size)
{
    uint8_t i;

    for(i = 0; i < size; i++)
    {
        if(buf1[i] != buf2[i])
        {
            return FALSE;
        }
    }

    return TRUE;
}
