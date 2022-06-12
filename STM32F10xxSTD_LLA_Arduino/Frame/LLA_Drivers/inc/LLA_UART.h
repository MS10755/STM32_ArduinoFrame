/*
 * @Description: LLA_UART module headers, DO NOT EDIT!!!
 * @Autor: Juan
 * @Date: 2022-04-01 14:49:54
 * @LastEditors: Juan
 * @LastEditTime: 2022-04-01 15:14:26
 */
#ifndef __LLA_UART_H__
#define __LLA_UART_H__


/* std lib includes */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

#define LLA_UART_PRINTF_BUFFER_LENGTH 512

typedef enum{
	LLA_UART1,
	LLA_UART2,
	LLA_UART3,
	LLA_UART4,
	LLA_UART5,
	LLA_UART6,
	LLA_UART7,
	LLA_UART8,
	
	_LLA_UART_MAX
}UART_name_t;


typedef enum{
	UART_CONFIG_8N1,
	UART_CONFIG_8N1_5,
	UART_CONFIG_8N2,
	UART_CONFIG_8E1,
	UART_CONFIG_8E1_5,
	UART_CONFIG_8E2,
	UART_CONFIG_8O1,
	UART_CONFIG_8O1_5,
	UART_CONFIG_8O2,
	
	_UART_CONCIG_MAX
}UART_config_t;


#ifdef __cplusplus
extern "C" {
#endif


void LLA_UART_Init(UART_name_t name,uint32_t baudRate,UART_config_t config);
void LLA_UART_DeInit(UART_name_t name);
void LLA_UART_Write(UART_name_t name,uint8_t data);
void LLA_UART_WriteBuffer(UART_name_t name,uint8_t *data,uint16_t len);
int LLA_UART_Printf(UART_name_t name,const char *__restrict __format, ...);
#ifdef __cplusplus
}
#endif


#endif
