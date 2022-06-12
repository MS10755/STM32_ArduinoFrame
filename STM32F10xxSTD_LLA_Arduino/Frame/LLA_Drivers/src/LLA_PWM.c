/*
 * @Description: 
 * @Autor: Juan
 * @Date: 2022-04-01 14:49:54
 * @LastEditors: Juan
 * @LastEditTime: 2022-04-01 15:31:12
 */
#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "stm32f10x.h"                  // Device header


/************************ User type define ****************************/
typedef struct{
	BaseIO_name_t name;
	TIM_TypeDef * TIM_x;
	uint16_t Channel_x;
}pwmPinInfo_t;


/************************ User user variables ****************************/
static pwmPinInfo_t pwmPinInfos[]={
	{PA0, TIM2, TIM_Channel_1},
	{PA1, TIM2, TIM_Channel_2},
	{PA2, TIM2, TIM_Channel_3},
	{PA3, TIM2, TIM_Channel_4},

	{PA6, TIM3, TIM_Channel_1},
	{PA7, TIM3, TIM_Channel_2},
	{PB0, TIM3, TIM_Channel_3},
	{PB1, TIM3, TIM_Channel_4},
};


/************************ User micro define ****************************/
#define TIM2_CLK  SystemCoreClock
#define TIM3_CLK  SystemCoreClock

/************************ User static function ****************************/
static void _TIMx_Init(TIM_TypeDef * TIM_x,uint16_t arr,uint16_t pre){
	if(TIM_x == TIM2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	}else if(TIM_x == TIM3){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	}
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = pre;
	TIM_TimeBaseInit(TIM_x, &TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM_x, ENABLE);
	TIM_Cmd(TIM_x, ENABLE);
}


static GPIO_TypeDef * nameToPort(BaseIO_name_t name){
	switch(name/16){
		case 0:
			return GPIOA;
		case 1:
			return GPIOB;
		case 2:
			return GPIOC;
		case 3:
			return GPIOD;
		case 4:
			return GPIOE;
		case 5:
			return GPIOF;
		case 6:
			return GPIOG;
		default:
			LLA_ASSERT(0,DriverCode_PWM, PWM_errorIOOutChipSupport);
			break;
	};
	return NULL;
}

static uint16_t nameToPin(BaseIO_name_t name){
	return (uint16_t) 1 << ( name % 16 );
}

static const pwmPinInfo_t * nameToPwmInfo(BaseIO_name_t name){
	for(uint8_t i=0;i<sizeof(pwmPinInfos)/sizeof(pwmPinInfos[0]);i++){
		if(pwmPinInfos[i].name == name){
			return &pwmPinInfos[i];
		}
	}
	return NULL;
}

/************************ LLA common micro define ****************************/

/************************ LLA common variables ****************************/


/************************ LLA common function ****************************/


/************************ LLA Interface function ****************************/


/**
 * @description: 
 * @param {uint32_t} hz
 * @return {*}
 * @author: Your name
 */
void LLA_PWM_BaseHZ(uint32_t hz){
	uint32_t time2_hz = TIM2_CLK / hz;
	uint32_t time3_hz = TIM3_CLK / hz;
	LLA_ASSERT(time2_hz>2 ,DriverCode_PWM, PWM_errorRange);
	LLA_ASSERT(time3_hz>2 ,DriverCode_PWM, PWM_errorRange);
	uint16_t pre2=1;
	
	while(time2_hz/pre2 > 0xFFFF){
		pre2++;
		time2_hz /= pre2;
	}
	
	_TIMx_Init(TIM2,time2_hz -1,pre2 -1);
	
	uint16_t pre3=1;
	
	while(time3_hz/pre3 > 0xFFFF){
		pre3++;
		time3_hz /= pre3;
	}

	_TIMx_Init(TIM3,time3_hz -1, pre3 -1);
}

/**
 * @description: 
 * @param {BaseIO_name_t} name
 * @param {uint8_t} duty
 * @param {PWM_Polar_t} polar
 * @return {*}
 * @author: Your name
 */
