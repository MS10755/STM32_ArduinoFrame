#ifndef __SPI1_H__
#define __SPI1_H__



#ifdef __cplusplus
extern "C" {
#endif
	
void SPI1_Init(void);
void SPI1_SendBytes(uint8_t *data, uint16_t len);
void SPI1_ReadBytes(uint8_t *data, uint16_t len);
	
#ifdef __cplusplus
}
#endif

#endif

