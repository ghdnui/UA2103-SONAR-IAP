
#include "stm32f0xx.h"
#include "uart.h"
#include "communicate.h"
#include <string.h>

FlagStatus 				SendMessageToPC_Flag 	= RESET;
FlagStatus WaitUp_Flag = RESET;
FlagStatus IDLE_Wait_Flag = RESET; 
uint16_t PacketRecNum = 0;
SystStatus_t  State 			= IDLE;
uint8_t      g_ucReciveDataDebug[100];   //����λ�������


void ProcUartCmdHead(USART_Buf_TypeDef * ptRxBuf,USART_Buf_TypeDef * ptTxBuf)	                                                         
{	
	uint8_t  ucHeadByte, ucTailByte, ucMCUAdd, ucDataLength, ucData, ucNum;
	uint8_t  ucCheckSum=0,i;
	uint16_t iCmdBuf; 													//ͨ��״ֵ̬
	
	if (((*ptRxBuf).ucBufCnt) >= 6)		        	//���ڽ��ջ��������ݴ���6����ʼ����			 
	{	
		ucHeadByte = UART_ReadChar(ptRxBuf, 0); 	//����������һ�����ݣ���֡ͷ  55   
		if(CMD_HEADING_HIGHBYTE == ucHeadByte) 		// �ж�֡ͷ�Ƿ���ȷ
		{   
			ucDataLength = UART_ReadChar(ptRxBuf, 1); 	//����֡����
			ucMCUAdd     = UART_ReadChar(ptRxBuf, 2);  	//������                                  
			// �жϵ�ַ�Ƿ���ȷ��֡�����Ƿ��ڿ�ѡ��Χ��  
			if(((*ptRxBuf).ucBufCnt>=(ucDataLength+4)))         
		    { 				
			    for(ucNum=0;ucNum<ucDataLength+2;ucNum++)			// �������У���
			    {
					ucData = UART_ReadChar(ptRxBuf, 1+ucNum); 	 
					ucCheckSum ^= ucData;
			    } 
				ucTailByte = UART_ReadChar(ptRxBuf, 1+ucNum);				
					// �ж�У���֡β�Ƿ���ȷ
				if((0x00==ucCheckSum)&&(CMD_ENDING_BYTE==ucTailByte))                          
				{
					if(0x24 == ucMCUAdd)     //������λ��������ָ��
					{
						for(i = 0;i<ucDataLength+4;i++) 
						{
							g_ucReciveDataDebug[i] = UART_GetChar(ptRxBuf);
						}							
						iCmdBuf =((uint16_t)g_ucReciveDataDebug[3] <<8) +g_ucReciveDataDebug[4];  //������
							
						switch(iCmdBuf)
						{
							case 0x5840:               // ��Ӧ��ʼ����ָ��
								State = START;
								TotalPacketNum = ((uint16_t)g_ucReciveDataDebug[5] <<8) +g_ucReciveDataDebug[6];  //��������С
							break;
								
							case 0x5210:   //����IAP
								State = Wait;
							break;
								
							case 0x5051:
								PacketRecNum = ((uint16_t)g_ucReciveDataDebug[5] <<8) +g_ucReciveDataDebug[6];
								if(Packet_Wanted == PacketRecNum)  //��������С��֤
								{
									Packet_Length = g_ucReciveDataDebug[7];
									memcpy(Packetdata,g_ucReciveDataDebug+8,Packet_Length);		//��ʼ������������
									//memcpy(Packetdata,g_ucReciveDataDebug[8],Packet_Length);	//��ʼ������������
									State = DownLoad;
								}
								else
								{
									State = Error;
								}
							break;
								
							case 0x2525:
								jump2app_flag = 1;
							break;
							default:	break;
						}
					}

				}
				else  // У���֡β�������㣬��ɾ��֡ͷ�͵�ַ�ֽ�                                                                        
				{
					UART_DelChar(ptRxBuf,ucDataLength+4);
				}	
			}		            														              
		}
		else 
		{
			UART_DelChar(ptRxBuf,1);					// ֡ͷ���ԣ�ɾ��֡ͷ���ֽ�
		}
	}  
}



