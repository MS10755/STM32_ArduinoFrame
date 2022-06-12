/*
 * @Description: 
 * @Autor: Juan
 * @Date: 2022-04-01 14:49:54
 * @LastEditors: Juan
 * @LastEditTime: 2022-04-01 15:32:14
 */
#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "stm32f10x.h"                  // Device header


/************************ User type define ****************************/


/************************ User user variables ****************************/

/************************ User micro define ****************************/
#define SPI1_CLOCK  SystemCoreClock
#define SPI2_CLOCK (SystemCoreClock/2)
#define SPI3_CLOCK (SystemCoreClock/2)


#define SPI_I2S_GET_FLAG(SPIx, SPI_I2S_FLAG) 	(SPIx->SR & SPI_I2S_FLAG)
#define SPI_I2S_RXDATA(SPIx)               		(SPIx->DR)
#define SPI_I2S_RXDATA_VOLATILE(SPIx)     		 volatile uint16_t vn = SPI_I2S_RXDATA(SPIx)
#define SPI_I2S_TXDATA(SPIx, data)            (SPIx->DR = data)

/************************ User static function ****************************/


/************************ LLA common micro define ****************************/
#define IS_LLA_SPI(x) (x<_LLA_SPI_MAX)
#define IS_LLA_SPI_MODE(x) (x<_LLA_SPI_MODE_MAX)


static void _SPI_Init(
	SPI_TypeDef* SPIx,
	uint16_t SPI_Mode_x,
	uint16_t SPI_DataSize_x,
	uint16_t SPI_MODEx,
	uint16_t SPI_NSS_x,
	uint16_t SPI_BaudRatePrescaler_x,
	uint16_t SPI_FirstBit_x
){
	SPI_InitTypeDef SPI_InitStructure;
	uint16_t SPI_CPOL_x, SPI_CPHA_x;
  SPI_Cmd(SPIx, DISABLE);

    switch(SPI_MODEx)
    {
    case 0:
        SPI_CPOL_x = SPI_CPOL_Low;
        SPI_CPHA_x = SPI_CPHA_1Edge;
        break;
    case 1:
        SPI_CPOL_x = SPI_CPOL_Low;
        SPI_CPHA_x = SPI_CPHA_2Edge;
        break;
    case 2:
        SPI_CPOL_x = SPI_CPOL_High;
        SPI_CPHA_x = SPI_CPHA_1Edge;
        break;
    case 3:
        SPI_CPOL_x = SPI_CPOL_High;
        SPI_CPHA_x = SPI_CPHA_2Edge;
        break;
    }

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式
    SPI_InitStructure.SPI_Mode = SPI_Mode_x;        //设置SPI工作模式:(SPI_Mode_Master,SPI_Mode_Slave)
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_x;        //设置SPI的数据大小:SPI发送接收x位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_x;        //选择了串行时钟的稳态
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_x;    //数据捕获时钟沿
    SPI_InitStructure.SPI_NSS = SPI_NSS_x;      //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制(SPI_NSS_Soft,SPI_NSS_Hard)
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_x;      //定义波特率预分频的值
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_x;    //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始(SPI_FirstBit_MSB,SPI_FirstBit_LSB)
    SPI_InitStructure.SPI_CRCPolynomial = 3;    //CRC值计算的多项式
    SPI_Init(SPIx, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

    SPI_Cmd(SPIx, ENABLE); //使能SPI外设
}

static uint16_t calculateSpiBaudRatePrescaler(uint32_t spi_clock,uint32_t clock){
  uint16_t SPI_BaudRatePrescaler_x;
	uint16_t clock_div = spi_clock / clock;
	if(clock_div <= 2)SPI_BaudRatePrescaler_x = SPI_BaudRatePrescaler_2;
	else if(clock_div <= 4)SPI_BaudRatePrescaler_x = SPI_BaudRatePrescaler_4;
	else if(clock_div <= 8)SPI_BaudRatePrescaler_x = SPI_BaudRatePrescaler_8;
	else if(clock_div <= 16)SPI_BaudRatePrescaler_x = SPI_BaudRatePrescaler_16;
	else if(clock_div <= 32)SPI_BaudRatePrescaler_x = SPI_BaudRatePrescaler_32;
	else if(clock_div <= 64)SPI_BaudRatePrescaler_x = SPI_BaudRatePrescaler_64;
	else if(clock_div <= 128)SPI_BaudRatePrescaler_x = SPI_BaudRatePrescaler_128;
	else SPI_BaudRatePrescaler_x = SPI_BaudRatePrescaler_256;
	return SPI_BaudRatePrescaler_x;
}

static void Spi_SendBuf(SPI_TypeDef* SPIx, uint8_t* pu8Buf, uint32_t u32Len)
{
    uint32_t u32Index=0;

    for(u32Index=0; u32Index<u32Len; u32Index++)
    {
			while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_TXE)==RESET);
			SPI_I2S_SendData(SPIx,pu8Buf[u32Index]);
			while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE)==RESET);
			pu8Buf[u32Index] = SPI_I2S_ReceiveData(SPIx);
    }

	while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_TXE)==RESET);
	while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_BSY)==SET);
}

