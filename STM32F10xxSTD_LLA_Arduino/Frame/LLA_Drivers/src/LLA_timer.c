/*
 * @Description: 
 * @Autor: Juan
 * @Date: 2022-04-01 14:49:54
 * @LastEditors: Juan
 * @LastEditTime: 2022-04-01 15:35:22
 */
#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "stm32f10x.h"                  // Device header


/************************ User type define ****************************/
typedef struct{
	LLA_Timer_t timer;
	TIM_TypeDef * TIM_x;
	void (*RCC_function)(uint32_t periph,FunctionalState state);
	uint32_t periph;
	uint8_t IRQ_n;
}timerInfot_t;

/************************ User user variables ****************************/
static const timerInfot_t timerInfots[]={
	{LLA_TIMER0,TIM4, RCC_APB1PeriphClockCmd, RCC_APB1Periph_TIM4, TIM4_IRQn},
#ifdef STM32F10X_HD
	{LLA_TIMER1,TIM5, RCC_APB1PeriphClockCmd, RCC_APB1Periph_TIM5, TIM5_IRQn},
	{LLA_TIMER2,TIM6, RCC_APB1PeriphClockCmd, RCC_APB1Periph_TIM6, TIM6_IRQn},
	{LLA_TIMER3,TIM7, RCC_APB1PeriphClockCmd, RCC_APB1Periph_TIM7, TIM7_IRQn},
#endif
};

static Timer_Handler_t Timer_Handler[4]={NULL,};
static volatile uint32_t timerReCounterTarget[4]={0};
static volatile uint32_t timerReCounter[4]={0};
/************************ User micro define ****************************/
#define TIMER_PreemptionPriority				3
#define TIMER_SubPriority								0


#define TIMx_CLK (SystemCoreClock)

#define TIMx_IRQhandelr(x) do {															 		\
	if(TIM_GetITStatus(TIM##x, TIM_IT_Update) == SET){						\
		TIM_ClearITPendingBit(TIM##x, TIM_IT_Update);								\
		if(timerReCounter[x-4] < timerReCounterTarget[x-4]){				\
			timerReCounter[x-4]++;																		\
		}else{																											\
			timerReCounter[x-4] = 0;																	\
			if(Timer_Handler[x-4]){																		\
				Timer_Handler[x-4]();																		\
			}																													\
		}																														\
	}																															\
}while(0)

/************************ User static function ****************************/
static const timerInfot_t * timerToInfo(LLA_Timer_t timer){
	for(uint8_t i=0;i<sizeof(timerInfots)/sizeof(timerInfots[0]);i++){
		if(timerInfots[i].timer == timer){
			return &timerInfots[i];
		}
	}
return NULL;
}

/************************ LLA common micro define ****************************/
#define IS_LLA_TIMER(x) 	(x<_LLA_TIMER_MAX)

/************************ LLA common variables ****************************/
//static Timer_Handler_t Timer_Handler_List[_LLA_TIMER_MAX]={0};

/************************ LLA common function ****************************/


/************************ LLA Interface function ****************************/

/**
 * @description: 
 * @param {LLA_Timer_t} timer
 * @param {uint32_t} period
 * @param {Timer_Handler_t} handler
 * @return {*}
 * @author: Your name
 */
void LLA_Timer_Set(LLA_Timer_t timer,uint32_t period,Timer_Handler_t handler){
	LLA_ASSERT(timer<=LLA_TIMER3,DriverCode_Timer, TIMER_errorTimer);
	LLA_ASSERT(handler, DriverCode_Timer, TIMER_errorCallback);
	
	const timerInfot_t * info = timerToInfo(timer);
	LLA_ASSERT(info,DriverCode_Timer, TIMER_errorTimerOutChipSupport);
	if(info == NULL) return;
	
	/* 1. rcc enable*/
	info->RCC_function(info->periph, ENABLE);
	
	/* 2. time base init */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 999;
	TIM_TimeBaseStructure.TIM_Prescaler = TIMx_CLK/1000000 -1;
	TIM_TimeBaseInit(info->TIM_x, &TIM_TimeBaseStructure);
	
	/* 3.NVIC init */
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = info->IRQ_n;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIMER_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIMER_SubPriority;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearITPendingBit(info->TIM_x, TIM_IT_Update);
	TIM_ITConfig(info->TIM_x, TIM_IT_Update, ENABLE);
	
	timerReCounterTarget[info->timer] = period;
	Timer_Handler[info->timer] = handler;
	TIM_ARRPreloadConfig(info->TIM_x, ENABLE);
}


/**
 * @description: 
 * @param {LLA_Timer_t} timer
 * @return {*}
 * @author: Your name
 */
void LLA_Timer_Start(LLA_Timer_t timer){
	const timerInfot_t * info = timerToInfo(timer);
	LLA_ASSERT(info,DriverCode_Timer, TIMER_errorTimerOutChipSupport);
	if(info == NULL) return;
	TIM_Cmd(info->TIM_x, ENABLE);
}


/**
 * @description: 
 * @param {LLA_Timer_t} timer
 * @return {*}
 * @author: Your name
 */
void LLA_Timer_Stop(LLA_Timer_t timer){
	const timerInfot_t * info = timerToInfo(timer);
	LLA_ASSERT(info,DriverCode_Timer, TIMER_errorTimerOutChipSupport);
	if(info == NULL) return;
	TIM_Cmd(info->TIM_x, DISABLE);
}


/************************ Your Interrupt handling function ****************************/

void TIM4_IRQHandler(){
	TIMx_IRQhandelr(4);
}

void TIM5_IRQHandler(){
	TIMx_IRQhandelr(5);
}

void TIM6_IRQHandler(){
	TIMx_IRQhandelr(6);
}

void TIM7_IRQHandler(){
	TIMx_IRQhandelr(7);
}

