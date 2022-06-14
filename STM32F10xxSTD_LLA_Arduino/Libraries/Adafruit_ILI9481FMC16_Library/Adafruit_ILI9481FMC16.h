#ifndef __ADAFRUIT_ILI9481FMC16_H__
#define __ADAFRUIT_ILI9481FMC16_H__
#include "Adafruit_GFX.h"

class Adafruit_ILI9481FMC16 : public Adafruit_GFX
{
	public:
		Adafruit_ILI9481FMC16() : Adafruit_GFX(320,480){
		};
	void begin(void);
	void drawPixel(int16_t x, int16_t y, uint16_t color) override;
	void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
	void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;
  void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
		
	void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
};


#endif

