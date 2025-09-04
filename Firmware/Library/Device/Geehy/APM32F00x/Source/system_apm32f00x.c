/*!
 * @file        system_apm32f00x.c
 *
 * @brief       CMSIS Cortex-M0+ Device Peripheral Access Layer System Source File
 *
 * @version     V1.0.1
 *
 * @date        2021-04-11
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

#include "apm32f00x.h"
#include "apm32f00x_rcm.h"
#include "apm32f00x_iwdt.h"


//#define MASTER_CLOCK_HXT    HXT_VALUE
//#define MASTER_CLOCK_LIRC   (128000)
//#define MASTER_CLOCK_6MHz   (6000000)
//#define MASTER_CLOCK_12MHz  (12000000)
#define MASTER_CLOCK_24MHz  (24000000)
//#define MASTER_CLOCK_48MHz  (48000000)

static void ConfigMasterClock(void);
#ifdef MASTER_CLOCK_HXT
static void MasterClockHXT(void);
#elif defined MASTER_CLOCK_6MHz
static void MasterClock6M(void);
#elif defined MASTER_CLOCK_12MHz
static void MasterClock12M(void);
#elif defined MASTER_CLOCK_24MHz
static void MasterClock24M(void);
#elif defined MASTER_CLOCK_48MHz
static void MasterClock48M(void);
#elif defined MASTER_CLOCK_LIRC
static void MasterClockLIRC(void);
#endif

/** #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET     0x00

// Define a timeout limit to avoid infinite loops if hardware is unstable
#define TIMEOUT_LIMIT 1000000  // Adjust this based on your clock speed and testing

/*!
 * @brief       Setup the microcontroller system.Initialize the Embedded Flash Interface and system clock
 *
 * @param       None
 *
 * @retval      None
 *
 * @note        This function should be used only after reset
 */
/*void SystemInit (void) // Orginal
{
    RCM->ICC = 0X01;
    RCM->ECC = 0X00;
    RCM->MCC = 0xE1;
    RCM->CSC = 0x00;
    RCM->CLKDIV = 0x18;
    RCM->APBEN1 = 0XFF;
    RCM->APBEN2 = 0XFF;
    RCM->APBEN3 = 0XFF;
    RCM->CSS = 0x00;
    RCM->COC = 0x00;
    while(RCM->COC_B.COEN == SET)
    {}
    RCM->COC = 0x00;
    RCM->HIRCTRIM = 0x00;

    ConfigMasterClock();

#ifdef VECT_TAB_SRAM
    SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET;
#else
    SCB->VTOR = FMC_BASE | VECT_TAB_OFFSET;
#endif
}*/

