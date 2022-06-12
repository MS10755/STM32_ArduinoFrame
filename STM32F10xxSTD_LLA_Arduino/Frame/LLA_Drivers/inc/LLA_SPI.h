/*
 * @Description: LLA_SPI module headers, DO NOT EDIT!!!
 * @Autor: Juan
 * @Date: 2022-04-01 14:49:54
 * @LastEditors: Juan
 * @LastEditTime: 2022-04-01 15:13:05
 */
#ifndef __LLA_SPI_H__
#define __LLA_SPI_H__


/* std lib includes */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


typedef enum{
	LLA_SPI0,
	LLA_SPI1,
	LLA_SPI2,
	LLA_SPI3,
	
	_LLA_SPI_MAX
}LLA_SPI_t;

typedef enum{
	LLA_SPI_Mode_CLK_L_CPHA_L,
	LLA_SPI_Mode_CLK_L_CPHA_H,
	LLA_SPI_Mode_CLK_H_CPHA_L,
	LLA_SPI_Mode_CLK_H_CPHA_H,
	
	_LLA_SPI_MODE_MAX
}LLA_SPI_Mode_t;


typedef enum{
	LLA_SPI_CS_soft,
	LLA_SPI_CS_Auto,
	
	_LLA_SPI_CS_MAX
}LLA_SPI_CS_t;

#ifdef __cplusplus
extern "C" {
#endif



void LLA_SPI_Init(LLA_SPI_t spi,uint32_t clock,LLA_SPI_Mode_t mode,LLA_SPI_CS_t cs_mode);
void LLA_SPI_DeInit(LLA_SPI_t spi);
void LLA_SPI_WriteBytes(LLA_SPI_t spi,uint8_t * data,uint32_t len);
void LLA_SPI_ReadBytes(LLA_SPI_t spi,uint8_t * buf,uint32_t len);
	
#ifdef __cplusplus
}
#endif


#endif