void LLA_PWM_Output1(BaseIO_name_t name,uint8_t duty,PWM_Polar_t polar){
	LLA_ASSERT(polar<_PWM_POLAR_MAX, DriverCode_PWM , PWM_errorPolar);
	GPIO_TypeDef * GPIO_x = nameToPort(name);
	if(GPIO_x == NULL) return;
	if(GPIO_x == GPIOA){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	}else if(GPIO_x == GPIOB){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	}
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = nameToPin(name);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_x , &GPIO_InitStructure);
	
  TIM_OCInitTypeDef TIM_OCInitStructure;
	
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = polar==PWM_Polar_HIGH? TIM_OCPolarity_High : TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	const pwmPinInfo_t * info = nameToPwmInfo(name);
	LLA_ASSERT(info, DriverCode_PWM, PWM_errorIONotSupportPWM);
	
	switch(info->Channel_x) {
		case TIM_Channel_1:
				TIM_OC1Init(info->TIM_x,&TIM_OCInitStructure);
				TIM_OC1PreloadConfig(info->TIM_x, TIM_OCPreload_Enable);
				break;
		case TIM_Channel_2:
				TIM_OC2Init(info->TIM_x,&TIM_OCInitStructure);
				TIM_OC2PreloadConfig(info->TIM_x, TIM_OCPreload_Enable);
				break;
		case TIM_Channel_3:
				TIM_OC3Init(info->TIM_x,&TIM_OCInitStructure);
				TIM_OC3PreloadConfig(info->TIM_x, TIM_OCPreload_Enable);
				break;
		case TIM_Channel_4:
				TIM_OC4Init(info->TIM_x,&TIM_OCInitStructure);
				TIM_OC4PreloadConfig(info->TIM_x, TIM_OCPreload_Enable);
				break;
	}
	TIM_CtrlPWMOutputs(info->TIM_x, ENABLE);
	
	uint16_t val = ((float)duty/256.0f)*info->TIM_x->ARR;
	switch(info->Channel_x) {
	case TIM_Channel_1:
			TIM_SetCompare1(info->TIM_x,val);
			break;
	case TIM_Channel_2:
			TIM_SetCompare2(info->TIM_x,val);
			break;
	case TIM_Channel_3:
			TIM_SetCompare3(info->TIM_x,val);
			break;
	case TIM_Channel_4:
			TIM_SetCompare4(info->TIM_x,val);
			break;
	}
}

/**
 * @description: 
 * @param {BaseIO_name_t} name
 * @param {uint32_t} hz
 * @param {uint8_t} duty
 * @param {PWM_Polar_t} polar
 * @return {*}
 * @author: Your name
 */
void LLA_PWM_OutPut2(BaseIO_name_t name,uint32_t hz,uint8_t duty,PWM_Polar_t polar){
	LLA_ASSERT(polar<_PWM_POLAR_MAX, DriverCode_PWM , PWM_errorPolar);
	const pwmPinInfo_t * info = nameToPwmInfo(name);
	LLA_ASSERT(info, DriverCode_PWM, PWM_errorIONotSupportPWM);
	
	LLA_PWM_BaseHZ(hz);
	LLA_PWM_Output1(name,duty,polar);
}

/**
 * @description: 
 * @param {BaseIO_name_t} name
 * @return {*}
 * @author: Your name
 */
void LLA_PWM_Stop(BaseIO_name_t name){
	GPIO_TypeDef * GPIO_x = nameToPort(name);
	if(GPIO_x == NULL) return;
	if(GPIO_x == GPIOA){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	}else if(GPIO_x == GPIOB){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	}
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = nameToPin(name);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_x , &GPIO_InitStructure);
}

/**
 * @description: 
 * @param {BaseIO_name_t} name
 * @return {*}
 * @author: Your name
 */
void LLA_PWM_Resume(BaseIO_name_t name){
	GPIO_TypeDef * GPIO_x = nameToPort(name);
	if(GPIO_x == NULL) return;
	if(GPIO_x == GPIOA){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	}else if(GPIO_x == GPIOB){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	}
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = nameToPin(name);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_x , &GPIO_InitStructure);
}

/************************ Your Interrupt handling function ****************************/


