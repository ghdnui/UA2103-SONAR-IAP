#ifndef _STM32_CONFIG_H
#define _STM32_CONFIG_H


void RCC_Configuration(void);
void NVIC_Configuration(void);
void SysTick_Configuration(void);
void GPIO_Configuration(void);
void TIMER_Configuration(void);
void TIMER1_Configuration(void);
void UART_Configuration(USART_TypeDef *ptUart,uint32_t BaudRate);
void SPI1_Configuration(void);
void delay_us(uint32_t timeout);
void delay_ms(uint32_t timeout);

#endif
