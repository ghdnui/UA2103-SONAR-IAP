

#include "stm32f0xx.h"
  
void RCC_Configuration(void)
{

	ErrorStatus HSEStartUpStatus;
	RCC_DeInit();  
	RCC_HSEConfig(RCC_HSE_ON); //INPUT HSE = 12M   
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus == SUCCESS)   
	{  
		FLASH_PrefetchBufferCmd(ENABLE);    //M0 defined   
		FLASH_SetLatency(FLASH_Latency_1);  //M0 defined   
		RCC_PREDIV1Config(RCC_PREDIV1_Div1);  
		RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_4);   //PLL CLK = HSE *RCC_PLLSource_PREDIV1*2 = 48M    
		RCC_PLLCmd(ENABLE);                                   //enable PLL CLK        
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)    //WAIT PLL CLK ready   
		{  
		}  
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //SELECT SYSCLK = PLLCLK =48M  
		// *           - 0x00: HSI used as system clock  
		// *           - 0x04: HSE used as system clock    
		// *           - 0x08: PLL used as system clock  
		// *           - 0x0C: HSI48 used as system clock, applicable only for STM32F072 devices    
		while(RCC_GetSYSCLKSource()!=0x08); 
		RCC_HCLKConfig(RCC_SYSCLK_Div1); //AHB CLK(HCLK) = SYS CLK = 48M  
		RCC_PCLKConfig(RCC_HCLK_Div1);   //APB CLK(PCLK) = HCLK = 48M  
		RCC_USARTCLKConfig(RCC_USART1CLK_PCLK);  // UART2 CLK = 48M  
		RCC_USARTCLKConfig(RCC_USART2CLK_PCLK);  // UART2 CLK = 48M  
		RCC_USARTCLKConfig(RCC_USART3CLK_PCLK);  // UART2 CLK = 48M  
	}  

	//外设时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2| RCC_APB1Periph_PWR,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_SYSCFG,ENABLE);
									
}	



void  NVIC_Configuration(void)
{
	NVIC_InitTypeDef	NVIC_InitStructure;
	
//配置USART3中断优先级
//	NVIC_InitStructure.NVIC_IRQChannel = USART3_4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	
//配置USART2中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}


void SysTick_Configuration(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);     //HCLK=8M
	SysTick_Config(48000);                               //1ms,systick的中断配置在此文件中
}


void GPIO_Configuration(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	
//	//配置PB.10(TX) PB.11(RX)作为串口3的功能管脚
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_4);
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_4);
	
	//配置PA.9(TX) PA.10(RX)作为串口1的功能管脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_1);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);	
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_2);
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_2);
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_2);
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_2);

//配置PA.2(TX) PA.3(RX)作为串口2的功能管脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_1);
}


void TIMER1_Configuration(void) 
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	//---------------TIM1的配置----------------------------------//
	TIM_TimeBaseStructure.TIM_Period = 2000;
	TIM_TimeBaseStructure.TIM_Prescaler = 47;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
  TIM_OCInitStructure.TIM_Pulse = 1000; 
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	
  TIM_OC1Init(TIM1, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure); 
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure); 
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure); 
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
  TIM_CtrlPWMOutputs(TIM1,ENABLE);            
  TIM_ARRPreloadConfig(TIM1, ENABLE); 
  TIM_Cmd(TIM1, ENABLE);
}



void UART_Configuration(USART_TypeDef *ptUart,uint32_t BaudRate)
{
	USART_InitTypeDef   USART_InitStructure;

	//---------USART1的基本配置-------------------------//
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(ptUart, &USART_InitStructure);
	
	// 使能USART,会发送一帧空闲帧,引发TxE和TC置位
	USART_Cmd(ptUart, ENABLE);
	
	// 避免第一次不必要的TC中断
	USART_ClearITPendingBit(ptUart, USART_IT_TC);

	// Enable USART1 Receive and transmit interrupts
	USART_ITConfig(ptUart, USART_IT_RXNE, ENABLE);
	USART_ITConfig(ptUart, USART_IT_TC, ENABLE);
}

void delay_us(uint32_t timeout)
{
	uint8_t  j;
	while(timeout--)
	{
		for(j=0;j<6;j++);
	}
}

void delay_ms(uint32_t timeout)
{
	while(timeout--)
	delay_us(1000);	
}


