					 
#include "flash.h"
#include "main.h"
uint32_t *p = NULL;	
        

void flash_init(void){

								/**  Erase page  */
								FMC_ErasePage(FMC_ADDR);
}


    

void wirte_flash(uint32_t address, uint32_t value){
	               uint32_t Mem_Addr = 0;
								/**  Unlock flash controler  */
								    FMC_Unlock();	                  
                 /**  Erase page  */
								FMC_ErasePage(FMC_ADDR);   
				         /**  Program  */
	              Mem_Addr = FMC_ADDR;
								
								SerialPrint_NumString("write value", value);
								FMC_ProgramWord((Mem_Addr + 4*address), value);
	
								/**  Lock flash controler  */
							  FMC_Lock();
          }



uint32_t read_flash(uint32_t address){
              
              p = (uint32_t*)FMC_ADDR;         

							p = p + address;
							//SerialPrint_NumString("Read value", *p );
							//return p[4*address];
							return *p;			 	
	
          // SerialPrint_NumString("read data", p[0] )	;				
       }

			 
			 // pagge wise read write
			 
void flash_init1(void){

								/**  Erase page  */
								FMC_ErasePage(FMC_ADDR);
} 


void wirte_flash1(uint32_t page_address, int32_t address, uint32_t value){
	               if (address >= MAX_FLASH_WORDS) return; // Prevent overflow
	               uint32_t Mem_Addr = 0;
								/**  Unlock flash controler  */
								 FMC_Unlock();	                  
                 /**  Erase page  */
								FMC_ErasePage(page_address);   
				         /**  Program  */
	              Mem_Addr = page_address;
								
								SerialPrint_NumString("write value1", value);
								FMC_ProgramWord((Mem_Addr + 4*address), value);	
								/**  Lock flash controler  */
							  FMC_Lock();
          }
			 

uint32_t read_flash1(uint32_t page_address, int32_t address){
              
              p = (uint32_t*)page_address;         

							p = p + address;
							//SerialPrint_NumString("Read value", *p );
							//return p[4*address];
							return *p;			 	
	
          // SerialPrint_NumString("read data", p[0] )	;				
       }

			 
			 
void wirte_flash2(uint32_t address, uint32_t value){ 
	               uint32_t Mem_Addr = 0;
	               if (address >= MAX_FLASH_VALUES) return;
	               uint32_t buffer[MAX_FLASH_VALUES];
	
	                  // Step 1: Read all current values
										for (int i = 0; i < MAX_FLASH_VALUES; i++) {
												buffer[i] = read_flash2(i);
										}
										
										// Step 2: Update the specific value
										   buffer[address] = value;
	             
								   /**  Unlock flash controler  */
								      FMC_Unlock();	                  
                   /**  Erase page  */
								   FMC_ErasePage(FMC_ADDR);   
				           /**  Program  */
										
									 //Mem_Addr = FMC_ADDR;									
									//SerialPrint_NumString("write value", value);
										
									// Step 4: Write all values back
									for (int i = 0; i < MAX_FLASH_VALUES; i++) {
											if (buffer[i] != EMPTY_FLASH_VALUE) {
													FMC_ProgramWord(FMC_ADDR + i * 4, buffer[i]);
											}
									}		
									/**  Lock flash controler  */
									FMC_Lock();
          }



uint32_t read_flash2(uint32_t address){
              if (address >= MAX_FLASH_VALUES) return EMPTY_FLASH_VALUE;
              p = (uint32_t*)FMC_ADDR;         

							p = p + address;
							//SerialPrint_NumString("Read value", *p );
							//return p[4*address];
							return *p;			 	
	
             // SerialPrint_NumString("read data", p[0] )	;				
       }