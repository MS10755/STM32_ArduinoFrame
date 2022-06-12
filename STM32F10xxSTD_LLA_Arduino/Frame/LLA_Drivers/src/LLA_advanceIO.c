/*
 * @Description: 
 * @Autor: Juan
 * @Date: 2022-04-01 14:49:54
 * @LastEditors: Juan
 * @LastEditTime: 2022-04-01 15:25:18
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
	
}InputCapturePinInfo_t;

typedef struct{
	uint32_t overTimes;
	uint8_t times;
	uint32_t val;
	AdvanceIO_PWC_t pwc;
}CaptureProcessVars_t;

/************************ User user variables ****************************/
//Reference: https://blog.csdn.net/KevinYT/article/details/124669022
static const InputCapturePinInfo_t InputCapturePinInfos[]={
	
	{PA8, TIM1, TIM_Channel_1},
	{PA9, TIM1, TIM_Channel_2},
	{PA10, TIM1, TIM_Channel_3},
	{PA11, TIM1, TIM_Channel_4},
	
	{PC6, TIM8, TIM_Channel_1},
	{PC7, TIM8, TIM_Channel_2},
	{PC8, TIM8, TIM_Channel_3},
	{PC9, TIM8, TIM_Channel_4},
	
	/* Remap pin. */
//	{PE9, TIM1, TIM_Channel_1},
//	{PE11, TIM1, TIM_Channel_2},
//	{PE13, TIM1, TIM_Channel_3},
//	{PE14, TIM1, TIM_Channel_4},
	
};

static volatile CaptureProcessVars_t TIM1CaptureProcessVars[4]={0,};
static volatile CaptureProcessVars_t TIM8CaptureProcessVars[4]={0,};

/************************ User micro define ****************************/
#define PWC_SubPriority								2
#define PWC_PreemptionPriority				0

#define TIM1_CLK 	(SystemCoreClock)
#define TIM8_CLK 	(SystemCoreClock)



