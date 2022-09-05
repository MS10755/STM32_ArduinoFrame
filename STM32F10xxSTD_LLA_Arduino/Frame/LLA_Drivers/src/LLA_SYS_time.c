#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "stm32f10x.h"                  // Device header
#if LLA_OS_SUPPORT
#include "FreeRTOS.h"
#include "task.h"
#endif

/************************ LLA Interface micro define ****************************/
#define LLA_SYS_TIME_CLK SystemCoreClock


/************************ LLA common micro define ****************************/
#define SysTick_LoadValue (LLA_SYS_TIME_CLK / 1000U)
#define CYCLES_PER_MICROSECOND (LLA_SYS_TIME_CLK/1000000U)



/************************ LLA common variables ****************************/
volatile static uint32_t System_ms = 0;



/************************ LLA common function ****************************/

/**
 * @description: 
 * @param {uint32_t} pre_ms
 * @return {*}
 * @author: Your name
 */
uint32_t LLA_SYS_Time_ConsumeMillis(uint32_t pre_ms){
	uint32_t current_ms = LLA_SYS_Time_Millis();
	return current_ms>=pre_ms? (current_ms-pre_ms): (UINT32_MAX+current_ms-pre_ms);
}

/**
 * @description: 
 * @param {uint32_t} pre_us
 * @return {*}
 * @author: Your name
 */
uint32_t LLA_SYS_Time_ConsumeMicros(uint32_t pre_us){
	uint32_t current_us = LLA_SYS_Time_Micros();
	return current_us>=pre_us? (current_us-pre_us): (UINT32_MAX+current_us-pre_us);
}


/************************ LLA Interface function ****************************/
/**
  * @brief	:	Init System time clock. (you can use SystemTick or other Hardware timer on your chip.)
  * @param	: 
  * @param	: 
  * @retval	:	
  */
void LLA_SYS_Time_Init(){
	SystemCoreClockUpdate();
  SysTick_Config(SysTick_LoadValue);
  NVIC_SetPriority(SysTick_IRQn, 0);
}


/**
  * @brief	:	Get System time in millisecond since system time clock initiation.
  * @param	: 
  * @param	: 
  * @retval	:	
  */
uint32_t LLA_SYS_Time_Millis(){

	return System_ms;
}


/**
  * @brief	:	Get System time in microsecond since system time clock initiation.
  * @param	: 
  * @param	: 
  * @retval	:	
  */
uint32_t LLA_SYS_Time_Micros(void)
{
   return (System_ms * 1000 + (SysTick_LoadValue - SysTick->VAL) / CYCLES_PER_MICROSECOND);
}


/**
  * @brief	:	delay a few millisecond.
  * @param	: 
  * @param	: 
  * @retval	:	
  */
void LLA_SYS_Time_DelayMS(uint32_t ms){
#if LLA_OS_SUPPORT
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED){//系统已经运行{
		vTaskDelay(ms);
	}else{
		uint32_t now = LLA_SYS_Time_Millis();
		while(LLA_SYS_Time_ConsumeMillis(now)<ms);
	}
#else
	uint32_t now = LLA_SYS_Time_Millis();
	while(LLA_SYS_Time_ConsumeMillis(now)<ms);
#endif
}



/**
  * @brief	:	delay a few microsecond.
  * @param	: 
  * @param	: 
  * @retval	:	
  */
void LLA_SYS_Time_DelayUS(uint32_t us){
		uint32_t total = 0;
    uint32_t target = CYCLES_PER_MICROSECOND * us;
    int last = SysTick->VAL;
    int now = last;
    int diff = 0;
start:
    now = SysTick->VAL;
    diff = last - now;
    if(diff > 0)
    {
        total += diff;
    }
    else
    {
        total += diff + SysTick_LoadValue;
    }
    if(total > target)
    {
        return;
    }
    last = now;
    goto start;
}

void SysTick_Handler(void)
{
    System_ms++;
#if LLA_OS_SUPPORT

	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
	{
		xPortSysTickHandler();
	}
#endif
}

