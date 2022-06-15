#include "Adafruit_ILI9481FMC16.h"
extern "C" {
#include "fmc16bit.h"
}

#define SET_X_CMD	0x2A
#define SET_Y_CMD	0x2B
#define SET_RAM_CMD	0x2C

void Adafruit_ILI9481FMC16::begin(){
	FMC16Bit_Init();
		FMC16Bit_WriteCmd(0X11);
		delay(20);
		FMC16Bit_WriteCmd(0XD0);//VCI1  VCL  VGH  VGL DDVDH VREG1OUT power amplitude setting
		FMC16Bit_WriteData(0X07); 
		FMC16Bit_WriteData(0X42); 
		FMC16Bit_WriteData(0X1B);
		
		FMC16Bit_WriteCmd(0XD1);//VCOMH VCOM_AC amplitude setting
		FMC16Bit_WriteData(0X00);
		FMC16Bit_WriteData(0X1E);
		FMC16Bit_WriteData(0X12); 
		
		FMC16Bit_WriteCmd(0XD2);//Operational Amplifier Circuit Constant Current Adjust , charge pump frequency setting
		FMC16Bit_WriteData(0X01);
		FMC16Bit_WriteData(0X11);
		
		FMC16Bit_WriteCmd(0XC0);//REV SM GS 
		FMC16Bit_WriteData(0X10);
		FMC16Bit_WriteData(0X3B);
		FMC16Bit_WriteData(0X00);
		FMC16Bit_WriteData(0X12);
		FMC16Bit_WriteData(0X01);
		
		FMC16Bit_WriteCmd(0XC5);// Frame rate setting = 72HZ  when setting 0x03
		FMC16Bit_WriteData(0X02);
		
		FMC16Bit_WriteCmd(0XC8);//Gamma setting
		FMC16Bit_WriteData(0X02);
		FMC16Bit_WriteData(0X46);
		FMC16Bit_WriteData(0X14);
		FMC16Bit_WriteData(0X31);
		FMC16Bit_WriteData(0X0a);
		FMC16Bit_WriteData(0X04);
		FMC16Bit_WriteData(0X37);
		FMC16Bit_WriteData(0X24);
		FMC16Bit_WriteData(0X57);
		FMC16Bit_WriteData(0X13);
		FMC16Bit_WriteData(0X06);	
		FMC16Bit_WriteData(0X0C);
		
		FMC16Bit_WriteCmd(0X20);//Exit invert mode

		FMC16Bit_WriteCmd(0X36);
		FMC16Bit_WriteData(0X0A);//原来是a
		
		FMC16Bit_WriteCmd(0X3A);
		FMC16Bit_WriteData(0X05);//16位模式	
		
		FMC16Bit_WriteCmd(0X2A);
		FMC16Bit_WriteData(0X00);
		FMC16Bit_WriteData(0X00);
		FMC16Bit_WriteData(0X01);
		FMC16Bit_WriteData(0X3F);
		
		FMC16Bit_WriteCmd(0X2B);
		FMC16Bit_WriteData(0X00);
		FMC16Bit_WriteData(0X00);
		FMC16Bit_WriteData(0X01);
		FMC16Bit_WriteData(0XDF);
		delay(120);
		FMC16Bit_WriteCmd(0X29); 
}

void SetWindow(int16_t x, int16_t y, int16_t w, int16_t h){
		FMC16Bit_WriteCmd(SET_X_CMD); 
		FMC16Bit_WriteData(x>>8); 
		FMC16Bit_WriteData(x&0XFF);	 
		FMC16Bit_WriteData((x+w-1)>>8); 
		FMC16Bit_WriteData((x+w-1)&0XFF);  
		FMC16Bit_WriteCmd(SET_Y_CMD); 
		FMC16Bit_WriteData(y>>8); 
		FMC16Bit_WriteData(y&0XFF); 
		FMC16Bit_WriteData((y+h-1)>>8); 
		FMC16Bit_WriteData((y+h-1)&0XFF);
}

void Adafruit_ILI9481FMC16::drawPixel(int16_t x, int16_t y, uint16_t color){
	SetWindow(x,y,1,1);
	FMC16Bit_WriteCmd(SET_RAM_CMD);
	FMC16Bit_WriteData(color);
}

void Adafruit_ILI9481FMC16::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
	SetWindow(x,y,w,h);
	FMC16Bit_WriteCmd(SET_RAM_CMD);
	uint32_t points = w*h;
	for(uint32_t i=0;i<points;i++){
		LCD->LCD_RAM = color;
	}
}

void Adafruit_ILI9481FMC16::writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color){
	SetWindow(x,y,1,h);
	FMC16Bit_WriteCmd(SET_RAM_CMD);
	for(uint16_t i=0;i<h;i++){
		LCD->LCD_RAM =color;
	}
}
void Adafruit_ILI9481FMC16::writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color){
	SetWindow(x,y,w,1);
	FMC16Bit_WriteCmd(SET_RAM_CMD);
	for(uint16_t i=0;i<w;i++){
		LCD->LCD_RAM =color;
	}
}

void Adafruit_ILI9481FMC16::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
	writeFillRect(x,y,w,h,color);
}