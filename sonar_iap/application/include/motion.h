/****************��Ȩ���� (C)2010, �人�ߵº���ɷ����޹�˾***********************

* �ļ����ƣ� motion.h
* �ļ���ʶ�� //000000
* ����ժҪ�� ���ļ���Ҫ�����궨�壬������������������
* ����˵���� ��
* ��ǰ�汾�� V1.0
* ��    �ߣ� fmc
* ������ڣ� 2010��8��20��
* 
* �޸ļ�¼1��// �޸���ʷ��¼�������޸����ڡ��޸��߼��޸�����
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ� 
* �޸ļ�¼2��...

*********************************************************************************/
#ifndef	_MOTION_H
#define	_MOTION_H

/*-------------------------------��ʼ-------------------------------------------*/

/*�����ļ�----------------------------------------------------------------------*/
//#include "stm32f10x_lib.h"

#include "stm32f0xx.h"
#include "uart.h"

/*�궨��------------------------------------------------------------------------*/
 
//#define   AdDataStart		              0x1000     	//EEPROM����ʼ��ַ ���ڴ��֡ͷ0x55AA

//#define	  StoreDataNum					  		36		 			//EEPROM�洢���ݸ��� ����PID����+�����λ+���ٶ�У׼


///*ȫ�ֱ�������------------------------------------------------------------------*/
//extern uint16_t  g_usADZeroYaw;
//extern uint16_t  g_usADZeroPitch;
//extern uint16_t  g_usADZeroRoll;
//extern uint16_t  uwSData[StoreDataNum];	//�洢�������飬���һ�������У��
//extern uint16_t  uwVirtAddVarTab[StoreDataNum];

/*ȫ�ֺ�������------------------------------------------------------------------*/
//void SetDefault(void);	
//void ReadZero(void);
//void ReadEepromData(void);	
//void SendZeroToAll(void); 
//void SendMessageToPC(void);
//void SaveZero(uint8_t Index,uint16_t Data);
//void SendDebugData(void);
//void Acc_Calibrate(void);
//void ACC_Compare(void);
//void PID_Zero(void);

//void SendZeroToAll(void);
//void Send_CirYaw_PIDToAll(void);
//void Send_CirRoll_PIDToAll(void);
//void Send_CirPitch_PIDToAll(void);
void SendMessageUP(USART_Buf_TypeDef * ptTxBuf,uint8_t Cmd,uint8_t *Senddata,uint8_t DataLength);


#endif 
