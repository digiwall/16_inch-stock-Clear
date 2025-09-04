
#include "stdio.h"
#include "apm32f00x_gpio.h"
#include "apm32f00x.h"
#include "apm32f00x_fmc.h"

///////////////flash memory read / write///////////
#define FMC_ADDR             (0x7800)           // Default adress 1 Flash page base address

//////////custom configuration
#define FMC_batt_status_memory             (0)           // user data 1 Flash page base address battery status
#define FMC_Power_onoff_memory             (1)           // user data 1 Flash page base address for power on off memory
#define FMC_batt_charge_cut_momory         (2)           // user data 2 Flash page base address for battery overcharge cuttoff
#define FMC_WatchDog_reset_detection       (3)           // user data 3 Flash page base address for battery overcharge cuttoff

#define FMC_PAGE_SIZE        (256)           // Page size in bytes
#define WORD_SIZE            (4)             // 1 word = 4 bytes
#define MAX_FLASH_WORDS      (FMC_PAGE_SIZE / WORD_SIZE)  // 64 uint32_t values per page

#define MAX_FLASH_VALUES   5         // how many values you want to store
#define EMPTY_FLASH_VALUE  0xFFFFFFFF

///////////////////////////////////////////////////



void flash_init(void);  

void wirte_flash(uint32_t address, uint32_t value);

uint32_t read_flash(uint32_t address);


// page wise configuration

void flash_init1(void);  

void wirte_flash1(uint32_t page_address, int32_t address, uint32_t value);

uint32_t read_flash1(uint32_t page_address, int32_t address);

//////////////Curently working fucntion////////////////////////
void wirte_flash2(uint32_t address, uint32_t value); /// with buffer
uint32_t read_flash2(uint32_t address); // with buffer