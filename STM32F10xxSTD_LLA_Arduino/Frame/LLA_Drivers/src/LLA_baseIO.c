/*
 * @Description: 
 * @Autor: Juan
 * @Date: 2022-04-01 14:49:54
 * @LastEditors: Juan
 * @LastEditTime: 2022-04-01 15:26:50
 */
#include "LLA_Drivers.h"


/* Chip SDK headers includes */
#include "stm32f10x.h"                  // Device header



/* Your static function must be declare in here. */
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
			LLA_ASSERT(0,DriverCode_BaseIO,BaseIO_errorIOOutChipSupport);
			break;
	};
	return NULL;
}

static uint16_t nameToPin(BaseIO_name_t name){
	return (uint16_t) 1 << ( name % 16 );
}

/* Your static function END. */


/**
 * @description: 
 * @param {BaseIO_name_t} name
 * @param {BaseIO_mode_t} mode
 * @param {BaseIO_status_t} outputStatus
 * @return {*}
 * @author: Your name
 */
void LLA_BaseIO_Mode(BaseIO_name_t name,BaseIO_mode_t mode,BaseIO_status_t outputStatus){
	LLA_ASSERT(IS_IO_NAME(name), DriverCode_BaseIO, BaseIO_errorIOName);
	LLA_ASSERT(IS_IO_MODE(mode), DriverCode_BaseIO, BaseIO_errorIOMode);
	LLA_ASSERT(IS_IO_STATUS(outputStatus), DriverCode_BaseIO, BaseIO_errorIOStatus);
	
	GPIO_TypeDef * GPIOx = nameToPort(name);
	LLA_ASSERT(GPIOx ,DriverCode_BaseIO,BaseIO_errorIOOutChipSupport);
	
	/* 1.enable clock */
	if(GPIOx == GPIOA){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	}else if(GPIOx == GPIOB){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	}else if(GPIOx == GPIOC){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	}else if(GPIOx == GPIOD){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	}else if(GPIOx == GPIOE){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	}else if(GPIOx == GPIOF){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	}else if(GPIOx == GPIOG){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	}
	
	/* 2.set gpio mode */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = nameToPin(name);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	
	if(BaseIOMode_INPUT == mode){
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	}else if(BaseIOMode_INPUT_PU == mode){
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	}else if(BaseIOMode_INPUT_PD == mode){
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	}else if(BaseIOMode_OUTPUT == mode){
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	}else if(BaseIOMode_OUTPUT_OD == mode){
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	}else{
		LLA_ASSERT(0, DriverCode_BaseIO, BaseIO_errorIOMode);
	}
	
	GPIO_Init(GPIOx,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOx, nameToPin(name), (BitAction)outputStatus);
}


/**
 * @description: 
 * @param {BaseIO_name_t} name
 * @param {BaseIO_status_t} outputStatus
 * @return {*}
 * @author: Your name
 */
void LLA_BaseIO_Write(BaseIO_name_t name,BaseIO_status_t outputStatus){
	LLA_ASSERT(IS_IO_NAME(name), DriverCode_BaseIO, BaseIO_errorIOName);
	LLA_ASSERT(IS_IO_STATUS(outputStatus), DriverCode_BaseIO, BaseIO_errorIOStatus);
	
	GPIO_TypeDef * GPIOx = nameToPort(name);
	LLA_ASSERT(GPIOx ,DriverCode_BaseIO,BaseIO_errorIOOutChipSupport);
	
	GPIO_WriteBit(GPIOx, nameToPin(name), (BitAction)outputStatus);
}

/**
 * @description: 
 * @param {BaseIO_name_t} name
 * @return {*}
 * @author: Your name
 */
BaseIO_status_t LLA_BaseIO_ReadInput(BaseIO_name_t name){
	LLA_ASSERT(IS_IO_NAME(name), DriverCode_BaseIO, BaseIO_errorIOName);
	
	GPIO_TypeDef * GPIOx = nameToPort(name);
	LLA_ASSERT(GPIOx ,DriverCode_BaseIO,BaseIO_errorIOOutChipSupport);
	
	return (BaseIO_status_t)GPIO_ReadInputDataBit(GPIOx, nameToPin(name));
}


/**
 * @description: 
 * @param {BaseIO_name_t} name
 * @return {*}
 * @author: Your name
 */
BaseIO_status_t LLA_BaseIO_ReadOutput(BaseIO_name_t name){
	LLA_ASSERT(IS_IO_NAME(name), DriverCode_BaseIO, BaseIO_errorIOName);
	
	GPIO_TypeDef * GPIOx = nameToPort(name);
	LLA_ASSERT(GPIOx ,DriverCode_BaseIO,BaseIO_errorIOOutChipSupport);
	
	return (BaseIO_status_t)GPIO_ReadOutputDataBit(GPIOx, nameToPin(name));
}


/**
 * @description: 
 * @param {BaseIO_name_t} name
 * @return {*}
 * @author: Your name
 */
BaseIO_status_t LLA_BaseIO_Toggle(BaseIO_name_t name){
	LLA_ASSERT(IS_IO_NAME(name), DriverCode_BaseIO, BaseIO_errorIOName);
	
	GPIO_TypeDef * GPIOx = nameToPort(name);
	LLA_ASSERT(GPIOx ,DriverCode_BaseIO,BaseIO_errorIOOutChipSupport);
	
	GPIO_WriteBit(GPIOx, nameToPin(name) , (BitAction)!GPIO_ReadOutputDataBit(GPIOx, nameToPin(name)));
	
	return (BaseIO_status_t)GPIO_ReadOutputDataBit(GPIOx, nameToPin(name));
}

