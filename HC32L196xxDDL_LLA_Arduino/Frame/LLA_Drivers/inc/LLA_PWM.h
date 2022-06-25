#ifndef __LLA_PWM_H__
#define __LLA_PWM_H__

#include "LLA_Drivers.h"

/* std lib includes */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>



typedef enum{
	PWM_Polar_HIGH,//PWM极性高电平
	PWM_Polar_LOW,//PWM极性低电平
	
	_PWM_POLAR_MAX
}PWM_Polar_t;


#ifdef __cplusplus
extern "C" {
#endif

void LLA_PWM_BaseHZ(uint32_t hz);
void LLA_PWM_Output1(BaseIO_name_t name,uint8_t duty,PWM_Polar_t polar);
void LLA_PWM_OutPut2(BaseIO_name_t name,uint32_t hz,uint8_t duty,PWM_Polar_t polar);
void LLA_PWM_Stop(BaseIO_name_t name);
#ifdef __cplusplus
}
#endif


#endif
