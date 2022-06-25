#ifndef __LLA_TEMPLATE_H__
#define __LLA_TEMPLATE_H__


/* std lib includes */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>



#ifdef __cplusplus
extern "C" {
#endif

void LLA_SYS_Time_Init(void);
uint32_t LLA_SYS_Time_Millis(void);
uint32_t LLA_SYS_Time_Micros(void);
void LLA_SYS_Time_DelayMS(uint32_t ms);
void LLA_SYS_Time_DelayUS(uint32_t us);
uint32_t LLA_SYS_Time_ConsumeMillis(uint32_t pre_ms);
uint32_t LLA_SYS_Time_ConsumeMicros(uint32_t pre_us);

	
#ifdef __cplusplus
}
#endif


#endif

