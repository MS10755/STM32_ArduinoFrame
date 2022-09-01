/*
 * @Description: 
 * @Autor: Juan
 * @Date: 2022-04-01 14:49:54
 * @LastEditors: Juan
 * @LastEditTime: 2022-04-01 15:36:32
 */
#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "stm32f10x.h"                  // Device header
#if LLA_OS_SUPPORT
#include "FreeRTOS.h"
#include "task.h"
#endif

#define UART_PreemptionPriority		1
#define UART_SubPriority					0

/************************ User type define ****************************/

/************************ User user variables ****************************/


/************************ LLA common micro define ****************************/
#define IS_UART_NAME(x) 	(x<_LLA_UART_MAX)
#define IS_UART_CONFIG(x) (x<_UART_CONCIG_MAX)

/************************ LLA common variables ****************************/
int LLA_UART_Printf(UART_name_t name,const char *__restrict __format, ...){
	LLA_ASSERT(IS_UART_NAME(name),DriverCode_UART,UART_errorName);
	
	static char printf_buff[LLA_UART_PRINTF_BUFFER_LENGTH];//此处使用静态的原因是防止用户不熟悉框架底层而设的						\
																									栈区太小，导致内存分配失败进入HardFault（略微浪费内存）
	va_list args;
	va_start(args, __format);
	int ret_status = vsnprintf(printf_buff, sizeof(printf_buff), __format, args);
	va_end(args);
	LLA_UART_WriteBuffer(name,(uint8_t*)printf_buff,ret_status);
	return ret_status;

}



/************************ LLA common function ****************************/
LLA_WEAK void LLA_UART1_IRQHandler(uint8_t data){};
LLA_WEAK void LLA_UART2_IRQHandler(uint8_t data){};
LLA_WEAK void LLA_UART3_IRQHandler(uint8_t data){};
LLA_WEAK void LLA_UART4_IRQHandler(uint8_t data){};
LLA_WEAK void LLA_UART5_IRQHandler(uint8_t data){};
LLA_WEAK void LLA_UART6_IRQHandler(uint8_t data){};
LLA_WEAK void LLA_UART7_IRQHandler(uint8_t data){};
LLA_WEAK void LLA_UART8_IRQHandler(uint8_t data){};

/************************ LLA Interface function ****************************/


/**
 * @description: 
 * @param {UART_name_t} name
 * @param {uint32_t} baudRate
 * @param {UART_config_t} config
 * @return {*}
 * @author: Your name
 */
void LLA_UART_Init(UART_name_t name,uint32_t baudRate,UART_config_t config){
	LLA_ASSERT(name<=LLA_UART5, DriverCode_UART, UART_errorOutChipSupport);
	LLA_ASSERT(IS_UART_CONFIG(config), DriverCode_UART, UART_errorConfig);
	
	/* 1. GPIO */
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if(name == LLA_UART1){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		
		GPIO_Init(GPIOA, &GPIO_InitStructure);// tx pin
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		
		GPIO_Init(GPIOA, &GPIO_InitStructure);// rx pin
				
	}else	if(name == LLA_UART2){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		
		GPIO_Init(GPIOA, &GPIO_InitStructure);// tx pin
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		
		GPIO_Init(GPIOA, &GPIO_InitStructure);// rx pin
		
	}else	if(name == LLA_UART3){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		
		GPIO_Init(GPIOB, &GPIO_InitStructure);// tx pin
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		
		GPIO_Init(GPIOB, &GPIO_InitStructure);// rx pin
		
	}else	if(name == LLA_UART4){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		
		GPIO_Init(GPIOC, &GPIO_InitStructure);// tx pin
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		
		GPIO_Init(GPIOC, &GPIO_InitStructure);// rx pin
	}else	if(name == LLA_UART5){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		
		GPIO_Init(GPIOC, &GPIO_InitStructure);// tx pin
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		
		GPIO_Init(GPIOD, &GPIO_InitStructure);// rx pin
	}
	
	/*  */
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = baudRate;
	if(config == UART_CONFIG_8N1 || config == UART_CONFIG_8N1_5 || config == UART_CONFIG_8N2){
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	}else{
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	}
	
	if(config == UART_CONFIG_8E1 || config == UART_CONFIG_8E1_5 || config == UART_CONFIG_8E2){
		USART_InitStructure.USART_Parity = USART_Parity_Even;
	}else if(config == UART_CONFIG_8O1 || config == UART_CONFIG_8O1_5 || config == UART_CONFIG_8O2){
		USART_InitStructure.USART_Parity = USART_Parity_Odd;
	}else if(config == UART_CONFIG_8N1 || config == UART_CONFIG_8N1_5 || config == UART_CONFIG_8N2){
		USART_InitStructure.USART_Parity = USART_Parity_No;
	}
	
	if(config == UART_CONFIG_8O1 || config == UART_CONFIG_8N1 || config == UART_CONFIG_8E1 ){
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
	}else if(config == UART_CONFIG_8O1_5 || config == UART_CONFIG_8N1_5 || config == UART_CONFIG_8E1_5 ){
		USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
	}else if(config == UART_CONFIG_8O2 || config == UART_CONFIG_8N2 || config == UART_CONFIG_8E2 ){
		USART_InitStructure.USART_StopBits = USART_StopBits_2;
	}
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	uint8_t USARTx_IRQn;
	if(name == LLA_UART1){
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 ,ENABLE);
		USART_Init(USART1, &USART_InitStructure);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		USART_Cmd(USART1, ENABLE);
		USARTx_IRQn = USART1_IRQn;
	}else if(name == LLA_UART2){
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 ,ENABLE);
		USART_Init(USART2, &USART_InitStructure);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		USART_Cmd(USART2, ENABLE);
		USARTx_IRQn = USART2_IRQn;
	}else if(name == LLA_UART3){
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 ,ENABLE);
		USART_Init(USART3, &USART_InitStructure);
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		USART_Cmd(USART3, ENABLE);
		USARTx_IRQn = USART3_IRQn;
	}else if(name == LLA_UART4){
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4 ,ENABLE);
		USART_Init(UART4, &USART_InitStructure);
		USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
		USART_Cmd(UART4, ENABLE);
		#ifdef STM32F10X_HD
		USARTx_IRQn = UART4_IRQn;
		#endif
	}else if(name == LLA_UART5){
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5 ,ENABLE);
		USART_Init(UART5, &USART_InitStructure);
		USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
		USART_Cmd(UART5, ENABLE);
		#ifdef STM32F10X_HD
		USARTx_IRQn = UART5_IRQn;
		#endif
	}
	
  NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART_PreemptionPriority;//抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = UART_SubPriority;        //子优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
  NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器
	
}

