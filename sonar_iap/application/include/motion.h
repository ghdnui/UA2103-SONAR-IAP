/****************版权所有 (C)2010, 武汉高德红外股份有限公司***********************

* 文件名称： motion.h
* 文件标识： //000000
* 内容摘要： 此文件主要用来宏定义，声明变量和声明函数
* 其它说明： 无
* 当前版本： V1.0
* 作    者： fmc
* 完成日期： 2010年8月20日
* 
* 修改记录1：// 修改历史记录，包括修改日期、修改者及修改内容
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容： 
* 修改记录2：...

*********************************************************************************/
#ifndef	_MOTION_H
#define	_MOTION_H

/*-------------------------------开始-------------------------------------------*/

/*包含文件----------------------------------------------------------------------*/
//#include "stm32f10x_lib.h"

#include "stm32f0xx.h"
#include "uart.h"

/*宏定义------------------------------------------------------------------------*/
 
//#define   AdDataStart		              0x1000     	//EEPROM的起始地址 用于存放帧头0x55AA

//#define	  StoreDataNum					  		36		 			//EEPROM存储数据个数 六组PID参数+电机零位+角速度校准


///*全局变量声明------------------------------------------------------------------*/
//extern uint16_t  g_usADZeroYaw;
//extern uint16_t  g_usADZeroPitch;
//extern uint16_t  g_usADZeroRoll;
//extern uint16_t  uwSData[StoreDataNum];	//存储数据数组，最后一个是异或校验
//extern uint16_t  uwVirtAddVarTab[StoreDataNum];

/*全局函数声明------------------------------------------------------------------*/
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
