#ifndef __W25Q128_USER_H__
#define __W25Q128_USER_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	
uint8_t w25q128_user_init(void);
uint8_t w25q128_user_read(uint32_t addr,uint8_t * buf,uint32_t len);
uint8_t w25q128_user_write(uint32_t addr,uint8_t * buf,uint32_t len);
uint8_t w25q128_user_erase_sector(uint32_t sector);

#ifdef __cplusplus
}
#endif
	
#endif
