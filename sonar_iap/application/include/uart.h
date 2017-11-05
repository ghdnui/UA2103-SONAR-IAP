/****************版权所有 (C)2010, 武汉高德红外股份有限公司***********************
* 文件名称：  uart.h
* 文件标识：  000000
* 内容摘要：  该文件包含了uart.c模块所用到的结构体变量以及全局函数声明
* 其它说明：  
* 当前版本：  V1.0
* 作    者：  00574zsm      
* 完成日期：  2010年6月11日 
* 修改记录1： 无
*********************************************************************************/
#ifndef _UART_H
#define _UART_H

#include "stm32f0xx.h"

typedef struct								    //缓冲区各变量，用于相关函数的参数
{	
	uint8_t  ucBufSize;							//缓冲区数组：缓冲区大小
	uint8_t  ucBufWrInde;						//缓冲区数组：写指针
	uint8_t  ucBufRdInde;						//缓冲区数组：读指针
	uint8_t  ucBufCnt;							//缓冲区数组：数据计数
	uint8_t  ucBufOvf;							//缓冲区数组：溢出标志，注意清零
	uint8_t  * pcBufAddr;						//缓冲区数组：首地址指针变量
	USART_TypeDef * ptUSARTx; 		  //缓冲区对应的USART
} USART_Buf_TypeDef;


typedef	struct 
{
	uint16_t *pSendData;
	uint16_t DataLength;			 			// 数据长度
}TransData;

#define RxBufSize1			225 			// USART1接收缓冲区，大小200，不使用设为0
#define TxBufSize1			250 			// USART1发送缓冲区，大小250，不使用设为0

#define RxBufSize2			225 			// USART2接收缓冲区，大小200，不使用设为0
#define TxBufSize2			250 			// USART2发送缓冲区，大小250，不使用设为0

#define RxBufSize3			225 			// USART3接收缓冲区，大小200，不使用设为0
#define TxBufSize3			250 			// USART3发送缓冲区，大小250，不使用设为0

#define RxBufSize4			225 			// USART4接收缓冲区，大小200，不使用设为0
#define TxBufSize4			250 			// USART4发送缓冲区，大小250，不使用设为0



#if RxBufSize1 != 0
	extern uint8_t RxBuf1[RxBufSize1];
	extern USART_Buf_TypeDef RxBuf_Struct1;
#endif

#if TxBufSize1 != 0
	extern uint8_t TxBuf1[TxBufSize1];
	extern USART_Buf_TypeDef TxBuf_Struct1;
#endif

#if RxBufSize2 != 0
	extern uint8_t RxBuf2[RxBufSize2];
	extern USART_Buf_TypeDef RxBuf_Struct2;
#endif

#if TxBufSize2 != 0
	extern uint8_t TxBuf2[TxBufSize2];
	extern USART_Buf_TypeDef TxBuf_Struct2;
#endif

#if RxBufSize3 != 0
	extern uint8_t RxBuf3[RxBufSize3];
	extern USART_Buf_TypeDef RxBuf_Struct3;
#endif

#if TxBufSize3 != 0
	extern uint8_t TxBuf3[TxBufSize3];
	extern USART_Buf_TypeDef TxBuf_Struct3;
#endif

#if RxBufSize4 != 0
	extern uint8_t RxBuf4[RxBufSize4];
	extern USART_Buf_TypeDef RxBuf_Struct4;
#endif

#if TxBufSize4 != 0
	extern uint8_t TxBuf4[TxBufSize4];
	extern USART_Buf_TypeDef TxBuf_Struct4;
#endif

//函数声明
void UART_PutChar(USART_Buf_TypeDef * ptTx, uint8_t ucData);
void UART_DelChar(USART_Buf_TypeDef * ptRx, uint8_t ucNum);
uint8_t UART_GetChar(USART_Buf_TypeDef * ptRx);
uint8_t UART_ReadChar(USART_Buf_TypeDef * ptRx,  uint8_t ucNum);
void USARTx_Rx_ISR(USART_Buf_TypeDef * ptRx);
void USARTx_Tx_ISR(USART_Buf_TypeDef * ptTx);
#endif
/*--------------------------------结束------------------------------------------*/



