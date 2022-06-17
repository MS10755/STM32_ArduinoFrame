#ifndef __OLED_H__
#define __OLED_H__
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


void OLED_Init();
void OLED_Fill(unsigned char bmp_dat);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t color);
#ifdef __cplusplus
}
#endif

#endif

