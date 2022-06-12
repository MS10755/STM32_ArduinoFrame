/*
 * @Description: 
 * @Autor: Juan
 * @Date: 2022-04-01 14:49:54
 * @LastEditors: Juan
 * @LastEditTime: 2022-04-01 15:29:16
 */
#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "stm32f10x.h"                  // Device header

#define EXTI_SubPriority							2
#define EXTI_PreemptionPriority				0

/************************ LLA common micro define ****************************/
#define IS_EXTI_CB(x) 	(!!x)
#define IS_EXTI_TYPE(x)	(x<_LLA_EXTI_TYPE_MAX)


#define EXTI_x_IRQHandler(x)																						\
do{																																			\
	if(EXTI_GetITStatus(EXTI_Line##x)==SET){															\
		if(LLA_EXTI_cb_list[x]) LLA_EXTI_cb_list[x]();											\
		EXTI_ClearITPendingBit(EXTI_Line##x);																\
	}																																			\
}while(0)

/************************ LLA common variables ****************************/
static LLA_EXTI_cb_t LLA_EXTI_cb_list[16]={0};

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
			LLA_ASSERT(0,DriverCode_EXTI, EXTI_errorIOOutChipSupport);
			break;
	};
	return NULL;
}

static uint16_t nameToPin(BaseIO_name_t name){
	return (uint16_t) 1 << ( name % 16 );
}

static uint8_t nameToIRQn(BaseIO_name_t name){
	IRQn_Type EXTI_x_IRQ;
	uint16_t _pin = name%16;
	if(_pin==0)										EXTI_x_IRQ = EXTI0_IRQn;
	else if(_pin==1)							EXTI_x_IRQ = EXTI1_IRQn;
	else if(_pin==2)							EXTI_x_IRQ = EXTI2_IRQn;
	else if(_pin==3)							EXTI_x_IRQ = EXTI3_IRQn;
	else if(_pin==4)							EXTI_x_IRQ = EXTI4_IRQn;
	else if(_pin>=5 && _pin<=9)		EXTI_x_IRQ = EXTI9_5_IRQn;
	else if(_pin>=10 && _pin<=15)	EXTI_x_IRQ = EXTI15_10_IRQn;
	else return 0xFF;
	return EXTI_x_IRQ;
}

/************************ LLA common function ****************************/

/************************ User function ****************************/



/************************ LLA Interface function ****************************/

/**
 * @description: 
 * @param {BaseIO_name_t} name
 * @param {LLA_EXTI_cb_t} cb
 * @param {LLA_EXTI_TYPE_t} type
 * @return {*}
 * @author: Your name
 */
void LLA_EXTI_AttachInterrupt(BaseIO_name_t name, LLA_EXTI_cb_t cb,LLA_EXTI_TYPE_t type){
	LLA_ASSERT(IS_IO_NAME(name), DriverCode_EXTI, EXTI_errorIOName);
	LLA_ASSERT(IS_EXTI_CB(cb), 	DriverCode_EXTI,  EXTI_errorCallback);
	LLA_ASSERT(IS_EXTI_TYPE(type), DriverCode_EXTI, EXTI_errorType);
	LLA_ASSERT(!(type == LLA_EXTI_TYPE_LOW || type == LLA_EXTI_TYPE_HIGH),DriverCode_EXTI, EXTI_errorTypeOutChipSupport);
	
	GPIO_TypeDef * GPIO_x = nameToPort(name);
	LLA_ASSERT(GPIO_x, DriverCode_EXTI, EXTI_errorIOOutChipSupport);
	if(GPIO_x == NULL) return;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	if(GPIO_x==GPIOA)				GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,name%16);
	else if(GPIO_x==GPIOB)	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,name%16);
	else if(GPIO_x==GPIOC)	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,name%16);
	else if(GPIO_x==GPIOD)	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,name%16);
	else if(GPIO_x==GPIOE)	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,name%16);
	else if(GPIO_x==GPIOF)	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,name%16);
	else if(GPIO_x==GPIOG)	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,name%16);
	else return;
	
	EXTI_InitTypeDef EXTI_InitStructure;

	EXTI_InitStructure.EXTI_Line = nameToPin(name);
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	if(type==LLA_EXTI_TYPE_RISING)				EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	else if(type==LLA_EXTI_TYPE_FALLING)	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	else if(type==LLA_ExTI_TYPE_CHANGE)	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	else return;
	
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = nameToIRQn(name);
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = EXTI_SubPriority;
	NVIC_Init(&NVIC_InitStructure);
	
	LLA_EXTI_cb_list[name % 16] = cb;
}

/**
 * @description: 
 * @param {BaseIO_name_t} name
 * @return {*}
 * @author: Your name
 */
void LLA_EXTI_DetachInterrupt(BaseIO_name_t name){
	LLA_ASSERT(IS_IO_NAME(name), DriverCode_EXTI, EXTI_errorIOName);
	LLA_EXTI_cb_list[name % 16] = NULL;
}

/************************ Your Interrupt handling function ****************************/


/**
  * @brief	:	外部中断0入口
  * @param	: 
  * @param	: 
  * @retval	:	
  */
void EXTI0_IRQHandler(void){
	EXTI_x_IRQHandler(0);
}

/**
  * @brief	:	外部中断1入口
  * @param	: 
  * @param	: 
  * @retval	:	
  */
void EXTI1_IRQHandler(void){
	EXTI_x_IRQHandler(1);
}

/**
  * @brief	:	外部中断2入口
  * @param	: 
  * @param	: 
  * @retval	:	
  */
void EXTI2_IRQHandler(void){
	EXTI_x_IRQHandler(2);
}


/**
  * @brief	:	外部中断3入口
  * @param	: 
  * @param	: 
  * @retval	:	
  */
void EXTI3_IRQHandler(void){
	EXTI_x_IRQHandler(3);
}


/**
  * @brief	:	外部中断4入口
  * @param	: 
  * @param	: 
  * @retval	:	
  */
void EXTI4_IRQHandler(void){
	EXTI_x_IRQHandler(4);
}


/**
  * @brief	:	外部中断5~9入口
  * @param	: 
  * @param	: 
  * @retval	:	
  */
void EXTI9_5_IRQHandler(void)
{
	EXTI_x_IRQHandler(5);
	EXTI_x_IRQHandler(6);
	EXTI_x_IRQHandler(7);
	EXTI_x_IRQHandler(8);
	EXTI_x_IRQHandler(9);
}


/**
  * @brief	:	外部中断10~15入口
  * @param	: 
  * @param	: 
  * @retval	:	
  */
void EXTI15_10_IRQHandler(void)
{
	EXTI_x_IRQHandler(10);
	EXTI_x_IRQHandler(11);
	EXTI_x_IRQHandler(12);
	EXTI_x_IRQHandler(13);
	EXTI_x_IRQHandler(14);
	EXTI_x_IRQHandler(15);
}
