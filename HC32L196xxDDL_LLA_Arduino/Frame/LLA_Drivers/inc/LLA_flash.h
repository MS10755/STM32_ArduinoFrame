#ifndef __LLA_FLASH_H__
#define __LLA_FLASH_H__


/* std lib includes */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>



#ifdef __cplusplus
extern "C" {
#endif

void LLA_Flash_Init(void);
uint32_t LLA_Flash_GetSize(void);
void LLA_Flash_Erase(uint32_t offset,uint32_t len);
void LLA_Flash_WriteBytes(uint32_t offset,uint8_t *data,uint32_t len);
void LLA_Flash_ReadBytes(uint32_t offset,uint8_t *data,uint32_t len);
uint32_t* Get_ChipID(void);
#ifdef __cplusplus
}
#endif


#endif
