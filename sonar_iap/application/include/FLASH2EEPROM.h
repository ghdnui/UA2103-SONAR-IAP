
#ifndef __FLASH2EEPROM_H
#define	__FLASH2EEPROM_H



#include "stm32f0xx.h"


//////////////////////////////////////////////////////////////////////////////////////////////
//

#define StartAddr  ADDR_FLASH_PAGE_0
#define EndAddr    ADDR_FLASH_PAGE_43
#define FLASH_PAGE_SIZE    ((u16)0x400)


#define ADDR_FLASH_PAGE_0                     ((uint32_t)0x08000000)         
#define ADDR_FLASH_PAGE_15                    ((uint32_t)0x08004000)         //16KB
#define ADDR_FLASH_PAGE_79                    ((uint32_t)0x08014000)         //64KB
#define ADDR_FLASH_PAGE_127                   ((uint32_t)0x08020000)         //48KB

#define	  StoreDataNum					  		200	
extern uint32_t  FlashTab[StoreDataNum];
						
///////////////////////////////////////////////////////////////////////////////////////////////

#define FLASH_USER_START_ADDR                   (ADDR_FLASH_PAGE_79)  
#define FLASH_USER_END_ADDR                     (ADDR_FLASH_PAGE_127)   //选用SECTOR_3用来存参数

#define FLASH_UP_START_ADDR                      (ADDR_FLASH_PAGE_15)

#define SECTOR_ERASED                 					((uint16_t)0xFFFF)   
#define SECTOR_VALID           									((uint16_t)0xEEEE)    
  
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

typedef  void (*pFunction)(void);

//#define FLASH_PAGE_SIZE    ((uint32_t)0x4000)

////////////////////////////////////////////////////////////////////////////////////////////////////

uint32_t  GetSector(uint32_t Address);
uint16_t FLASH2EEPROM_Init(void);
void FLASH2EEPROM_Read(uint16_t sign, uint16_t* Data);
void FLASH2EEPROM_Write(uint16_t Sign, uint16_t Data);
ErrorStatus FLASH_WriteWordArray(uint32_t StartAddrofFLASH,uint32_t * StartAddrOfArray,uint16_t Length);
void Jump2APP(void);
FLASH_Status Flash_EraseAll(void);




#endif

