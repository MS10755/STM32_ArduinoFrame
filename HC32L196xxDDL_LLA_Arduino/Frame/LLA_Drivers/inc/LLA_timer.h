#ifndef __LLA_TIMER_H__
#define __LLA_TIMER_H__


/* std lib includes */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


typedef enum{
	LLA_TIMER0,
	LLA_TIMER1,
	LLA_TIMER2,
	LLA_TIMER3,
	LLA_TIMER4,
	LLA_TIMER5,
	LLA_TIMER6,
	LLA_TIMER7,
	
	_LLA_TIMER_MAX
}LLA_Timer_t;

typedef void (*Timer_Handler_t)(void);

#ifdef __cplusplus
extern "C" {
#endif



void LLA_Timer_Set(LLA_Timer_t timer,uint32_t period,Timer_Handler_t handler);
void LLA_Timer_Start(LLA_Timer_t timer);
void LLA_Timer_Stop(LLA_Timer_t timer);
	
#ifdef __cplusplus
}
#endif


#endif
