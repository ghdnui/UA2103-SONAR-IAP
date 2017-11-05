/****************版权所有 (C)2010, 武汉高德红外股份有限公司***********************

* 文件名称：  UART.c
* 文件标识：  000000
* 内容摘要：  该模块为对数据的缓冲区处理的函数
* 其它说明：  
*		 					函数列表:     
*    					1. -------UART_GetChar
*    					2. -------UART_ReadChar
*    					3. -------UART_DelChar     
*  	 					4. -------UART_PutChar
*  	 					5. -------USARTx_Tx_ISR
*  	 					6. -------USARTx_Rx_ISR
* 当前版本：  V1.0
* 作    者：  00574zsm      
* 完成日期：  2010年6月11日 
* 修改记录1： 无
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容：
*********************************************************************************/
#include "uart.h"

/* 私有自定义数据类型（结构struct、枚举enum、联合union） ******************************************/
/* 私有预条件（#define）定义 **********************************************************************/
/* 私有宏（Macro）定义 ****************************************************************************/
/* 私有常数（const）定义 **************************************************************************/
/* 私有（static）全局变量 *************************************************************************/
/* 公有（缺省方式、无static声明）全局变量 *********************************************************/
#if RxBufSize1 != 0
	uint8_t RxBuf1[RxBufSize1];
	USART_Buf_TypeDef RxBuf_Struct1={RxBufSize1,0,0,0,0,RxBuf1,USART1};
#endif

#if TxBufSize1 != 0
	uint8_t TxBuf1[TxBufSize1];
	USART_Buf_TypeDef TxBuf_Struct1={TxBufSize1,0,0,0,0,TxBuf1,USART1};
#endif

#if RxBufSize2 != 0
	uint8_t RxBuf2[RxBufSize2];
	USART_Buf_TypeDef RxBuf_Struct2={RxBufSize2,0,0,0,0,RxBuf2,USART2};
#endif

#if TxBufSize2 != 0
	uint8_t TxBuf2[TxBufSize2];
	USART_Buf_TypeDef TxBuf_Struct2={TxBufSize2,0,0,0,0,TxBuf2,USART2};
#endif
	
#if RxBufSize3 != 0
	uint8_t RxBuf3[RxBufSize3];
	USART_Buf_TypeDef RxBuf_Struct3={RxBufSize3,0,0,0,0,RxBuf3,USART3};
#endif

#if TxBufSize3 != 0
	uint8_t TxBuf3[TxBufSize3];
	USART_Buf_TypeDef TxBuf_Struct3={TxBufSize3,0,0,0,0,TxBuf3,USART3};
#endif

#if RxBufSize4 != 0
	uint8_t RxBuf4[RxBufSize4];
	USART_Buf_TypeDef RxBuf_Struct4={RxBufSize4,0,0,0,0,RxBuf4,USART4};
#endif

#if TxBufSize4 != 0
	uint8_t TxBuf4[TxBufSize4];
	USART_Buf_TypeDef TxBuf_Struct4={TxBufSize4,0,0,0,0,TxBuf4,USART4};
#endif


