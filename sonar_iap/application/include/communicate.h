#ifndef _COMMUNICATE_H
#define _COMMUNICATE_H

#include "stm32f0xx.h"
#include "uart.h"

#define CMD_HEADING_HIGHBYTE     	(uint8_t)0x55	  //Ö¡Í·¸ß×Ö½Ú
#define CMD_ENDING_BYTE           	(uint8_t)0xF0    //Ö¡Î²     


typedef enum 
{
	IDLE, DownLoad, WaitFotNextPacket, Completed, START, Error,TimeOut,Wait,NOP,
} SystStatus_t;


extern FlagStatus 					SendMessageToPC_Flag;
extern FlagStatus WaitUp_Flag;
extern FlagStatus IDLE_Wait_Flag; 
extern uint16_t PacketRecNum;
extern SystStatus_t  State;
extern uint16_t Packet_Wanted;
extern uint16_t TotalPacketNum;
extern uint16_t Packet_Length;
extern uint32_t Packetdata[64];
extern uint32_t Flash_Addr;
extern uint8_t jump2app_flag;
extern uint8_t  g_ucReciveDataDebug[100]; 



void ProcUartCmdHead(USART_Buf_TypeDef * ptRxBuf,USART_Buf_TypeDef * ptTxBuf);
void send_feedback_packet(uint16_t rmp,uint8_t state,uint8_t id,uint8_t Number);
uint16_t EscCrc16(uint16_t crc, uint8_t const *buffer, uint16_t len);
void SetPWM(uint16_t *_rpm);

#endif
