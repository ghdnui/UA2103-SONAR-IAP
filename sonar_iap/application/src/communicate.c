
#include "stm32f0xx.h"
#include "uart.h"
#include "communicate.h"
#include <string.h>

FlagStatus 				SendMessageToPC_Flag 	= RESET;
FlagStatus WaitUp_Flag = RESET;
FlagStatus IDLE_Wait_Flag = RESET; 
uint16_t PacketRecNum = 0;
SystStatus_t  State 			= IDLE;
uint8_t      g_ucReciveDataDebug[100];   //命令段缓存数组


void ProcUartCmdHead(USART_Buf_TypeDef * ptRxBuf,USART_Buf_TypeDef * ptTxBuf)	                                                         
{	
	uint8_t  ucHeadByte, ucTailByte, ucMCUAdd, ucDataLength, ucData, ucNum;
	uint8_t  ucCheckSum=0,i;
	uint16_t iCmdBuf; 													//通信状态值
	
	if (((*ptRxBuf).ucBufCnt) >= 6)		        	//串口接收缓冲区数据大于6个后开始解析			 
	{	
		ucHeadByte = UART_ReadChar(ptRxBuf, 0); 	//读缓冲区第一个数据，即帧头  55   
		if(CMD_HEADING_HIGHBYTE == ucHeadByte) 		// 判断帧头是否正确
		{   
			ucDataLength = UART_ReadChar(ptRxBuf, 1); 	//数据帧长度
			ucMCUAdd     = UART_ReadChar(ptRxBuf, 2);  	//功能码                                  
			// 判断地址是否正确，帧长度是否在可选范围内  
			if(((*ptRxBuf).ucBufCnt>=(ucDataLength+4)))         
		    { 				
			    for(ucNum=0;ucNum<ucDataLength+2;ucNum++)			// 计算异或校验和
			    {
					ucData = UART_ReadChar(ptRxBuf, 1+ucNum); 	 
					ucCheckSum ^= ucData;
			    } 
				ucTailByte = UART_ReadChar(ptRxBuf, 1+ucNum);				
					// 判断校验和帧尾是否正确
				if((0x00==ucCheckSum)&&(CMD_ENDING_BYTE==ucTailByte))                          
				{
					if(0x24 == ucMCUAdd)     //升级上位机发来的指令
					{
						for(i = 0;i<ucDataLength+4;i++) 
						{
							g_ucReciveDataDebug[i] = UART_GetChar(ptRxBuf);
						}							
						iCmdBuf =((uint16_t)g_ucReciveDataDebug[3] <<8) +g_ucReciveDataDebug[4];  //功能码
							
						switch(iCmdBuf)
						{
							case 0x5840:               // 对应开始升级指令
								State = START;
								TotalPacketNum = ((uint16_t)g_ucReciveDataDebug[5] <<8) +g_ucReciveDataDebug[6];  //升级包大小
							break;
								
							case 0x5210:   //启动IAP
								State = Wait;
							break;
								
							case 0x5051:
								PacketRecNum = ((uint16_t)g_ucReciveDataDebug[5] <<8) +g_ucReciveDataDebug[6];
								if(Packet_Wanted == PacketRecNum)  //升级包大小验证
								{
									Packet_Length = g_ucReciveDataDebug[7];
									memcpy(Packetdata,g_ucReciveDataDebug+8,Packet_Length);		//开始接收升级代码
									//memcpy(Packetdata,g_ucReciveDataDebug[8],Packet_Length);	//开始接收升级代码
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
				else  // 校验和帧尾不不满足，则删掉帧头和地址字节                                                                        
				{
					UART_DelChar(ptRxBuf,ucDataLength+4);
				}	
			}		            														              
		}
		else 
		{
			UART_DelChar(ptRxBuf,1);					// 帧头不对，删掉帧头高字节
		}
	}  
}



