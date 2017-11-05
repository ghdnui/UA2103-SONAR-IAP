/****************��Ȩ���� (C)2010, �人�ߵº���ɷ����޹�˾***********************

* �ļ����ƣ�  UART.c
* �ļ���ʶ��  000000
* ����ժҪ��  ��ģ��Ϊ�����ݵĻ���������ĺ���
* ����˵����  
*		 					�����б�:     
*    					1. -------UART_GetChar
*    					2. -------UART_ReadChar
*    					3. -------UART_DelChar     
*  	 					4. -------UART_PutChar
*  	 					5. -------USARTx_Tx_ISR
*  	 					6. -------USARTx_Rx_ISR
* ��ǰ�汾��  V1.0
* ��    �ߣ�  00574zsm      
* ������ڣ�  2010��6��11�� 
* �޸ļ�¼1�� ��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ�
*********************************************************************************/
#include "uart.h"

/* ˽���Զ����������ͣ��ṹstruct��ö��enum������union�� ******************************************/
/* ˽��Ԥ������#define������ **********************************************************************/
/* ˽�к꣨Macro������ ****************************************************************************/
/* ˽�г�����const������ **************************************************************************/
/* ˽�У�static��ȫ�ֱ��� *************************************************************************/
/* ���У�ȱʡ��ʽ����static������ȫ�ֱ��� *********************************************************/
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
* ������  :  	  UART_GetChar
* ��������:       �ӽ��ջ�������һ�ֽڣ���������ֽڣ���ȷ���������������ݡ�
* �������:       - USART_Buf_TypeDef * ptRx: ���ջ������ṹ����׵�ַ
*
* �������:       ��
* �� �� ֵ:       �ӽ��ջ�������ȡ�Ǹ��ֽڵ�����
* ����˵���� 
* �޸�����    	  �汾��     �޸���	     �޸�����
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
		USART_ITConfig((*ptRx).ptUSARTx,USART_IT_RXNE,DISABLE);//�ؽ����ж�
		--(*ptRx).ucBufCnt;
		USART_ITConfig((*ptRx).ptUSARTx,USART_IT_RXNE,ENABLE);//�������ж�
	}
	return ucData;
}
/*******************************************************************************
* ������  : 			 UART_ReadChar
* ��������:        �ӽ��ջ�������ȡָ��λ�õ�һ�ֽڣ�����������ֽڣ���ȷ���������������ݡ�
* �������:        - USART_Buf_TypeDef * stRx�� ���ջ������ṹ��ĵ�ַ
*									 - ucNum��ָ����������Ҫ��ȡ�ֽڵ�λ��
* �������:        ��
* �� �� ֵ: 			 �ӽ��ջ�������ȡ�Ǹ��ֽڵ�����
* ����˵���� 
* �޸�����    	  �汾��     �޸���	     �޸�����
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
* ������  :			   UART_DelChar
* ��������:        �ӽ��ջ��������ָ�����ȵ����ݣ�
* �������:        - USART_Buf_TypeDef * ptRx: ���ջ������ṹ��ĵ�ַ
*									 - ucNum��ָ����������Ҫɾ���ֽڵĳ���									
* �������:        ��
* �� �� ֵ:        ��
* ����˵���� 
* �޸�����    	  �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 2010/06/11      V1.0	     00574zsm	       XXXX
*******************************************************************************/
void UART_DelChar(USART_Buf_TypeDef * ptRx, uint8_t ucNum)	 
{	
	uint16_t i;
	if ((*ptRx).ucBufCnt >= ucNum)	
	{	
		USART_ITConfig((*ptRx).ptUSARTx,USART_IT_RXNE,DISABLE);//�ؽ����ж�
		(*ptRx).ucBufCnt -= ucNum;	
		USART_ITConfig((*ptRx).ptUSARTx,USART_IT_RXNE,ENABLE);//�������ж�
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
		USART_ITConfig((*ptRx).ptUSARTx,USART_IT_RXNE,DISABLE);//�ؽ����ж�
		(*ptRx).ucBufCnt = 0;
		USART_ITConfig((*ptRx).ptUSARTx,USART_IT_RXNE,ENABLE);//�������ж�
	 }
}

/*******************************************************************************
* ������  :			   UART_PutChar
* ��������:        ���ͻ�����д��һ���ֽ�����
* �������:        - USART_Buf_TypeDef * ptTx: ���ͻ������ṹ����׵�ַ
*									 - cData��д�뷢�ͻ������е�����									
* �������:        ��
* �� �� ֵ:        ��
* ����˵���� 
* �޸�����    	  �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 2010/06/11      V1.0	     00574zsm	       XXXX
*******************************************************************************/

void UART_PutChar(USART_Buf_TypeDef * ptTx, uint8_t ucData)
{	
	while ((*ptTx).ucBufCnt == (*ptTx).ucBufSize);			 //���ͻ��������ȴ������д�����������󻺳���
	USART_ITConfig((*ptTx).ptUSARTx,USART_IT_TC,DISABLE);//�ط����ж�
	if((*ptTx).ucBufCnt == 0)
	{
		if(USART_GetFlagStatus((*ptTx).ptUSARTx, USART_FLAG_TXE)== SET) //check if transmit data register full or not
		{
			USART_SendData((*ptTx).ptUSARTx,ucData);
			USART_ITConfig((*ptTx).ptUSARTx,USART_IT_TC,ENABLE);//�������ж�
			return;
		}
	}	
	*((*ptTx).pcBufAddr + (*ptTx).ucBufWrInde) = ucData;
	if(++(*ptTx).ucBufWrInde == (*ptTx).ucBufSize)
	{
	  (*ptTx).ucBufWrInde = 0;
	}
	++(*ptTx).ucBufCnt;
	USART_ITConfig((*ptTx).ptUSARTx,USART_IT_TC,ENABLE);//�������ж�
}

/*******************************************************************************
* ������  :        USARTx_Tx_ISR
* ��������:        �˺���Ϊ���ڷ�������жϵķ�����
* ����    :        - USART_Buf_TypeDef * ptTx: ���ͻ������ṹ����׵�ַ
* ���    :        ��
* �� �� ֵ:        ��
* ����˵���� 
* �޸�����    	  �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 2010/06/11      V1.0	     00574zsm	       XXXX
*******************************************************************************/
void USARTx_Tx_ISR(USART_Buf_TypeDef * ptTx)
{  
   if(USART_GetITStatus((*ptTx).ptUSARTx,USART_IT_TC) != RESET)        //��������ж�
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
* ������  :        USARTx_Rx_ISR
* ��������:        �˺���Ϊ���ڽ����жϵķ�����
* �������:        - USART_Buf_TypeDef * ptRx: ���ջ������ṹ��ĵ�ַ
* �������:				 ��
* �� �� ֵ:        ��
* ����˵���� 
* �޸�����    	   �汾��     �޸���	     �޸�����
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
	if(USART_GetITStatus((*ptRx).ptUSARTx,USART_IT_RXNE) != RESET)   //�����ж�
	{
      USART_ClearFlag((*ptRx).ptUSARTx,USART_FLAG_RXNE);
	  	ucData = USART_ReceiveData((*ptRx).ptUSARTx);
	
	  	if((((*ptRx).ptUSARTx->ISR)& 0x0000) == 0)//�޴���0x000F;
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


/*--------------------------------����------------------------------------------*/
