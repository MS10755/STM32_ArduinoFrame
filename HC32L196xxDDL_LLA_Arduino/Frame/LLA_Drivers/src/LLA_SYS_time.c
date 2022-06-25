#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "ddl.h" 

/************************ LLA Interface micro define ****************************/
#define LLA_SYS_TIME_CLK	48000000

/************************ LLA common micro define ****************************/
#define CYCLES_PER_MICROSECOND (LLA_SYS_TIME_CLK/1000000U)



/************************ LLA common variables ****************************/
static volatile  uint32_t SYS_TIME_ms;



/************************ LLA common function ****************************/
uint32_t LLA_SYS_Time_ConsumeMillis(uint32_t pre_ms){
	uint32_t current_ms = LLA_SYS_Time_Millis();
	return current_ms>=pre_ms? (current_ms-pre_ms): (UINT32_MAX+current_ms-pre_ms);
}

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
	SYS_TIME_ms = 0;
	SysTick->LOAD = LLA_SYS_TIME_CLK/1000;
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk;
}


/**
  * @brief	:	Get System time in millisecond since system time clock initiation.
  * @param	: 
  * @param	: 
  * @retval	:	
  */
uint32_t LLA_SYS_Time_Millis(){
	return SYS_TIME_ms;
}


/**
  * @brief	:	Get System time in microsecond since system time clock initiation.
  * @param	: 
  * @param	: 
  * @retval	:	
  */
uint32_t LLA_SYS_Time_Micros(void)
{
    return (SYS_TIME_ms * 1000 + (SysTick->LOAD - SysTick->VAL) / CYCLES_PER_MICROSECOND);
}


/**
  * @brief	:	delay a few millisecond.
  * @param	: 
  * @param	: 
  * @retval	:	
  */
void LLA_SYS_Time_DelayMS(uint32_t ms){
	uint32_t now = LLA_SYS_Time_Millis();
	while(LLA_SYS_Time_ConsumeMillis(now)<ms);
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
        total += diff + SysTick->LOAD;
    }
    if(total > target)
    {
        return;
    }
    last = now;
    goto start;
}



void SysTick_Handler(void){
	SYS_TIME_ms++;
}

