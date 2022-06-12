/*
 * @Description: LLA_ADC module headers, DO NOT EDIT!!!
 * @Autor: Juan
 * @Date: 2022-04-01 14:49:54
 * @LastEditors: Juan
 * @LastEditTime: 2022-04-01 15:10:31
 */
#ifndef __LLA_ADC_H__
#define __LLA_ADC_H__


/* std lib includes */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "LLA_baseIO.h"


typedef enum{
	LLA_ADC_SPECIAL_CHANNEL_0,
	LLA_ADC_SPECIAL_CHANNEL_1,
	LLA_ADC_SPECIAL_CHANNEL_2,
	LLA_ADC_SPECIAL_CHANNEL_3,
	LLA_ADC_SPECIAL_CHANNEL_4,
	
	_LLA_ADC_SPECIAL_CHANNEL_MAX
}LLA_ADC_Special_channel_t;

#ifdef __cplusplus
extern "C" {
#endif


void LLA_ADC_Init(void);
uint32_t LLA_ADC_Read(BaseIO_name_t name);
uint32_t LLA_ADC_ReadSpecial(LLA_ADC_Special_channel_t channel);
	
#ifdef __cplusplus
}
#endif


#endif
