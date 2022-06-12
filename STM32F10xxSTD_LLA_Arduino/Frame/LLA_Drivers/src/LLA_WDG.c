/*
 * @Description: 
 * @Autor: Juan
 * @Date: 2022-04-01 14:49:54
 * @LastEditors: Juan
 * @LastEditTime: 2022-04-01 15:37:14
 */
#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "stm32f10x.h"                  // Device header


/************************ User type define ****************************/


/************************ User user variables ****************************/
static float overTime = 0;

/************************ User micro define ****************************/
#define IWDG_CLK	40000

/************************ User static function ****************************/


/************************ LLA common micro define ****************************/

/************************ LLA common variables ****************************/


/************************ LLA common function ****************************/


/************************ LLA Interface function ****************************/

/**
 * @description: 
 * @param {uint16_t} ms
 * @return {*}
 * @author: Your name
 */
void lLA_WDG_Start(uint16_t ms){
	uint16_t div,reload_val;
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//写访问
	if(ms < 300){
		//使用4分频
		IWDG_SetPrescaler(IWDG_Prescaler_4);
		div = 4;
	}else{
		IWDG_SetPrescaler(IWDG_Prescaler_256);
		div = 256;
	}
	reload_val = ms/1000/(1.0f/(IWDG_CLK/div));
	IWDG_SetReload(reload_val);
	IWDG_ReloadCounter();
	IWDG_Enable();
	overTime = (float)reload_val/(float)(IWDG_CLK/div) *1000;
}


/**
 * @description: 
 * @param {*}
 * @return {*}
 * @author: Your name
 */
float LLA_WDG_GetOVRTime(){

	return overTime;
}

/**
 * @description: 
 * @param {*}
 * @return {*}
 * @author: Your name
 */
void LLA_WDG_Feed(){
	IWDG_ReloadCounter();
}


/**
 * @description: 
 * @param {*}
 * @return {*}
 * @author: Your name
 */
void LLA_WDG_Stop(){
	LLA_ASSERT(0, DriverCode_WDG, WDG_errorStop);//STM32的看门狗一旦打开就不能关闭
	
}

/************************ Your Interrupt handling function ****************************/


