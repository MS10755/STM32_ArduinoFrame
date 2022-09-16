#ifndef __FLASH_CACHE_H__
#define __FLASH_CACHE_H__

#include <stdint.h>

#define FLASH_CACHE_SIZE (0x800)

class FLachCacheClass{
	public:
		void begin(void);
		void write(uint16_t index,uint8_t data);
		uint8_t read(uint16_t index);
		void sync(void);
		uint8_t operator[](uint16_t index){
			return read(index);
		}
		
		
	private:
		uint8_t _buffer[FLASH_CACHE_SIZE];
};


extern FLachCacheClass FlashCache;

#endif
