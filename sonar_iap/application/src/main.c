

//////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include "stm32f0xx.h"
#include "stm32_config.h"
#include "FLASH2EEPROM.h"
#include "motion.h"
#include "communicate.h"
#include <string.h>
 
 
 
///////////////////////////////////////////////////////////////////////////////////
RCC_ClocksTypeDef sysclock;   // ϵͳʱ��

volatile FLASH_Status UP_FLASHStatus = FLASH_COMPLETE;
volatile ErrorStatus UP_WriteFLASHStatus = SUCCESS;

uint8_t  Send_Data[10] = {0};
uint32_t UP_Flash_Addr = FLASH_UP_START_ADDR;
uint32_t Packetdata[64] = {0};
uint8_t  WaitCount = 0;
uint8_t IDLE_Wait_Count = 0;
uint16_t Packet_Wanted = 0;
uint16_t Packet_Length;
uint16_t TotalPacketNum;
uint8_t i;
uint8_t jump2app_flag = 0;

//#define APPLICATION_ADDRESS     (uint32_t)0x08000000

//uint32_t eee = 0;
int main(void)
{


//	for(i = 0; i < 48; i++)
//  {
//    *((uint32_t*)(0x20000000 + (i << 2)))=*(__IO uint32_t*)(APPLICATION_ADDRESS + (i<<2));
//	}
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); 
//  SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
	
	RCC_Configuration();
	NVIC_Configuration();
	GPIO_Configuration();
	SysTick_Configuration();
	UART_Configuration(USART2,115200);   //����1������������
	RCC_GetClocksFreq(&sysclock);
	

	while(1)
	{
//eee++;
		ProcUartCmdHead(&RxBuf_Struct2,&TxBuf_Struct2);     //������λ��������ָ��		
		//USART_SendData(USART1,0x55);
		if(SendMessageToPC_Flag == SET)   									//1�뷢��һ��״̬����λ��
		{
			SendMessageToPC_Flag = RESET;
			Send_Data[0] = State;
			SendMessageUP(&TxBuf_Struct2,0x47,Send_Data,1);
			//55 04 25 24 47 State xx F0    //״̬����
		}
		
		switch(State)
		{
			case START:   //����׼��
			//	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
			//	USART_ITConfig(USART1, USART_IT_TC, DISABLE);  //���ж�
			//	USART_Cmd(USART1, DISABLE);//�ش���
				
				FLASH_Unlock();
				FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
			  for(i=16;i<64;i++)            //�ӵ�8kb��ʼ��������32kb
				{
					UP_FLASHStatus = FLASH_ErasePage(FlashTab[i]); //����APP��
					
					if(UP_FLASHStatus != FLASH_COMPLETE) //����ʧ��
					{
						State = Error;
						Send_Data[0] = 0x01;                
						SendMessageUP(&TxBuf_Struct2,0x44,Send_Data,1);   //֪ͨ��λ��������ʧ��
						break;
					}
				
				}
				FLASH_Lock();    //�����ɹ�
			
			//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
			//	USART_ITConfig(USART1, USART_IT_TC, ENABLE);  //���ж�
			//	USART_Cmd(USART1, ENABLE);//������
				State = WaitFotNextPacket;
				WaitCount = 0;
				Packet_Wanted = 0;
				Send_Data[0] = Packet_Wanted>>8;
				Send_Data[1] = Packet_Wanted;
				SendMessageUP(&TxBuf_Struct2,0x41,Send_Data,2);	//׼����ϣ����Խ�������	
			break;
			
			case WaitFotNextPacket:                  //�ȴ����ݣ�1��
				if(WaitUp_Flag == SET)
				{
					WaitUp_Flag = RESET;
					WaitCount++;
				}
				if(WaitCount == 10)
				{
					WaitCount = 0;
					State = TimeOut;
				}
			break;
			
			case DownLoad:
			//	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
			//	USART_ITConfig(USART1, USART_IT_TC, DISABLE);  //���ж�
			//	USART_Cmd(USART1, DISABLE);//�ش���
			
				UP_WriteFLASHStatus = FLASH_WriteWordArray(UP_Flash_Addr,Packetdata,Packet_Length/4);
			  UP_Flash_Addr = UP_Flash_Addr + Packet_Length;
			
			//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
			//	USART_ITConfig(USART1, USART_IT_TC, ENABLE);  //���ж�
			//	USART_Cmd(USART1, ENABLE);//���ж�
			 
				if(UP_WriteFLASHStatus != SUCCESS)   //д��ʧ�ܣ�����������λ���ٷ�
				{
					State = WaitFotNextPacket;
					WaitCount = 0;
					Send_Data[0] = Packet_Wanted>>8;
					Send_Data[1] = Packet_Wanted;         //��Ҫ�ĸ���
					SendMessageUP(&TxBuf_Struct2,0x41,Send_Data,2);	
				}
				else
				{
					Packet_Wanted++;      //���ճɹ���������һ����
					if(Packet_Wanted != TotalPacketNum)
					{
						State = WaitFotNextPacket;
						WaitCount = 0;
						Send_Data[0] = Packet_Wanted>>8;
						Send_Data[1] = Packet_Wanted;
						SendMessageUP(&TxBuf_Struct2,0x41,Send_Data,2);	
					}
					else
					{
						State = Completed;
						Send_Data[0] = 0x01;
						SendMessageUP(&TxBuf_Struct2,0x42,Send_Data,1);					//���������ɹ���ʾ	
					}
				}
			  
			break;
			
			case IDLE:        
				//�ȴ�1�룬���û��ָ�����������APP
				if(IDLE_Wait_Flag == SET)
				{
					IDLE_Wait_Flag = RESET;
					IDLE_Wait_Count++;
				}
				if(IDLE_Wait_Count == 10)
				{
					IDLE_Wait_Count = 0;
//					UP_Flash_Addr = ADDR_FLASH_SECTOR_4;
					Jump2APP();
				}	
				
//				//�ȴ���ת��APP��ָ��
//				if(jump2app_flag == 1)
//				{
//					Jump2APP();
//				}
//				else 
//				{
//					State = IDLE;
//				}
				
			break;
			
			case Completed:       //������ɺ��Զ�����������Ĵ���
				State = IDLE;
			break;
			
			case Error:
				State = IDLE;
			break;
			
			case TimeOut:                      //�ȴ���ʱ�ˣ���֮��λ��
				State = WaitFotNextPacket;
				WaitCount = 0;
				Send_Data[0] = Packet_Wanted>>8;
				Send_Data[1] = Packet_Wanted;
				SendMessageUP(&TxBuf_Struct2,0x41,Send_Data,2);	
			break;
			
			case Wait:
				Send_Data[0] = 0x88;
				SendMessageUP(&TxBuf_Struct2,0x45,Send_Data,1);	     //��������λ������
	// 55 04 25 24 45 88 C8 F0    //����ָ��
				State = NOP;
			break;
			
			case NOP:  	break;
			default:	break;		
		}	

	}

}