void SystemInit(void)
{
	
	
		IWDT->KEYWORD = IWDT_KEY_ENABLE;
	  IWDT->KEYWORD = IWDT_KEY_ACCESS;
	  IWDT->PSC = (uint32_t)IWDT_DIV_128;
    //IWDT_SetReloadCounter(99);  // fro .5s= freqLIRC/(2*IWDT_DIV_256) =2*256 = 512, for  .3s  = (1/.3)*256, . for .4 = (1/.4)*256
	  IWDT->CNTRLD = (uint32_t)99;
    //IWDT_ReloadCounter();
	  IWDT->KEYWORD = IWDT_KEY_REFRESH;
    /*
     * Initialize RCM registers to desired default values
     * These registers configure the clock system (oscillators, dividers, etc.)
     */
    RCM->ICC = 0x01;      // Enable Internal Clock Control (e.g., HIRC)
    RCM->ECC = 0x00;      // Disable External Clock Control
    //RCM->MCC = 0xE1;      // Master Clock Control register – sets clock source and output enable
    RCM->CSC = 0x00;      // Clock Switch Control – no switching initially
    RCM->CLKDIV = 0x18;   // Clock Divider register – divide factor for system clock
    RCM->APBEN1 = 0xFF;   // Enable all APB1 peripheral clocks
    RCM->APBEN2 = 0xFF;   // Enable all APB2 peripheral clocks
    RCM->APBEN3 = 0xFF;   // Enable all APB3 peripheral clocks
    RCM->CSS = 0x00;      // Clock Security System – disable initially
    RCM->COC = 0x00;      // Clock Output Control – no output initially
    

    /*
     * Short delay for clocks to stabilize
     * This ensures that the written register values take effect
     */
    for (volatile int i = 0; i < 10000; i++);

    /*
     * Wait for Configurable Clock Output (COC) circuit to clear its busy flag
     * Some microcontrollers require waiting until COC_B.COEN == 0 (disabled)
     * This ensures no conflicts when reconfiguring clocks
     */
		 
    int timeout = 0;                      // Initialize timeout counter
    while (RCM->COC_B.COEN == 1)          // Check if the COC enable bit is still set
    {
         IWDT->KEYWORD = IWDT_KEY_REFRESH; // Reload counter
        if (++timeout > TIMEOUT_LIMIT)    // If timeout occurs
        {
            NVIC_SystemReset();           // Reset the system to recover from error
        }
    }

    // After ensuring COC is disabled, clear the register again for safety
    RCM->COC = 0x00;

    /*
     * Wait for High-speed Internal RC (HIRC) ready flag
     * This confirms that the HIRC oscillator has stabilized and is ready for use
     */
    timeout = 0;                          // Reset timeout counter
    while (!RCM_ReadClockFlag(RCM_CLOCK_FLAG_HIRCRDY))  // Check if HIRC ready flag is SET
    {
      IWDT->KEYWORD = IWDT_KEY_REFRESH; // Reload counter  
			if (++timeout > TIMEOUT_LIMIT)    // If timeout occurs
        {
					  
            NVIC_SystemReset();           // Reset system if HIRC not ready within limit
        }
    }
		
		
    RCM->COC = 0x00; //// 5. Clear clock output control register
		
		RCM->HIRCTRIM = 0x00; // HIRC trimming – default/no trimming

    /*
     * Optional: If using Low-speed Internal RC (LIRC), check its ready flag too
     * Uncomment below if LIRC is part of your system clock configuration
     */
    /*
    timeout = 0;
    while (!RCM_ReadClockFlag(RCM_CLOCK_FLAG_LIRCRDY))
    {
        if (++timeout > TIMEOUT_LIMIT)
        {
            NVIC_SystemReset();
        }
    }
    */
		///////////////Configures the master clock and waits for it to stabilize.////////////
		    // Wait until HIRC ready
    timeout = 0;
    while (!RCM_ReadClockFlag(RCM_CLOCK_FLAG_HIRCRDY))
    {
        timeout++;
        IWDT->KEYWORD = IWDT_KEY_REFRESH; // Reload counter
        if (timeout > 1000000)
        {
            NVIC_SystemReset();
        }
    }

    // Set HIRC as master clock
    RCM->MCC = 0xE1;

    // Small delay for stabilization
    for (volatile int i = 0; i < 1000; i++);
		
		    // 8. Check for clock security faults
    if (RCM_ReadClockFlag(RCM_CLOCK_FLAG_CSSFD))
    {
        // Take protective action if clock security system detected a fault
        NVIC_SystemReset();
    }
		
		

    /*
     * Now that the oscillators are ready and stable, configure the master clock
     * This typically sets the final system clock source (e.g., HIRC, HXT, LIRC)
     */
    ConfigMasterClock();

    /*
     * Set the interrupt vector table offset
     * This tells the MCU where to find the interrupt vector table in memory
     * Can be in SRAM or Flash (FMC) depending on your project configuration
     */
#ifdef VECT_TAB_SRAM
    SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; // Use SRAM as vector table base
#else
    SCB->VTOR = FMC_BASE | VECT_TAB_OFFSET;  // Use Flash (FMC) as vector table base
#endif
}




/*!
 * @brief       Configures the master clock and cpu clock
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
static void ConfigMasterClock(void)
{
#ifdef MASTER_CLOCK_HXT
     MasterClockHXT();
#elif defined MASTER_CLOCK_6MHz
    MasterClock6M();
#elif defined MASTER_CLOCK_12MHz
    MasterClock12M();
#elif defined MASTER_CLOCK_24MHz
    MasterClock24M();
#elif defined MASTER_CLOCK_48MHz
    MasterClock48M();
#elif defined MASTER_CLOCK_LIRC
    MasterClockLIRC();
#endif
}

#ifdef MASTER_CLOCK_HXT

/*!
 * @brief       Selects HXT as Master clock source and configure CPU clock frequency
 *
 * @param        None
 *
 * @retval      None
 *
 * @note
 */
