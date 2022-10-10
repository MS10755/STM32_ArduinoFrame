/*
 * @Description: LLA_Flash module headers, DO NOT EDIT!!!
 * @Autor: Juan
 * @Date: 2022-04-01 14:49:54
 * @LastEditors: Juan
 * @LastEditTime: 2022-04-01 15:12:43
 */
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
uint8_t LLA_Flash_Erase(uint32_t offset,uint32_t len);
uint8_t LLA_Flash_WriteBytes(uint32_t offset,uint8_t *data,uint32_t len);
void LLA_Flash_ReadBytes(uint32_t offset,uint8_t *data,uint32_t len);

#ifdef __cplusplus
}
#endif


#endif
