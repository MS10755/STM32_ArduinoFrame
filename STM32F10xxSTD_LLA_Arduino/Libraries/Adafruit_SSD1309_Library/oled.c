#include "oled.h"
#include "stm32f10x.h"                  // Device header
#include "spi1.h"
#include "Frame.h"
//CS 	PA2
//DC 	PA1
//RST PA0


#define X_WIDTH 		128
#define Y_WIDTH 		64

#define RST_SET() GPIO_SetBits(GPIOA,GPIO_Pin_0);
#define RST_CLR() GPIO_ResetBits(GPIOA,GPIO_Pin_0);
#define CMD_SET() GPIO_SetBits(GPIOA,GPIO_Pin_1);
#define CMD_CLR() GPIO_ResetBits(GPIOA,GPIO_Pin_1);
#define CS_SET() GPIO_SetBits(GPIOA,GPIO_Pin_2);
#define CS_CLR() GPIO_ResetBits(GPIOA,GPIO_Pin_2);

static uint8_t OLED_BUF[Y_WIDTH/8][X_WIDTH];



void OLED_WriteCMD(uint8_t cmd){
	CS_CLR();
	CMD_CLR();
	SPI1_SendBytes(&cmd,1);
	CS_SET();
}

void OLED_WriteData(uint8_t data){
	CS_CLR();
	CMD_SET();
	SPI1_SendBytes(&data,1);
	CS_SET();
}

void OLED_SetPos(uint8_t page, uint8_t column){
	OLED_WriteCMD(column&0x0F);
	OLED_WriteCMD((column>>4) | 0x10);
	OLED_WriteCMD(page | 0xb0);
}


void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t color){
	uint8_t ByteData = OLED_BUF[y/8][x];
	if(color){
		ByteData |= 1<<(y % 8);
	}else{
		ByteData &=~(1<<(y % 8));
	}
	OLED_SetPos(y/8,x);
	OLED_WriteData(ByteData);
	OLED_BUF[y/8][x] = ByteData;
}





static void _OLED_GPIO_Init(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStrucutre;
	GPIO_InitStrucutre.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStrucutre.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStrucutre.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStrucutre);
	
}

void OLED_Init(){
	_OLED_GPIO_Init();
	SPI1_Init();
	RST_CLR();
	LLA_SYS_Time_DelayMS(200);
	RST_SET();
	OLED_WriteCMD(0xae);//--turn off oled panel
	OLED_WriteCMD(0xae);//--turn off oled panel
	OLED_WriteCMD(0x00);//---set low column address
	OLED_WriteCMD(0x10);//---set high column address
	OLED_WriteCMD(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WriteCMD(0x81);//--set contrast control register
	OLED_WriteCMD(0xcf); // Set SEG Output Current Brightness
	OLED_WriteCMD(0xa1);//--Set SEG/Column Mapping     0xa0,0xa1
	OLED_WriteCMD(0xc8);//Set COM/Row Scan Direction   0xc0,0xc8
	OLED_WriteCMD(0xa6);//--set normal display
	OLED_WriteCMD(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WriteCMD(0x3f);//--1/64 duty
	OLED_WriteCMD(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WriteCMD(0x00);//-not offset
	OLED_WriteCMD(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WriteCMD(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WriteCMD(0xd9);//--set pre-charge period
	OLED_WriteCMD(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WriteCMD(0xda);//--set com pins hardware configuration
	OLED_WriteCMD(0x12);
	OLED_WriteCMD(0xdb);//--set vcomh
	OLED_WriteCMD(0x40);//Set VCOM Deselect Level
	OLED_WriteCMD(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WriteCMD(0x02);//
	OLED_WriteCMD(0x8d);//--set Charge Pump enable/disable
	OLED_WriteCMD(0x14);//--set(0x10) disable
	OLED_WriteCMD(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WriteCMD(0xa6);// Disable Inverse Display On (0xa6/a7) 
	OLED_WriteCMD(0xaf);//--turn on oled panel
}



void OLED_Fill(unsigned char bmp_dat)//È«ÆÁÌî³ä
{
	for(uint8_t i=0;i<Y_WIDTH;i++){
		for(uint8_t j=0;j<X_WIDTH;j++){
			OLED_DrawPoint(j,i,bmp_dat);
		}
	}
}
