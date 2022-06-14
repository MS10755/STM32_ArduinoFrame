#ifndef __FMC16BIT_H__
#define __FMC16BIT_H__
#include "stm32f10x.h"                  // Device header
#include <stdint.h>

typedef struct
{
	vu16 LCD_REG;
	vu16 LCD_RAM;
} LCD_TypeDef;
    
#define LCD_BASE        ((u32)(0x6C000000 | 0x000007FE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)

void FMC16Bit_Init();
void FMC16Bit_WriteCmd(uint16_t cmd);
void FMC16Bit_WriteData(uint16_t data);
uint16_t FMC16Bit_Read(void);
uint16_t FMC16Bit_ReadReg(uint16_t reg);

#endif

