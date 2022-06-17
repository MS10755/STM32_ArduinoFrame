#include "Adafruit_SSD1309.h"
#include "oled.h"

void Adafruit_SSD1309::begin(void){
	OLED_Init();
	OLED_Fill(0);
}
void Adafruit_SSD1309::drawPixel(int16_t x, int16_t y, uint16_t color){
	OLED_DrawPoint(x,y, color);
}
