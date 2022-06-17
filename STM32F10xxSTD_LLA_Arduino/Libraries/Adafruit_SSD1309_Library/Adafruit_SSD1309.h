#ifndef __ADAFRUIT_SSD1309_H__
#define __ADAFRUIT_SSD1309_H__
#include "Adafruit_GFX.h"

class Adafruit_SSD1309 : public Adafruit_GFX
{
	public:
		Adafruit_SSD1309() : Adafruit_GFX(128,64){
		};
	void begin(void);
	void drawPixel(int16_t x, int16_t y, uint16_t color) override;
};


#endif