#define TIM_CC_Handler_x(TIMx,CCX) do { 																											\
	if(TIM_GetITStatus(TIMx, TIM_IT_CC##CCX) == SET){																						\
		 TIM_ClearITPendingBit(TIMx, TIM_IT_CC##CCX);																							\
		volatile CaptureProcessVars_t * vars = TIMToCaptureProcessVars(TIMx,TIM_Channel_##CCX);		\
		if(vars->times == 0){																																			\
			if(vars->pwc == AdvanceIO_PWC_F2R){																											\
				TIM_ICConfig(TIMx, TIM_Channel_##CCX, TIM_ICPolarity_Rising);													\
			}else if(vars->pwc == AdvanceIO_PWC_R2F){																								\
				TIM_ICConfig(TIMx, TIM_Channel_##CCX, TIM_ICPolarity_Falling);												\
			}																																												\
			vars->times = 1;																																				\
		}else if(vars->times == 1){																																\
			vars->val = vars->overTimes * 1000 + TIM_GetCapture##CCX(TIMx);													\
			vars->times = 2;																																				\
		}																																													\
	}																																														\
}while(0)

/************************ User static function ****************************/
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
			LLA_ASSERT(0,DriverCode_AdvanceIO,AdvanceIO_errorIOOutChipSupport);
			break;
	};
	return NULL;
}

static uint16_t nameToPin(BaseIO_name_t name){
	return (uint16_t) 1 << ( name % 16 );
}

static const InputCapturePinInfo_t * nameToInputCapturePinInfo(BaseIO_name_t name){
	for(uint8_t i=0;i<sizeof(InputCapturePinInfos)/sizeof(InputCapturePinInfos[0]);i++){
		if(InputCapturePinInfos[i].name == name){
			return &InputCapturePinInfos[i];
		}
	}
	return NULL;
}

static void TIM_ICConfig(TIM_TypeDef * TIM_x,uint16_t channel,uint16_t ICPolarity){
	TIM_ICInitTypeDef  TIM_ICInitStructure;
  TIM_ICInitStructure.TIM_Channel = channel;
  TIM_ICInitStructure.TIM_ICPolarity = ICPolarity;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM_x, &TIM_ICInitStructure);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	if(TIM_x == TIM1){
		TIM_TimeBaseStructure.TIM_Prescaler = (TIM1_CLK/1000000)-1; //设置预分频值
	}else if(TIM_x == TIM8){
		TIM_TimeBaseStructure.TIM_Prescaler = (TIM8_CLK/1000000)-1; //设置预分频值
	}
	TIM_TimeBaseStructure.TIM_Period = 1000;//1ms 产生一次溢出
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分频系数
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数
  TIM_TimeBaseInit(TIM_x, &TIM_TimeBaseStructure); //初始化 TIMx
	TIM_ARRPreloadConfig(TIM_x, ENABLE);
}
static volatile CaptureProcessVars_t * TIMToCaptureProcessVars(TIM_TypeDef * TIM_x, uint16_t Channel){
	volatile CaptureProcessVars_t * CaptureProcessVars = NULL;
	if(TIM_x == TIM1){
		CaptureProcessVars = &TIM1CaptureProcessVars[0];
	}else if(TIM_x == TIM8){
		CaptureProcessVars = &TIM8CaptureProcessVars[0];
	}
	
	if(CaptureProcessVars == NULL) return NULL;
	
	if(Channel == TIM_Channel_1){
		CaptureProcessVars+=0;
	}else if(Channel == TIM_Channel_2){
		CaptureProcessVars+=1;
	}else if(Channel == TIM_Channel_3){
		CaptureProcessVars+=2;
	}else if(Channel == TIM_Channel_4){
		CaptureProcessVars+=3;
	}
	return CaptureProcessVars;
}
/************************ LLA common micro define ****************************/


/************************ LLA common variables ****************************/


/************************ LLA common function ****************************/
/**
 * @description: 
 * @param {BaseIO_name_t} dataPin
 * @param {BaseIO_name_t} clockPin
 * @param {LLA_BitOrder_t} bitOrder
 * @return {*}
 * @author: Juan
 */
uint8_t LLA_AdvanceIO_ShiftIn(BaseIO_name_t dataPin, BaseIO_name_t clockPin, LLA_BitOrder_t bitOrder) {
	uint8_t value = 0;
	uint8_t i;

	for (i = 0; i < 8; ++i) {
		LLA_BaseIO_Write(clockPin,BaseIOStatus_HIGH);
		if (bitOrder == LLA_LSBFIRST)
			value |= LLA_BaseIO_ReadInput(dataPin) << i;
		else
			value |= LLA_BaseIO_ReadInput(dataPin) << (7 - i);
		LLA_BaseIO_Write(clockPin,BaseIOStatus_LOW);
	}
	return value;
}

/**
 * @description: 
 * @param {BaseIO_name_t} dataPin
 * @param {BaseIO_name_t} clockPin
 * @param {LLA_BitOrder_t} bitOrder
 * @param {uint8_t} val
 * @return {*}
 * @author: Juan
 */
void LLA_AdvanceIO_ShiftOut(BaseIO_name_t dataPin, BaseIO_name_t clockPin, LLA_BitOrder_t bitOrder, uint8_t val)
{
	uint8_t i;

	for (i = 0; i < 8; i++)  {
		if (bitOrder == LLA_LSBFIRST) {
			LLA_BaseIO_Write(dataPin,(BaseIO_status_t)(val & 1));
			val >>= 1;
		} else {	
			LLA_BaseIO_Write(dataPin,(BaseIO_status_t)((val & 128) != 0));
			val <<= 1;
		}
		LLA_BaseIO_Write(clockPin, BaseIOStatus_HIGH);
		LLA_BaseIO_Write(clockPin, BaseIOStatus_LOW);		
	}
}

/************************ LLA Interface function ****************************/

/**
 * @description: 
 * @param {BaseIO_name_t} name: 
 * @param {AdvanceIO_PWC_t} pwc
 * @param {BaseIO_status_t} idle_status
 * @param {uint32_t} time_out
 * @return {*}
 * @author: Your name
 */
uint32_t LLA_AdvanceIO_PluseMeasure(BaseIO_name_t name,AdvanceIO_PWC_t pwc,BaseIO_status_t idle_status,uint32_t time_out){
	LLA_ASSERT(IS_IO_NAME(name),DriverCode_AdvanceIO, AdvanceIO_errorIOOutChipSupport);
	LLA_ASSERT(pwc<_ADVANCEIO_PWC_MAX,DriverCode_AdvanceIO, AdvanceIO_errorPWC);
	
	/* 1.gpio init */
	const InputCapturePinInfo_t * info = nameToInputCapturePinInfo(name);
	LLA_ASSERT(info, DriverCode_AdvanceIO, AdvanceIO_errorIONotSupportPWC);
	if(info == NULL) return 0;
	GPIO_TypeDef * GPIO_x = nameToPort(name);
	
	if(GPIO_x == GPIOA){
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		
	}else if(GPIO_x == GPIOC){
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		
	}else if(GPIO_x == GPIOE){
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
		
	}else{
		return 0;
	}
	
	GPIO_InitTypeDef GPIO_InitStructure;
	if(idle_status == BaseIOStatus_LOW){
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	}else {
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	}
	GPIO_InitStructure.GPIO_Pin = nameToPin(name);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIO_x, &GPIO_InitStructure);
	
	/* 2.1NVIC config */

  NVIC_InitTypeDef NVIC_InitStructure;
	if(info->TIM_x == TIM1){
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
		
	}else if(info->TIM_x == TIM8){
		#ifdef STM32F10X_HD
		NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
		#endif
		
	}
  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PWC_SubPriority;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = PWC_SubPriority;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* 2.2 NVIC config*/
	
	if(info->TIM_x == TIM1){
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
		
	}else if(info->TIM_x == TIM8){
		#ifdef STM32F10X_HD
		NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;
		#endif
	}
  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PWC_SubPriority;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = PWC_SubPriority;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* 3.TIM config*/
	if(pwc == AdvanceIO_PWC_F2F || pwc == AdvanceIO_PWC_F2R){
		TIM_ICConfig(info->TIM_x,info->Channel_x,TIM_ICPolarity_Falling);
	}else if(pwc == AdvanceIO_PWC_R2R || pwc == AdvanceIO_PWC_R2F){
		TIM_ICConfig(info->TIM_x,info->Channel_x,TIM_ICPolarity_Rising);
	}
	
	if(info->Channel_x == TIM_Channel_1){

		TIM_ITConfig(info->TIM_x, TIM_IT_CC1, ENABLE);
		
	}else if(info->Channel_x == TIM_Channel_2){
		
		TIM_ITConfig(info->TIM_x, TIM_IT_CC2, ENABLE);
		
	}else if(info->Channel_x == TIM_Channel_3){
		
		TIM_ITConfig(info->TIM_x, TIM_IT_CC3, ENABLE);
		
	}else if(info->Channel_x == TIM_Channel_4){
		
		TIM_ITConfig(info->TIM_x, TIM_IT_CC4, ENABLE);
		
	}
	
	volatile CaptureProcessVars_t * CaptureProcessVars = TIMToCaptureProcessVars(info->TIM_x, info->Channel_x);
	
	if(CaptureProcessVars){
		CaptureProcessVars->pwc = pwc;
		CaptureProcessVars->val = 0;
		CaptureProcessVars->times = 0;
	}
	
	
	TIM_ITConfig(info->TIM_x, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(info->TIM_x, ENABLE);
	
	uint32_t start_ms = LLA_SYS_Time_Millis();
	while(CaptureProcessVars->times !=2 && LLA_SYS_Time_ConsumeMillis(start_ms) <time_out){
	}
	uint32_t us = CaptureProcessVars->val;
	CaptureProcessVars->overTimes = 0;
	CaptureProcessVars->pwc = (AdvanceIO_PWC_t)0;
	CaptureProcessVars->val = 0;
	CaptureProcessVars->times = 0;
	
	return us;
}



/************************ Your Interrupt handling function ****************************/

void TIM1_CC_IRQHandler(void)
{ 
	TIM_CC_Handler_x(TIM1,1);
	TIM_CC_Handler_x(TIM1,2);
	TIM_CC_Handler_x(TIM1,3);
	TIM_CC_Handler_x(TIM1,4);
	
}
void TIM8_CC_IRQHandler(void)
{ 
	TIM_CC_Handler_x(TIM8,1);
	TIM_CC_Handler_x(TIM8,2);
	TIM_CC_Handler_x(TIM8,3);
	TIM_CC_Handler_x(TIM8,4);
	
}

void TIM1_UP_IRQHandler(void){
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) == SET){
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
		for(uint8_t i=0;i<4;i++){
			if(TIM1CaptureProcessVars[i].times == 1){
				TIM1CaptureProcessVars[i].overTimes++;
			}
		}
	}
}

void TIM8_UP_IRQHandler(void){
	if(TIM_GetITStatus(TIM8, TIM_IT_Update) == SET){
		TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
		for(uint8_t i=0;i<4;i++){
			if(TIM8CaptureProcessVars[i].times == 1){
				TIM8CaptureProcessVars[i].overTimes++;
			}
		}
	}
}