/**
 * @description: 
 * @param {UART_name_t} name
 * @return {*}
 * @author: Your name
 */
void LLA_UART_DeInit(UART_name_t name){
	LLA_ASSERT(name<=LLA_UART5, DriverCode_UART, UART_errorOutChipSupport);
	
	if( name == LLA_UART1){
		
		USART_Cmd(USART1, DISABLE);
		
	}else if( name == LLA_UART2){
		
		USART_Cmd(USART2, DISABLE);
		
	}else if( name == LLA_UART3){
		
		USART_Cmd(USART3, DISABLE);
		
	}else if( name == LLA_UART4){
		
		USART_Cmd(UART4, DISABLE);
		
	}else if( name == LLA_UART5){
		
		USART_Cmd(UART5, DISABLE);
		
	}
}


/**
 * @description: 
 * @param {UART_name_t} name
 * @param {uint8_t} data
 * @return {*}
 * @author: Your name
 */
void LLA_UART_Write(UART_name_t name,uint8_t data){
	LLA_ASSERT(name<=LLA_UART5, DriverCode_UART, UART_errorOutChipSupport);
	
	USART_TypeDef * USART_x;
	
	if( name == LLA_UART1){
		
		USART_x = USART1;
		
	}else if( name == LLA_UART2){
		
		USART_x = USART2;
		
	}else if( name == LLA_UART3){
		
		USART_x = USART3;
		
	}else if( name == LLA_UART4){
		
		USART_x = UART4;
		
	}else if( name == LLA_UART5){
		
		USART_x = UART5;
		
	}
	
	while(!USART_GetFlagStatus(USART_x, USART_FLAG_TXE)){};
	USART_SendData(USART_x, data);
	while(!USART_GetFlagStatus(USART_x, USART_FLAG_TC)){};
	USART_GetFlagStatus(USART_x,USART_FLAG_TC);//读SR,下次写DR时清除TC

}


/**
 * @description: 
 * @param {UART_name_t} name
 * @param {uint8_t} *data
 * @param {uint16_t} len
 * @return {*}
 * @author: Your name
 */
void LLA_UART_WriteBuffer(UART_name_t name,uint8_t *data,uint16_t len){
	LLA_ASSERT(name<=LLA_UART5, DriverCode_UART, UART_errorOutChipSupport);
	
	while(len--){
		LLA_UART_Write(name, *data++);
	}
}

/************************ Your Interrupt handling function ****************************/
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
#if LLA_OS_SUPPORT
		uint32_t status = taskENTER_CRITICAL_FROM_ISR();
#endif
		LLA_UART1_IRQHandler(USART1->DR);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
#if LLA_OS_SUPPORT
		taskEXIT_CRITICAL_FROM_ISR(status);
#endif
	}
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){
#if LLA_OS_SUPPORT
		uint32_t status = taskENTER_CRITICAL_FROM_ISR();
#endif
		LLA_UART2_IRQHandler(USART2->DR);
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
#if LLA_OS_SUPPORT
		taskEXIT_CRITICAL_FROM_ISR(status);
#endif
	}
}

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){
#if LLA_OS_SUPPORT
		uint32_t status = taskENTER_CRITICAL_FROM_ISR();
#endif
		LLA_UART3_IRQHandler(USART3->DR);
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
#if LLA_OS_SUPPORT
		taskEXIT_CRITICAL_FROM_ISR(status);
#endif
	}
}

void UART4_IRQHandler(void)
{
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET){
#if LLA_OS_SUPPORT
		uint32_t status = taskENTER_CRITICAL_FROM_ISR();
#endif
		LLA_UART4_IRQHandler(UART4->DR);
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
#if LLA_OS_SUPPORT
		taskEXIT_CRITICAL_FROM_ISR(status);
#endif
	}
}

void UART5_IRQHandler(void)
{
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET){
#if LLA_OS_SUPPORT
		uint32_t status = taskENTER_CRITICAL_FROM_ISR();
#endif
		LLA_UART5_IRQHandler(UART5->DR);
		USART_ClearITPendingBit(UART5, USART_IT_RXNE);
#if LLA_OS_SUPPORT
		taskEXIT_CRITICAL_FROM_ISR(status);
#endif
	}
}

