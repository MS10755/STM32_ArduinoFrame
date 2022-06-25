#ifndef __LLA_ADVANCEIO_H__
#define __LLA_ADVANCEIO_H__

#include "./LLA_Drivers/inc/LLA_baseIO.h"

/* std lib includes */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum{
	LLA_LSBFIRST,//低位先出
	LLA_MSBFIRST//高位先出
}LLA_BitOrder_t;

typedef enum{
	AdvanceIO_PWC_F2F,//下降沿到下降沿模式
	AdvanceIO_PWC_R2R,//上升沿到上升沿模式
	AdvanceIO_PWC_F2R,//下降沿到上升沿模式
	AdvanceIO_PWC_R2F,//上升沿到下降沿模式
	
	_ADVANCEIO_PWC_MAX
}AdvanceIO_PWC_t;

#ifdef __cplusplus
extern "C" {
#endif


uint8_t LLA_AdvanceIO_ShiftIn(BaseIO_name_t dataPin, BaseIO_name_t clockPin, LLA_BitOrder_t bitOrder);
void LLA_AdvanceIO_ShiftOut(BaseIO_name_t dataPin, BaseIO_name_t clockPin, LLA_BitOrder_t bitOrder, uint8_t val);
uint32_t LLA_AdvanceIO_PluseMeasure(BaseIO_name_t name,AdvanceIO_PWC_t pwc,BaseIO_status_t idle_status,uint32_t time_out);

#ifdef __cplusplus
}
#endif


#endif
