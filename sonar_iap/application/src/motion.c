


/* 包含文件******************************************** ******************************************/
#include "motion.h"
#include "stm32f0xx.h"
#include "stm32_config.h"
#include "communicate.h"
//#include "FLASH2EEPROM.h"
#include <math.h>

/* 私有自定义数据类型（结构struct、枚举enum、联合union） ******************************************/
/* 私有预条件（#define）定义 **********************************************************************/
/* 私有宏（Macro）定义 ****************************************************************************/
/* 私有常数（const）定义 **************************************************************************/
/* 私有（static）全局变量 *************************************************************************/

/* 公有（缺省方式、无static声明）全局变量 *********************************************************/


										
/* 局部函数声明 ***********************************************************************************/
						
										

///*******************************************************************************
//* 函数名称：  ReadZero
//* 功能描述：  电机零位值、加速度校准值、结构零位 赋值
//* 输入参数：  无
//* 输出参数：  无
//* 返 回 值：  无
//* 其它说明：  无
//* 修改日期    版本号     修改人	     修改内容
//* -----------------------------------------------
//* 2015/07/06    V1.0	      kcf	      XXXX
//*********************************************************************************/
//void ReadZero(void)
//{   
//	g_usADZeroYaw 		= uwSData[YawZeroAngle_Index];
//	g_usADZeroPitch 	= uwSData[PitchZeroAngle_Index];
//	g_usADZeroRoll 		= uwSData[RollZeroAngle_Index];   //电机零位
//	
//	mpu6500_acc_x_cal = uwSData[mpu6500AccX_Index];
//	mpu6500_acc_y_cal = uwSData[mpu6500AccY_Index];
//	mpu6500_acc_z_cal = uwSData[mpu6500AccZ_Index];    //加速度校正
//	
//	PosZeroYaw 				= uwSData[PosZeroYaw_Index];
//	PosZeroPitch 			= uwSData[PosZeroPitch_Index];
//	PosZeroRoll 			= uwSData[PosZeroRoll_Index];   //结构零位
//}										
///*******************************************************************************
//* 函数名称：  ReadEepromData
//* 功能描述：  读取EEPROM中存储的数据，如果EEPROM未初始化则进行初始化
//* 输入参数：  无
//* 输出参数：  无
//* 返 回 值：  无
//* 其它说明：  无
//* 修改日期    版本号     修改人	     修改内容
//* -----------------------------------------------
//* 2015/07/06   V1.0	      kcf	      XXXX
//*********************************************************************************/
//void ReadEepromData()
//{   
//	u16 i,uwZeroBuf;
//	FLASH_Unlock();
//	FLASH2EEPROM_Read(AdDataStart,&uwZeroBuf);
//	if(uwZeroBuf != 0x55AA)  //写入初始值
//	{
//		SetDefault();
//		FLASH2EEPROM_Write(AdDataStart, 0x55AA);
//		for(i=0;i<StoreDataNum;i++)
//		{
//			uwZeroBuf = uwSData[i];
//			FLASH2EEPROM_Write(uwVirtAddVarTab[i], uwZeroBuf);
//		}
//		ReadZero();
//		PID_Init();
//		FLASH_Lock();
//	}
//	else //读取初始值
//	{
//		for(i=0;i<StoreDataNum;i++)
//		{
//			FLASH2EEPROM_Read(uwVirtAddVarTab[i],&uwZeroBuf);
//			if((0x0000 != uwZeroBuf)&&(0xffff != uwZeroBuf))
//			{
//				uwSData[i] = uwZeroBuf;
//			}
//		}
//		ReadZero();	
//		PID_Init();		
//		FLASH_Lock();
//	}
//}



void SendMessageUP(USART_Buf_TypeDef * ptTxBuf,uint8_t Cmd,uint8_t *Senddata,uint8_t DataLength)
{
	uint8_t databuf[20];
	uint8_t i;
	uint8_t CheckSum = 0;
	
	databuf[0] = 0x55;
	databuf[1] = DataLength + 3;
	databuf[2] = 0x25;
	databuf[3] = 0x24;
	databuf[4] = Cmd;
	for(i=0;i<DataLength;i++)
	{
		databuf[5+i] = Senddata[i];
	}
	for(i=1;i<DataLength+5;i++)
	{
		CheckSum ^= databuf[i];
	}
	databuf[DataLength+5] = CheckSum;
	databuf[DataLength+6] = 0xF0;
	
	for(i=0;i<DataLength+7;i++)
	{
		UART_PutChar(ptTxBuf,databuf[i]);
	}
}


