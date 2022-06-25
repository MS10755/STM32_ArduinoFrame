#ifndef __LLA_EXTI_H__
#define __LLA_EXTI_H__

#include "LLA_Drivers.h"

/* std lib includes */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum{
	LLA_EXTI_TYPE_LOW,//低电平触发
	LLA_EXTI_TYPE_HIGH,//高电平触发
	LLA_EXTI_TYPE_FALLING,//下降沿触发
	LLA_EXTI_TYPE_RISING,//上升沿触发
	LLA_ExTI_TYPE_CHANGE,//下降和上升沿都触发
	_LLA_EXTI_TYPE_MAX
}LLA_EXTI_TYPE_t;


typedef void (*LLA_EXTI_cb_t)(void);


#ifdef __cplusplus
extern "C" {
#endif

	
void LLA_EXTI_AttachInterrupt(BaseIO_name_t name, LLA_EXTI_cb_t cb,LLA_EXTI_TYPE_t type);
void LLA_EXTI_DetachInterrupt(BaseIO_name_t name);
	
#ifdef __cplusplus
}
#endif


#endif