/*******************************************************************************
* 函数名  :  	  UART_GetChar
* 函数描述:       从接收缓冲区读一字节，并清除此字节，需确保缓冲区内有数据。
* 输入参数:       - USART_Buf_TypeDef * ptRx: 接收缓冲区结构体的首地址
*
* 输出参数:       无
* 返 回 值:       从接收缓冲区读取那个字节的数据
* 其它说明： 
* 修改日期    	  版本号     修改人	     修改内容
* -----------------------------------------------
* 2010/06/11      V1.0	     00574zsm	       XXXX
*******************************************************************************/
uint8_t UART_GetChar(USART_Buf_TypeDef * ptRx)
{	
	uint8_t ucData;
	ucData = *((*ptRx).pcBufAddr + (*ptRx).ucBufRdInde);
	if ((*ptRx).ucBufCnt != 0)
	{	
		if (++(*ptRx).ucBufRdInde == (*ptRx).ucBufSize)
		{
		   (*ptRx).ucBufRdInde = 0;
		}
		USART_ITConfig((*ptRx).ptUSARTx,USART_IT_RXNE,DISABLE);//关接收中断
		--(*ptRx).ucBufCnt;
		USART_ITConfig((*ptRx).ptUSARTx,USART_IT_RXNE,ENABLE);//开接收中断
	}
	return ucData;
}
/*******************************************************************************
* 函数名  : 			 UART_ReadChar
* 函数功能:        从接收缓冲区读取指定位置的一字节，但不清除此字节，需确保缓冲区内有数据。
* 输入参数:        - USART_Buf_TypeDef * stRx： 接收缓冲区结构体的地址
*									 - ucNum：指定缓冲区中要读取字节的位置
* 输出参数:        无
* 返 回 值: 			 从接收缓冲区读取那个字节的数据
* 其它说明： 
* 修改日期    	  版本号     修改人	     修改内容
* -----------------------------------------------
* 2010/06/11      V1.0	     00574zsm	       XXXX
*******************************************************************************/
uint8_t UART_ReadChar(USART_Buf_TypeDef * ptRx, uint8_t ucNum)	 
{	
	uint8_t ucData;
	uint16_t  i;
	i = ucNum;
	if ((*ptRx).ucBufCnt >= ucNum)
	{
		i += (*ptRx).ucBufRdInde;
		if (i >= (*ptRx).ucBufSize)
		{	
			i -=((*ptRx).ucBufSize);
		}
	}
	else
	{
	  i=0;
	}
	ucData = *((*ptRx).pcBufAddr + i);	
	return ucData;
}

/*******************************************************************************
* 函数名  :			   UART_DelChar
* 函数功能:        从接收缓冲区清除指定长度的数据，
* 输入参数:        - USART_Buf_TypeDef * ptRx: 接收缓冲区结构体的地址
*									 - ucNum：指定缓冲区中要删除字节的长度									
* 输出参数:        无
* 返 回 值:        无
* 其它说明： 
* 修改日期    	  版本号     修改人	     修改内容
* -----------------------------------------------
* 2010/06/11      V1.0	     00574zsm	       XXXX
*******************************************************************************/
void UART_DelChar(USART_Buf_TypeDef * ptRx, uint8_t ucNum)	 
{	
	uint16_t i;
	if ((*ptRx).ucBufCnt >= ucNum)	
	{	
		USART_ITConfig((*ptRx).ptUSARTx,USART_IT_RXNE,DISABLE);//关接收中断
		(*ptRx).ucBufCnt -= ucNum;	
		USART_ITConfig((*ptRx).ptUSARTx,USART_IT_RXNE,ENABLE);//开接收中断
		i = ucNum;
		i += (*ptRx).ucBufRdInde;
		if (i >= (*ptRx).ucBufSize)
		{
		  i -= (*ptRx).ucBufSize;
		}
		(*ptRx).ucBufRdInde = i;
	 }
	 else
	 {  
	    i = (*ptRx).ucBufCnt;
		i += (*ptRx).ucBufRdInde;
		if (i >= (*ptRx).ucBufSize)
		{
		  i -= (*ptRx).ucBufSize;
		}
		(*ptRx).ucBufRdInde = i;
		USART_ITConfig((*ptRx).ptUSARTx,USART_IT_RXNE,DISABLE);//关接收中断
		(*ptRx).ucBufCnt = 0;
		USART_ITConfig((*ptRx).ptUSARTx,USART_IT_RXNE,ENABLE);//开接收中断
	 }
}

/*******************************************************************************
* 函数名  :			   UART_PutChar
* 函数功能:        向发送缓冲区写入一个字节数据
* 输入参数:        - USART_Buf_TypeDef * ptTx: 发送缓冲区结构体的首地址
*									 - cData：写入发送缓冲区中的数据									
* 输出参数:        无
* 返 回 值:        无
* 其它说明： 
* 修改日期    	  版本号     修改人	     修改内容
* -----------------------------------------------
* 2010/06/11      V1.0	     00574zsm	       XXXX
*******************************************************************************/

