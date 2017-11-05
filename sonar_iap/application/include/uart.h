/****************��Ȩ���� (C)2010, �人�ߵº���ɷ����޹�˾***********************
* �ļ����ƣ�  uart.h
* �ļ���ʶ��  000000
* ����ժҪ��  ���ļ�������uart.cģ�����õ��Ľṹ������Լ�ȫ�ֺ�������
* ����˵����  
* ��ǰ�汾��  V1.0
* ��    �ߣ�  00574zsm      
* ������ڣ�  2010��6��11�� 
* �޸ļ�¼1�� ��
*********************************************************************************/
#ifndef _UART_H
#define _UART_H

#include "stm32f0xx.h"

typedef struct								    //��������������������غ����Ĳ���
{	
	uint8_t  ucBufSize;							//���������飺��������С
	uint8_t  ucBufWrInde;						//���������飺дָ��
	uint8_t  ucBufRdInde;						//���������飺��ָ��
	uint8_t  ucBufCnt;							//���������飺���ݼ���
	uint8_t  ucBufOvf;							//���������飺�����־��ע������
	uint8_t  * pcBufAddr;						//���������飺�׵�ַָ�����
	USART_TypeDef * ptUSARTx; 		  //��������Ӧ��USART
} USART_Buf_TypeDef;


typedef	struct 
{
	uint16_t *pSendData;
	uint16_t DataLength;			 			// ���ݳ���
}TransData;

#define RxBufSize1			225 			// USART1���ջ���������С200����ʹ����Ϊ0
#define TxBufSize1			250 			// USART1���ͻ���������С250����ʹ����Ϊ0

#define RxBufSize2			225 			// USART2���ջ���������С200����ʹ����Ϊ0
#define TxBufSize2			250 			// USART2���ͻ���������С250����ʹ����Ϊ0

#define RxBufSize3			225 			// USART3���ջ���������С200����ʹ����Ϊ0
#define TxBufSize3			250 			// USART3���ͻ���������С250����ʹ����Ϊ0

#define RxBufSize4			225 			// USART4���ջ���������С200����ʹ����Ϊ0
#define TxBufSize4			250 			// USART4���ͻ���������С250����ʹ����Ϊ0



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

//��������
void UART_PutChar(USART_Buf_TypeDef * ptTx, uint8_t ucData);
void UART_DelChar(USART_Buf_TypeDef * ptRx, uint8_t ucNum);
uint8_t UART_GetChar(USART_Buf_TypeDef * ptRx);
uint8_t UART_ReadChar(USART_Buf_TypeDef * ptRx,  uint8_t ucNum);
void USARTx_Rx_ISR(USART_Buf_TypeDef * ptRx);
void USARTx_Tx_ISR(USART_Buf_TypeDef * ptTx);
#endif
/*--------------------------------����------------------------------------------*/



