/*
 * @Description: 
 * @Autor: Juan
 * @Date: 2022-04-01 14:49:54
 * @LastEditors: Juan
 * @LastEditTime: 2022-04-01 15:32:55
 */
#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "stm32f10x.h"                  // Device header


/************************ LLA common micro define ****************************/

/************************ LLA common variables ****************************/


/************************ LLA common function ****************************/


/************************ LLA Interface function ****************************/

/**
 * @description: 
 * @param {*}
 * @return {*}
 * @author: Your name
 */
void LLA_SYS_clock_Init(void){
	/*
		TODO:
			初始化外部时钟，校准LSI，HSI
	*/
}


/**
 * @description: 
 * @param {*}
 * @return {*}
 * @author: Your name
 */
void LLA_SYS_clock_Update(void){
	SystemCoreClockUpdate();
}
/************************ Your Interrupt handling function ****************************/





