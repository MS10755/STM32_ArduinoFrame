#ifndef __FMC16BIT_H__
#define __FMC16BIT_H__

#include <stdint.h>


void FMC16Bit_Init();
void FMC16Bit_WriteCmd(uint16_t cmd);
void FMC16Bit_WriteData(uint16_t data);
uint16_t FMC16Bit_Read(void);
uint16_t FMC16Bit_ReadReg(uint16_t reg);

#endif

