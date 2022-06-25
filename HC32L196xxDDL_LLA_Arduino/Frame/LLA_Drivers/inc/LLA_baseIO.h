#ifndef __LLA_BASEIO_H__
#define __LLA_BASEIO_H__


/* std lib includes */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum{
	BaseIOStatus_LOW,
	BaseIOStatus_HIGH,
	
	_BASEIO_STATUS_MAX
}BaseIO_status_t;


typedef enum{
	BaseIOMode_INPUT,//浮空输入
	BaseIOMode_INPUT_PU,//上拉输入
	BaseIOMode_INPUT_PD,//上拉输出
	BaseIOMode_OUTPUT,//推挽输出
	BaseIOMode_OUTPUT_OD,//开漏输出
	BaseIOMode_OUTPUT_OD_PU,//开漏上拉输出
	BaseIOMode_OUTPUT_OD_PD,//开漏下拉输出
	
	_BASEIO_MODE_MAX
}BaseIO_mode_t;

typedef enum{
	PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PA8,PA9,PA10,PA11,PA12,PA13,PA14,PA15,
	PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,PB8,PB9,PB10,PB11,PB12,PB13,PB14,PB15,
	PC0,PC1,PC2,PC3,PC4,PC5,PC6,PC7,PC8,PC9,PC10,PC11,PC12,PC13,PC14,PC15,
	PD0,PD1,PD2,PD3,PD4,PD5,PD6,PD7,PD8,PD9,PD10,PD11,PD12,PD13,PD14,PD15,
	PE0,PE1,PE2,PE3,PE4,PE5,PE6,PE7,PE8,PE9,PE10,PE11,PE12,PE13,PE14,PE15,
	PF0,PF1,PF2,PF3,PF4,PF5,PF6,PF7,PF8,PF9,PF10,PF11,PF12,PF13,PF14,PF15,
	PG0,PG1,PG2,PG3,PG4,PG5,PG6,PG7,PG8,PG9,PG10,PG11,PG12,PG13,PG14,PG15,
	
	_BASEIO_NAME_MAX,
}BaseIO_name_t;



#define IS_IO_NAME(x)		(x<_BASEIO_NAME_MAX)
#define IS_IO_MODE(x)		(x<_BASEIO_MODE_MAX)
#define IS_IO_STATUS(x)	(x<_BASEIO_STATUS_MAX)


#ifdef __cplusplus
extern "C" {
#endif

void LLA_BaseIO_Mode(BaseIO_name_t name,BaseIO_mode_t mode,BaseIO_status_t outputStatus);
void LLA_BaseIO_Write(BaseIO_name_t name,BaseIO_status_t outputStatus);
BaseIO_status_t LLA_BaseIO_ReadInput(BaseIO_name_t name);
BaseIO_status_t LLA_BaseIO_ReadOutput(BaseIO_name_t name);
BaseIO_status_t LLA_BaseIO_Toggle(BaseIO_name_t name);

#ifdef __cplusplus
}
#endif


#endif