///*******************************************************************************
//* 函数名称：  SetDefault
//* 功能描述：  将uwSData数组初值设定为默认值
//* 输入参数：  无
//* 输出参数：  无
//* 返 回 值：  无
//* 其它说明：  无
//* 修改日期    版本号     修改人	     修改内容
//* -----------------------------------------------
//* 2015/07/06   V1.0	      kcf	      XXXX
//*********************************************************************************/
//void SetDefault(void)				
//{
//	uwSData[Yaw_PosPidKp_Index]				= YAW_PID_POS_KP_DEFAULT;
//	uwSData[Yaw_PosPidKi_Index]				= YAW_PID_POS_KI_DEFAULT;
//	uwSData[Yaw_PosPidKd_Index]	    	= YAW_PID_POS_KD_DEFAULT;
//	
//	uwSData[Roll_PosPidKp_Index]			= ROLL_PID_POS_KP_DEFAULT;
//	uwSData[Roll_PosPidKi_Index]			= ROLL_PID_POS_KI_DEFAULT;
//	uwSData[Roll_PosPidKd_Index]			= ROLL_PID_POS_KD_DEFAULT;
//	
//	uwSData[Pitch_PosPidKp_Index]	    = PITCH_PID_POS_KP_DEFAULT;
//	uwSData[Pitch_PosPidKi_Index]		  = PITCH_PID_POS_KI_DEFAULT;
//	uwSData[Pitch_PosPidKd_Index]			= PITCH_PID_POS_KD_DEFAULT;
//	
//	uwSData[Yaw_SpdPidKp_Index]				= YAW_PID_SPEED_KP_DEFAULT;
//	uwSData[Yaw_SpdPidKi_Index]				= YAW_PID_SPEED_KI_DEFAULT;
//	uwSData[Yaw_SpdPidKd_Index]				= YAW_PID_SPEED_KD_DEFAULT;
//	
//	uwSData[Roll_SpdPidKp_Index]		  = ROLL_PID_SPEED_KP_DEFAULT;
//	uwSData[Roll_SpdPidKi_Index]		  = ROLL_PID_SPEED_KI_DEFAULT;
//	uwSData[Roll_SpdPidKd_Index]			= ROLL_PID_SPEED_KD_DEFAULT;
//	
//	uwSData[Pitch_SpdPidKp_Index]			= PITCH_PID_SPEED_KP_DEFAULT;
//	uwSData[Pitch_SpdPidKi_Index]			= PITCH_PID_SPEED_KI_DEFAULT;
//	uwSData[Pitch_SpdPidKd_Index]			= PITCH_PID_SPEED_KD_DEFAULT;

//	uwSData[YawZeroAngle_Index]				= YawZeroAngle_DEFAULT;
//	uwSData[RollZeroAngle_Index]			= RollZeroAngle_DEFAULT;
//	uwSData[PitchZeroAngle_Index]			= PitchZeroAngle_DEFAULT;   //电机零位
//	
//	uwSData[mpu6500AccX_Index]		 	  = mpu6500AccX_DEFAULT;
//	uwSData[mpu6500AccY_Index]		  	= mpu6500AccY_DEFAULT;
//	uwSData[mpu6500AccZ_Index]				= mpu6500AccZ_DEFAULT;	
//	
//	uwSData[PosZeroYaw_Index]		 	    = PosZeroYaw_DEFAULT;
//	uwSData[PosZeroRoll_Index]		  	= PosZeroRoll_DEFAULT;
//	uwSData[PosZeroPitch_Index]				= PosZeroPitch_DEFAULT;		//结构零位
//	
//	uwSData[Pitch_Cir_Kp_Index]	  		= Pitch_PID_Cir_KP_DEFAULT;
//	uwSData[Pitch_Cir_Ki_Index]	  		= Pitch_PID_Cir_KI_DEFAULT;
//	uwSData[Pitch_Cir_Kd_Index]	  		= Pitch_PID_Cir_KD_DEFAULT;
//	uwSData[Roll_Cir_Kp_Index]				= ROLL_PID_Cir_KP_DEFAULT;
//	uwSData[Roll_Cir_Ki_Index]				= ROLL_PID_Cir_KI_DEFAULT;
//	uwSData[Roll_Cir_Kd_Index]				= ROLL_PID_Cir_KD_DEFAULT;
//	uwSData[Yaw_Cir_Kp_Index]					= YAW_PID_Cir_KP_DEFAULT;
//	uwSData[Yaw_Cir_Ki_Index]					= YAW_PID_Cir_KI_DEFAULT;
//	uwSData[Yaw_Cir_Kd_Index]					= YAW_PID_Cir_KD_DEFAULT;
//	
//}

/*******************************************************************************
* 函数名称：  PID_Zero
* 功能描述：  将三个轴速度环位置环的PID参数（积分项、参考值）清零
* 输入参数：  无
* 输出参数：  无
* 返 回 值：  无
* 其它说明：  无
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
* 2015/07/06   V1.0	      kcf	      XXXX
*********************************************************************************/
//void PID_Zero()
//{
//	Yaw_PID_Pos_InitStructure.wIntegral 		= 0;
//	Pitch_PID_Pos_InitStructure.wIntegral 	= 0;
//	Roll_PID_Pos_InitStructure.wIntegral 		= 0;
//	Yaw_PID_Speed_InitStructure.wIntegral 	= 0;
//	Pitch_PID_Speed_InitStructure.wIntegral = 0;
//	Roll_PID_Speed_InitStructure.wIntegral 	= 0;
//	yaw_pos_reference 	= 0;
//	pitch_pos_reference = 0;
//	roll_pos_reference 	= 0;
//	yaw_spd_reference 	= 0;
//	pitch_spd_reference = 0;
//	roll_spd_reference 	= 0;
//}
/*----------------局部函数实现---------------------------------------------------*/
