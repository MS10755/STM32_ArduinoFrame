#include "flash_cache.h"
#include <Frame.h>
#include <string.h>

void FLachCacheClass::begin(void){
	LLA_Flash_ReadBytes(0,_buffer,FLASH_CACHE_SIZE);
}

void FLachCacheClass::write(uint16_t index,uint8_t data){
	if(index>= FLASH_CACHE_SIZE){
		return;
	}
	_buffer[index] = data;
	
}
uint8_t FLachCacheClass::read(uint16_t index){
	if(index >= FLASH_CACHE_SIZE){
		return 0xFF;
	}
	return _buffer[index];
}
void FLachCacheClass::sync(void){
	static uint8_t _cmpBuf[FLASH_CACHE_SIZE];
	LLA_Flash_ReadBytes(0,_cmpBuf,FLASH_CACHE_SIZE);
	if(memcmp(_cmpBuf,_buffer,FLASH_CACHE_SIZE)!=0){
		LLA_Flash_Erase(0,FLASH_CACHE_SIZE);
		LLA_Flash_WriteBytes(0,_buffer,FLASH_CACHE_SIZE);
	}
}

FLachCacheClass FlashCache;
