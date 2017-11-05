

#include "FLASH2EEPROM.h"
//#include "motion.h"

#include "communicate.h"
#include <string.h>

pFunction Jump_To_Application;	


uint32_t  FlashTab[StoreDataNum]  =  {
										0x08000000, //PAGE0
										0x08000400,       
										0x08000800,
										0x08000C00,
										0x08001000,
										0x08001400,
										0x08001800,
										0x08001C00,  //PAGE7 8Kb
										0x08002000,
										0x08002400,
										0x08002800,
										0x08002C00,
										0x08003000,
										0x08003400,
										0x08003800,
										0x08003C00,     //PAGE15 16kb
										0x08004000,
										0x08004400,
										0x08004800,
										0x08004C00,
										0x08005000,
										0x08005400,
										0x08005800,
										0x08005C00, 
										0x08006000,
										0x08006400,
										0x08006800,
										0x08006C00,
										0x08007000,
										0x08007400,
										0x08007800,
										0x08007C00,  // PAGE31 32kb
										0x08008000,
										0x08008400,
										0x08008800,
										0x08008C00,
										0x08009000,
										0x08009400,
										0x08009800,
										0x08009C00,     
										0x0800A000,
										0x0800A400,
										0x0800A800,
										0x0800AC00,
										0x0800B000,
										0x0800B400,
										0x0800B800,
										0x0800BC00,  //PAGE47 48kb 
										0x0800C000,
										0x0800C400,
										0x0800C800,
										0x0800CC00,
										0x0800D000,
										0x0800D400,
										0x0800D800,
										0x0800DC00,
										0x0800E000,
										0x0800E400,
										0x0800E800,
										0x0800EC00,
										0x0800F000,
										0x0800F400,
										0x0800F800,     
										0x0800FC00, //PAGE63 64kb
										0x08010000, 
										0x08010400,       
										0x08010800,
										0x08010C00,
										0x08011000,
										0x08011400,
										0x08011800,
										0x08011C00,
										0x08012000,
										0x08012400,
										0x08012800,
										0x08012C00,
										0x08013000,
										0x08013400,
										0x08013800,
										0x08013C00,  //PAGE79 80kb
										0x08014000,
										0x08014400,
										0x08014800,
										0x08014C00,
										0x08015000,
										0x08015400,
										0x08015800,
										0x08015C00, 
										0x08016000,
										0x08016400,
										0x08016800,
										0x08016C00,
										0x08017000,
										0x08017400,
										0x08017800,
										0x08017C00,  // PAGE95 96kb
										0x08018000,
										0x08018400,
										0x08018800,
										0x08018C00,
										0x08019000,
										0x08019400,
										0x08019800,
										0x08019C00,     
										0x0801A000,
										0x0801A400,
										0x0801A800,
										0x0801AC00,
										0x0801B000,
										0x0801B400,
										0x0801B800,
										0x0801BC00,  //PAGE111 112kb 
										0x0801C000,
										0x0801C400,
										0x0801C800,
										0x0801CC00,
										0x0801D000,
										0x0801D400,
										0x0801D800,
										0x0801DC00,
										0x0801E000,
										0x0801E400,
										0x0801E800,
										0x0801EC00,
										0x0801F000,
										0x0801F400,
										0x0801F800,     
										0x0801FC00, //PAGE127 128kb
										0x08020000
									};  //Flash Page 数据


ErrorStatus FLASH_WriteWordArray(uint32_t StartAddrofFLASH,uint32_t * StartAddrOfArray,uint16_t Length)
{
	//u32 EraseCounter = 0x00;						// 记录擦除FLASH时页面序号，由0开始
	uint32_t FLASH_Address = 0x00;				// 记录写操作时的FLASH地址
	volatile uint32_t * Array_Address;		// 记录写操作时的Array地址
	volatile uint32_t NbrOfPage = 0x00;		// 需要操作的FLASH页面数
	uint32_t EndAddrofFLASH;							// 操作的末地址

	volatile FLASH_Status FLASHStatus;		// 操作FLASH的返回状态
	volatile TestStatus MemoryProgramStatus;

	FLASHStatus = FLASH_COMPLETE;
	MemoryProgramStatus = PASSED;

	EndAddrofFLASH = StartAddrofFLASH + Length*4 - 1;

	/* Unlock the Flash Program Erase controller */
	FLASH_Unlock();

	/* Define the number of page to be erased */
	NbrOfPage = 0x08014000;

	/* Clear All pending flags */
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP  | FLASH_FLAG_PGERR  | FLASH_FLAG_WRPERR);	

	// FLASH在写(编程)操作之前必须对相应页面进行擦除操作，否则写(编程)操作无效，即无法改变原页面存储内容
	/* Erase the FLASH pages */
	//for(EraseCounter = 0;(EraseCounter <= NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
	//{
	//	FLASHStatus = FLASH_ErasePage(StartAddrofFLASH + (FLASH_PAGE_SIZE * EraseCounter));
	//}

	/*  FLASH Word program of data 0x15041979 at addresses defined by StartAddr and EndAddr*/
	FLASH_Address = StartAddrofFLASH;
	Array_Address = StartAddrOfArray;
	while((FLASH_Address < EndAddrofFLASH) && (FLASHStatus == FLASH_COMPLETE))
	{
		FLASHStatus = FLASH_ProgramWord(FLASH_Address,(*Array_Address));
		FLASH_Address += 4;
		Array_Address++;
	}

	/* Check the corectness of written data */
	FLASH_Address = StartAddrofFLASH;
	Array_Address = StartAddrOfArray;
	while((FLASH_Address < EndAddrofFLASH) && (MemoryProgramStatus != FAILED))
	{
		if((*(volatile uint32_t*)FLASH_Address) != (*Array_Address))
		{
			MemoryProgramStatus = FAILED;
		}
		FLASH_Address += 4;
		Array_Address++;
	}

	FLASH_Lock();

	if(MemoryProgramStatus == FAILED)
		return ERROR;

	return SUCCESS;
}



uint32_t ApplicationAddress = 0x08004000;     //APP的起始地址
uint32_t m_JumpAddress;	
//uint32_t testadd;

void Jump2APP(void)
{

	//testadd = ((*(__IO uint32_t *)ApplicationAddress)&0x2FFE0000);
	if(((*(__IO uint32_t *)ApplicationAddress)&0x2FFE0000) == 0x20000000) //判断栈顶地址是否在ram之内
	{
		RCC_DeInit();
		USART_DeInit(USART2);
//  EXTI_DeInit();
//	NVIC_SystemReset();
		GPIO_DeInit(GPIOA);
		//GPIO_DeInit(GPIOB);
		m_JumpAddress = *(__IO uint32_t *)(ApplicationAddress + 4);  				//复位地址
		Jump_To_Application = (pFunction)m_JumpAddress;  										//指向复位函数
		__set_MSP(*(__IO uint32_t*) ApplicationAddress); 										//设置主函数栈指针
		Jump_To_Application();            																	//执行复位函数，即APP
	}
	else 
	{
		State = IDLE;
	}
}