static void Spi_ReceiveBuf(SPI_TypeDef* SPIx, uint8_t* pu8Buf, uint32_t u32Len)
{
    uint32_t u32Index=0;

    for(u32Index=0; u32Index<u32Len; u32Index++)
    {
			while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_TXE)==RESET);
			SPI_I2S_SendData(SPIx,0x00);
			while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE)==RESET);
			pu8Buf[u32Index] = SPI_I2S_ReceiveData(SPIx);
    }
	while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_BSY)==SET);
}

/************************ LLA common variables ****************************/


/************************ LLA common function ****************************/


/************************ LLA Interface function ****************************/

/**
 * @description: 
 * @param {LLA_SPI_t} spi
 * @param {uint32_t} clock
 * @param {LLA_SPI_Mode_t} mode
 * @param {LLA_SPI_CS_t} cs_mode
 * @return {*}
 * @author: Your name
 */
void LLA_SPI_Init(LLA_SPI_t spi,uint32_t clock,LLA_SPI_Mode_t mode,LLA_SPI_CS_t cs_mode){
	LLA_ASSERT(spi<= LLA_SPI2, DriverCode_SPI, SPI_errorSPIOutChipSupport);
	LLA_ASSERT(IS_LLA_SPI_MODE(mode), DriverCode_SPI, SPI_errorMode);
	
	/* 1.Init SPI clock and gpio. */
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if(spi == LLA_SPI0){
		
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	}else if(spi == LLA_SPI1){
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	}else if(spi == LLA_SPI2){
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	}
	
	/* 2. init spi */

	if(spi == LLA_SPI0){
		
		_SPI_Init(
			SPI1,
			SPI_Mode_Master,
			SPI_DataSize_8b,
			mode,
			LLA_SPI_CS_soft == cs_mode? SPI_NSS_Soft : SPI_NSS_Hard,
			calculateSpiBaudRatePrescaler(SPI1_CLOCK,clock),
			SPI_FirstBit_MSB
		);
	
	}else if(spi == LLA_SPI1){
		
		_SPI_Init(
			SPI2,
			SPI_Mode_Master,
			SPI_DataSize_8b,
			mode,
			LLA_SPI_CS_soft == cs_mode? SPI_NSS_Soft : SPI_NSS_Hard,
			calculateSpiBaudRatePrescaler(SPI2_CLOCK,clock),
			SPI_FirstBit_MSB
		);
	
	}else if(spi == LLA_SPI2){
		
		_SPI_Init(
			SPI3,
			SPI_Mode_Master,
			SPI_DataSize_8b,
			mode,
			LLA_SPI_CS_soft == cs_mode? SPI_NSS_Soft : SPI_NSS_Hard,
			calculateSpiBaudRatePrescaler(SPI3_CLOCK,clock),
			SPI_FirstBit_MSB
		);
		
	}
	
}

/**
 * @description: 
 * @param {LLA_SPI_t} spi
 * @return {*}
 * @author: Your name
 */
void LLA_SPI_DeInit(LLA_SPI_t spi){
	LLA_ASSERT(spi<= LLA_SPI2, DriverCode_SPI, SPI_errorSPIOutChipSupport);
	
	if(spi == LLA_SPI0){
		
		SPI_Cmd(SPI1, DISABLE);
	
	}else if(spi == LLA_SPI1){
		
		SPI_Cmd(SPI2, DISABLE);
	
	}else if(spi == LLA_SPI2){
		
		SPI_Cmd(SPI3, DISABLE);
	}
}

/**
 * @description: 
 * @param {LLA_SPI_t} spi
 * @param {uint8_t *} data
 * @param {uint32_t} len
 * @return {*}
 * @author: Your name
 */
void LLA_SPI_WriteBytes(LLA_SPI_t spi,uint8_t * data,uint32_t len){
	LLA_ASSERT(spi<= LLA_SPI2, DriverCode_SPI, SPI_errorSPIOutChipSupport);
	SPI_TypeDef * SPIx;
	
	if(spi == LLA_SPI0){
		
		SPIx = SPI1;
	
	}else if(spi == LLA_SPI1){
		
		SPIx = SPI2;
	
	}else if(spi == LLA_SPI2){
		
		SPIx = SPI3;
		
	}
	Spi_SendBuf(SPIx, data, len);
}


/**
 * @description: 
 * @param {LLA_SPI_t} spi
 * @param {uint8_t *} buf
 * @param {uint32_t} len
 * @return {*}
 * @author: Your name
 */
void LLA_SPI_ReadBytes(LLA_SPI_t spi,uint8_t * buf,uint32_t len){
LLA_ASSERT(spi<= LLA_SPI2, DriverCode_SPI, SPI_errorSPIOutChipSupport);
	SPI_TypeDef * SPIx;
	
	if(spi == LLA_SPI0){
		
		SPIx = SPI1;
	
	}else if(spi == LLA_SPI1){
		
		SPIx = SPI2;
	
	}else if(spi == LLA_SPI2){
		
		SPIx = SPI3;
		
	}
	Spi_ReceiveBuf(SPIx, buf, len);
}

/************************ Your Interrupt handling function ****************************/