void UART_PutChar(USART_Buf_TypeDef * ptTx, uint8_t ucData)
{	
	while ((*ptTx).ucBufCnt == (*ptTx).ucBufSize);			 //发送缓冲满，等待。若有此情况建议增大缓冲区
	USART_ITConfig((*ptTx).ptUSARTx,USART_IT_TC,DISABLE);//关发送中断
	if((*ptTx).ucBufCnt == 0)
	{
		if(USART_GetFlagStatus((*ptTx).ptUSARTx, USART_FLAG_TXE)== SET) //check if transmit data register full or not
		{
			USART_SendData((*ptTx).ptUSARTx,ucData);
			USART_ITConfig((*ptTx).ptUSARTx,USART_IT_TC,ENABLE);//开发送中断
			return;
		}
	}	
	*((*ptTx).pcBufAddr + (*ptTx).ucBufWrInde) = ucData;
	if(++(*ptTx).ucBufWrInde == (*ptTx).ucBufSize)
	{
	  (*ptTx).ucBufWrInde = 0;
	}
	++(*ptTx).ucBufCnt;
	USART_ITConfig((*ptTx).ptUSARTx,USART_IT_TC,ENABLE);//开发送中断
}

/*******************************************************************************
* 函数名  :        USARTx_Tx_ISR
* 函数功能:        此函数为串口发送完成中断的服务函数
* 输入    :        - USART_Buf_TypeDef * ptTx: 发送缓冲区结构体的首地址
* 输出    :        无
* 返 回 值:        无
* 其它说明： 
* 修改日期    	  版本号     修改人	     修改内容
* -----------------------------------------------
* 2010/06/11      V1.0	     00574zsm	       XXXX
*******************************************************************************/
void USARTx_Tx_ISR(USART_Buf_TypeDef * ptTx)
{  
   if(USART_GetITStatus((*ptTx).ptUSARTx,USART_IT_TC) != RESET)        //发送完成中断
   {
      USART_ClearFlag((*ptTx).ptUSARTx,USART_FLAG_TC);
	  	if((* ptTx).ucBufCnt)
	  	{
			--(* ptTx).ucBufCnt;
			USART_SendData((*ptTx).ptUSARTx,*((* ptTx).pcBufAddr + (* ptTx).ucBufRdInde));
			if(++(* ptTx).ucBufRdInde >= (* ptTx).ucBufSize)
			{
			  (* ptTx).ucBufRdInde = 0;
			}
	  	}
   } 
}

/*******************************************************************************
* 函数名  :        USARTx_Rx_ISR
* 函数功能:        此函数为串口接收中断的服务函数
* 输入参数:        - USART_Buf_TypeDef * ptRx: 接收缓冲区结构体的地址
* 输出参数:				 无
* 返 回 值:        无
* 其它说明： 
* 修改日期    	   版本号     修改人	     修改内容
* -----------------------------------------------
* 2010/06/11       V1.0	     00574zsm	       XXXX
*******************************************************************************/
void USARTx_Rx_ISR(USART_Buf_TypeDef * ptRx)
{
   uint8_t ucData;
		
	if(USART_GetFlagStatus((*ptRx).ptUSARTx,USART_FLAG_ORE) !=RESET)
	{
		USART_ClearFlag((*ptRx).ptUSARTx,USART_FLAG_ORE);
	}
	if(USART_GetITStatus((*ptRx).ptUSARTx,USART_IT_RXNE) != RESET)   //接收中断
	{
      USART_ClearFlag((*ptRx).ptUSARTx,USART_FLAG_RXNE);
	  	ucData = USART_ReceiveData((*ptRx).ptUSARTx);
	
	  	if((((*ptRx).ptUSARTx->ISR)& 0x0000) == 0)//无错误0x000F;
	  	{
			*((* ptRx).pcBufAddr + (* ptRx).ucBufWrInde) = ucData;      
			if(++(* ptRx).ucBufWrInde >= (* ptRx).ucBufSize)
			{
				(* ptRx).ucBufWrInde = 0; 
			}	
			if(++(* ptRx).ucBufCnt >= (* ptRx).ucBufSize)  
			{
				(* ptRx).ucBufCnt = 1;
		   		(* ptRx).ucBufOvf = 1;
			}
		}
	}
} 


/*--------------------------------结束------------------------------------------*/
