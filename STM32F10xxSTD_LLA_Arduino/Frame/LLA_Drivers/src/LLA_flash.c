/*
 * @Description: 
 * @Autor: Juan
 * @Date: 2022-04-01 14:49:54
 * @LastEditors: Juan
 * @LastEditTime: 2022-04-01 15:29:54
 */
#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "stm32f10x.h"                  // Device header


/************************ User type define ****************************/


/************************ User user variables ****************************/


/************************ User micro define ****************************/
/* Define the STM32F10x FLASH Page Size depending on the used STM32 device */
#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
  #define FLASH_PAGE_SIZE    ((uint16_t)0x800)
#else
  #define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#endif


#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
  #define FLASH_ADDR_START    ((uint32_t)0x0807F800)
	#define FLASH_ADDR_END			((uint32_t)0x0807FFFF)
#elif	defined (STM32F10X_MD) || defined (STM32F10X_MD_VL)
  #define FLASH_ADDR_START    ((uint32_t)0x0801FC00)
	#define FLASH_ADDR_END			((uint32_t)0x0801FFFF)
#else
  #define FLASH_ADDR_START    ((uint32_t)0x08007C00)
	#define FLASH_ADDR_END			((uint32_t)0x08007FFF)
#endif


/************************ User static function ****************************/


/************************ LLA common micro define ****************************/

/************************ LLA common variables ****************************/


/************************ LLA common function ****************************/


/************************ LLA Interface function ****************************/

/**
 * @description: 
 * @param {*}
 * @return {*}
 * @author: Your name
 */
void LLA_Flash_Init(){
}

/**
 * @description: 
 * @param {uint32_t} offset
 * @param {uint32_t} len
 * @return {*}
 * @author: Your name
 */
void LLA_Flash_Erase(uint32_t offset,uint32_t len){
	FLASH_Status status;
  uint32_t addr = FLASH_ADDR_START + offset;
	uint8_t pageNumber = len / FLASH_PAGE_SIZE;
	FLASH_UnlockBank1();
	for(uint8_t i=0; i<pageNumber; i++){
		status = FLASH_ErasePage(addr + (FLASH_PAGE_SIZE * i));
		if(status != FLASH_COMPLETE){
			FLASH_LockBank1();
			LLA_ASSERT(0, DriverCode_Flash, FLASH_errorErase);
			return;
		}
	}
	FLASH_LockBank1();
}

/**
 * @description: 
 * @param {uint32_t} offset
 * @param {uint8_t} *data
 * @param {uint32_t} len
 * @return {*}
 * @author: Your name
 */
void LLA_Flash_WriteBytes(uint32_t offset,uint8_t *data,uint32_t len){
	LLA_ASSERT(offset%2==0,DriverCode_Flash, FLASH_errorAddrAlignment);//F10x系列必须以2字节对齐写入
	uint32_t halfWordNum = len/2 + (len%2==0?0:1);
	
	uint32_t addr = FLASH_ADDR_START + offset;
	FLASH_UnlockBank1();
	while(halfWordNum--){
		if(addr>FLASH_ADDR_END){
			break;
		}
		uint16_t halfWord = 0;
		if(halfWordNum==0 && len%2){
			halfWord = data[0];
		}else{
			halfWord = data[1]<<8 | data[0];
		}
		FLASH_ProgramHalfWord(addr,halfWord);
		addr+=2;
		data+=2;
	}
	FLASH_LockBank1();
}

/**
 * @description: 
 * @param {uint32_t} offset
 * @param {uint8_t} *data
 * @param {uint32_t} len
 * @return {*}
 * @author: Your name
 */
void LLA_Flash_ReadBytes(uint32_t offset,uint8_t *data,uint32_t len){
	uint8_t * p = (uint8_t *)((uint32_t)FLASH_ADDR_START + offset);
	LLA_ASSERT(((uint32_t)p<=FLASH_ADDR_END), DriverCode_Flash, FLASH_errorAddr);
	while(len--){
		*data++ = *p++;
	}
}

/**
 * @description: 
 * @param {*}
 * @return {*}
 * @author: Your name
 */
uint32_t LLA_Flash_GetSize(){
	return (FLASH_ADDR_END - FLASH_ADDR_START + 1);;
}
/************************ Your Interrupt handling function ****************************/