static void MasterClockHXT(void)
{
    uint16_t counter = 0xFFFF;

    /** Enable Prefetch Buffer */
    FMC->CTRL1_B.PBEN = BIT_SET;
    /** Flash 0 wait state */
    FMC->CTRL1_B.LATENCY = 0;

    RCM->CLKDIV_B.CPUDIV = 0;
    RCM->CSC_B.CSIE = 0;
    RCM->MCC = 0xB4;

    while((RCM->CSC_B.CSIF == BIT_RESET) && (counter))
    {
        counter--;
    }

    if(RCM->CSC_B.CSIF == BIT_SET)
    {
        RCM->CSC_B.CSEN = BIT_SET;
        RCM->ICC_B.HIRCEN = BIT_RESET;
        RCM->CSS_B.CSSEN = BIT_SET;
    }
    else
    {
        RCM->CSC_B.CSBF = BIT_RESET;
    }
}


#elif defined MASTER_CLOCK_6MHz

/*!
 * @brief       Selects HIRC/8 as Master clock source and configure CPU clock
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
static void MasterClock6M(void)
{
    /** Enable Prefetch Buffer */
    FMC->CTRL1_B.PBEN = BIT_SET;
    /** Flash 0 wait state */
    FMC->CTRL1_B.LATENCY = 0;

    RCM->CLKDIV_B.CPUDIV = 0;
    RCM->CLKDIV_B.HDS = 1;
    RCM->CLKDIV_B.HIRCDIV = 3;
}

#elif defined MASTER_CLOCK_12MHz

/*!
 * @brief       Selects HIRC/4 as Master clock source and configure CPU clock
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
static void MasterClock12M(void)
{
    /** Enable Prefetch Buffer */
    FMC->CTRL1_B.PBEN = BIT_SET;
    /** Flash 0 wait state */
    FMC->CTRL1_B.LATENCY = 0;

    RCM->CLKDIV_B.CPUDIV = 0;
    RCM->CLKDIV_B.HDS = 1;
    RCM->CLKDIV_B.HIRCDIV = 2;
}

#elif defined MASTER_CLOCK_24MHz

/*!
 * @brief       Selects HIRC/2 as Master clock source and configure CPU clock
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
static void MasterClock24M(void)
{
    /** Enable Prefetch Buffer */
    FMC->CTRL1_B.PBEN = BIT_SET;
    /** Flash 0 wait state */
    FMC->CTRL1_B.LATENCY = 0;
    RCM->CLKDIV_B.HDS = 1;
    RCM->CLKDIV_B.CPUDIV = 0;
    RCM->CLKDIV_B.HIRCDIV = 1;
}

#elif defined MASTER_CLOCK_48MHz

/*!
 * @brief       Selects HIRC as Master clock source and configure CPU clock
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
static void MasterClock48M(void)
{
    /** Enable Prefetch Buffer */
    FMC->CTRL1_B.PBEN = BIT_SET;
    /** Flash 1 wait state */
    FMC->CTRL1_B.LATENCY = 1;

    RCM->CLKDIV_B.CPUDIV = 0;
    RCM->CLKDIV_B.HDS = 1;
    RCM->CLKDIV_B.HIRCDIV = 0;
}

#elif defined MASTER_CLOCK_LIRC

/*!
 * @brief       Selects LIRC as Master clock source and configure CPU clock
 *
 * @param        None
 *
 * @retval      None
 *
 * @note
 */
static void MasterClockLIRC(void)
{
    uint16_t counter = 0xFFFF;

    /** Enable Prefetch Buffer */
    FMC->CTRL1_B.PBEN = BIT_SET;
    /** Flash 0 wait state */
    FMC->CTRL1_B.LATENCY = 0;

    RCM->CLKDIV_B.CPUDIV = 0;
    RCM->CSC_B.CSIE = 0;
    RCM->MCS = 0xD2;

    while((RCM->CSC_B.CSIF == BIT_RESET) && (counter))
    {
        counter--;
    }

    if(RCM->CSC_B.CSIF == BIT_SET)
    {
        RCM->CSC_B.CSEN = BIT_SET;
        RCM->ICC_B.HIRCEN = BIT_RESET;
    }
    else
    {
        RCM->CSC_B.CSBF = BIT_RESET;
    }

}
#endif
