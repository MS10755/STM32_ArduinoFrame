/*
 * @Description: LLA_EXTI module headers, DO NOT EDIT!!!
 * @Autor: Juan
 * @Date: 2022-04-01 14:49:54
 * @LastEditors: Juan
 * @LastEditTime: 2022-04-01 15:12:31
 */
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
	LLA_EXTI_TYPE_LOW,
	LLA_EXTI_TYPE_HIGH,
	LLA_EXTI_TYPE_FALLING,
	LLA_EXTI_TYPE_RISING,
	LLA_ExTI_TYPE_CHANGE,
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
